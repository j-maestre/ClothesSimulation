#include "public/rope.h"
#include "public/wind_turbine.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

namespace JE {


void Rope::InitRope(Vec3 first_pos, Vec3 direction, float mass, float friction_factor, bool enable_collision, CollisionManager* cm){

    
    m_points = new Point[m_numParticles];
    assert(m_points && "New Points is null");

    Vec3 second_pos = first_pos + direction;

    m_enabled_collision = enable_collision;
    if (enable_collision) m_cm = cm;

    for (int i = 0; i < m_numParticles; i++) {

        // How close are we to the last point?
        float w = (float)i / (m_numParticles - 1);

        float x = w * second_pos.x + (1.0f - w) * first_pos.x;
        float y = w * second_pos.y + (1.0f - w) * first_pos.y;
        float z = w * second_pos.z + (1.0f - w) * first_pos.z;

   
        m_points[i].position.x = x;
        m_points[i].position.y = y;
        m_points[i].position.z = z;

        m_points[i].previous_position.x = x;
        m_points[i].previous_position.y = y;
        m_points[i].previous_position.z = z;
        
        m_points[i].mass = mass;
        m_points[i].friction_factor = friction_factor * 0.1f;
        m_points[i].fixed = i == 0; // We fix only the first point

        //unsigned int numberOfSegments = m_numParticles - 1;

        float ropeLength = sqrt(pow(first_pos.x - second_pos.x, 2.0f) + pow(first_pos.y - second_pos.y, 2.0f) + pow(first_pos.z - second_pos.z, 2.0f));
        m_desiredDistance = ropeLength / (m_numParticles - 1);

        if (enable_collision) {
            m_points[i].sphere_collision = cm->CreateSphereCollision(0.01f, Vec3(x,y,z));
        }

    }

}

Rope::Rope(float lenght, unsigned int particles) : m_lenght(lenght), m_numParticles(particles){
    m_points = nullptr;
}

Rope::Rope(const Rope& other){
    //other.m_desiredDistance = m_desiredDistance;
}

Rope::Rope(Rope&& other){

}

Rope::~Rope(){
    if (m_points) {
        delete[] m_points;
    }
}

void Rope::Update(float dt){


    m_timeStep = dt;
    // Verlet integration    
    for (int i = 0; i < m_numParticles; i++) {
        if (!m_points[i].fixed) {
            float x_tmp = m_points[i].position.x;
            float y_tmp = m_points[i].position.y;
            float z_tmp = m_points[i].position.z;


            // Calculating previous velocity
            float colliding = 1.0f;
            if (m_enabled_collision && m_cm->GetsphereCollision(m_points[i].sphere_collision).GetColliding()) colliding = -0.5f;
            float xVelocity = ((m_points[i].position.x - m_points[i].previous_position.x) * colliding) / m_timeStep;
            float yVelocity = ((m_points[i].position.y - m_points[i].previous_position.y) * colliding) / m_timeStep;
            float zVelocity = ((m_points[i].position.z - m_points[i].previous_position.z) * colliding) / m_timeStep;

            float gravity_force = m_gravity * m_points[i].mass;
            float acceleration_y = gravity_force / m_points[i].mass;

            yVelocity += gravity_force * m_timeStep;

            xVelocity *= (1 - m_points[i].friction_factor);
            yVelocity *= (1 - m_points[i].friction_factor);
            zVelocity *= (1 - m_points[i].friction_factor);
            
            // Update new position using Verlet
            m_points[i].position.x += xVelocity * m_timeStep;
            m_points[i].position.y += yVelocity * m_timeStep;
            m_points[i].position.z += zVelocity * m_timeStep;

            m_points[i].previous_position.x = x_tmp;
            m_points[i].previous_position.y = y_tmp;
            m_points[i].previous_position.z = z_tmp;

            if(m_enabled_collision)m_cm->GetsphereCollision(m_points[i].sphere_collision).m_position = Vec3(m_points[i].position.x, m_points[i].position.y, m_points[i].position.z);
        }
    }


    // Enforce constraints
    for (int iterations = 0; iterations < m_jakobsenIterations; iterations++) {
        
        for (int i = 1; i < m_numParticles; i++) {
            Point& previous = m_points[i - 1];
            Point& actual = m_points[i];
        
            float distance = sqrt(pow(previous.position.x - actual.position.x, 2) + pow(previous.position.y - actual.position.y, 2));
            float distanceError = distance - m_desiredDistance;

             // The direction in which particles should be pulled or pushed
            
            float xDifference = actual.position.x - previous.position.x;
            float yDifference = actual.position.y - previous.position.y;
            float zDifference = actual.position.z - previous.position.z;

            // Make unit vector
            float distanceSquared = (xDifference * xDifference) + (yDifference * yDifference) + (zDifference * zDifference);

            if (distanceSquared > 0.0f) {
                float distance = sqrt(distanceSquared);
                
                float xDirection = xDifference / distance;
                float yDirection = yDifference / distance;
                float zDirection = zDifference / distance;

                float totalMass =actual.mass + previous.mass;
                float correction_factor_previous = previous.mass / totalMass;
                float correction_factor_actual = actual.mass / totalMass;


                if (previous.fixed && !actual.fixed) {
                    // First with the second

                    actual.position.x -= correction_factor_previous * (xDirection * distanceError);
                    actual.position.y -= correction_factor_previous * (yDirection * distanceError);
                    actual.position.z -= correction_factor_previous * (zDirection * distanceError);

                }
                else if (actual.fixed && !previous.fixed) {
                    // Second with the first

                    previous.position.x += correction_factor_actual * (xDirection * distanceError);
                    previous.position.y += correction_factor_actual * (yDirection * distanceError);
                    previous.position.z += correction_factor_actual * (zDirection * distanceError);

                }
                else if (!previous.fixed && !actual.fixed) {
                    // All except first

                    actual.position.x -= 0.5 * (xDirection * distanceError);
                    actual.position.y -= 0.5 * (yDirection * distanceError);
                    actual.position.z -= 0.5f * (zDirection * distanceError);

                    previous.position.x += 0.5 * (xDirection * distanceError);
                    previous.position.y += 0.5 * (yDirection * distanceError);
                    previous.position.z += 0.5f * (zDirection * distanceError);
                }
            }
            //float xDirection = xDifference / sqrt(pow(xDifference, 2) + pow(yDifference, 2));
            //float yDirection = yDifference / sqrt(pow(xDifference, 2) + pow(yDifference, 2));

            
        }
    }

}


void Rope::ApplyWindTurbine(WindTurbine wind, float dt){
    for (int i = 0; i < m_numParticles; i++) {
        Point& point = m_points[i];

        // Vector desde el origen del viento hacia el punto de la cuerda
        Vec3 pointToWindOrigin = point.position - wind.m_position;

        // Proyección del punto en la dirección del viento
        float distanceAlongWind = pointToWindOrigin.Dot(wind.m_direction);

        // Verificar si el punto está dentro del alcance del viento
        if (distanceAlongWind > 0 && distanceAlongWind < wind.m_maxDistance) {
            // Ángulo entre la dirección del viento y el punto
            Vec3 projectedPoint = pointToWindOrigin - (wind.m_direction * distanceAlongWind);
            float distanceFromCenter = projectedPoint.Length();

            // Si está dentro del ángulo de expansión del viento
            float maxRadiusAtDistance = distanceAlongWind * tan(wind.m_spreadAngle);
            if (distanceFromCenter < maxRadiusAtDistance) {
                // Calcular la fuerza del viento que afecta al punto
                float distanceFactor = 1.0f - (distanceAlongWind / wind.m_maxDistance);
                float windForce = wind.m_strength * distanceFactor;

                // Aplicar la fuerza en la dirección del viento
                Vec3 force = wind.m_direction * windForce;

                // Aumentar la velocidad del punto de la cuerda con esta fuerza (proporcional a su masa)
                // Multiplicar por dt para aplicar la fuerza proporcional al tiempo
                point.position.x += force.x * dt / point.mass; // Ajustado para fuerza
                point.position.y += force.y * dt / point.mass; // Ajustado para fuerza
                point.position.z += force.z * dt / point.mass; // Ajustado para fuerza
            }
        }
    }

}

void Rope::GetPosition(int index, float& x, float& y, float& z){

    x = m_points[index].position.x;
    y = m_points[index].position.y;
    z = m_points[index].position.z;
}

void Rope::SetPointPosition(int index, float x, float y, float z){

    m_points[index].position.x = x;
    m_points[index].position.y = y;
    m_points[index].position.z = z;

}

void Rope::TranslateRope(float x, float y, float z){

    for (int i = 0; i < m_numParticles; i++) {
        m_points[i].position.x = x;
        m_points[i].position.y = y;
        m_points[i].position.z = z;
        
        m_points[i].previous_position.x = x;
        m_points[i].previous_position.y = y;
        m_points[i].previous_position.z = z;
    }
}

void Rope::SetFixed(unsigned int index, bool fixed){
    assert(index < m_numParticles);
    m_points[index].fixed = fixed;
}

void Rope::SetAllFriction(float friction){

    if (friction >= 0.0f) {

        for (int i = 0; i < m_numParticles; i++) {
            m_points[i].friction_factor = friction;
        }
    }
}

void Rope::SetFriction(int index, float friction){
    if (index >= 0 && friction >= 0.0f) {
        m_points[index].friction_factor = friction;
    }
}

void Rope::SetStepSize(float step){

}

void Rope::SetMass(int index, float mass){
    m_points[index].mass = mass;
}

void Rope::SetAllMass(float mass){
    for (int i = 0; i < m_numParticles; i++) {
        m_points[i].mass = mass;
    }
}


};