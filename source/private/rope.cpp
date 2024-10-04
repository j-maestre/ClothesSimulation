#include "public/rope.h"
#include <assert.h>
#include <math.h>

void Rope::InitRope(float x1, float y1, float z1, float x2, float y2, float z2){
        


    //Rope* rope_tmp = new Rope();
    //assert(rope_tmp && "new Rope is nullptr");
    
    m_points = new Point[m_numParticles];
    assert(m_points && "New Points is null");

    for (int i = 0; i < m_numParticles; i++) {

        // How close are we to the last point?
        float w = (float)i / (m_numParticles - 1);

        float x = w * x2 + (1 - w) * x1;
        float y = w * y2 + (1 - w) * y1;
        float z = w * z2 + (1 - w) * z1;

   
        m_points[i].position[0] = x;
        m_points[i].position[1] = y;
        m_points[i].position[2] = z;

        m_points[i].previous_position[0] = x;
        m_points[i].previous_position[1] = y;
        m_points[i].previous_position[2] = z;
        
        m_points[i].fixed = i == 0; // We fix only the first point

        unsigned numberOfSegments = m_numParticles - 1;

        float ropeLength = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
        m_desiredDistance = ropeLength / (m_numParticles - 1);

    }




  
}

Rope::Rope(float lenght, unsigned int particles) : m_lenght(lenght), m_numParticles(particles){
    m_points = nullptr;
}

Rope::Rope(const Rope&){

}

Rope::Rope(Rope&&){

}

Rope::~Rope(){
    if (m_points) {
        delete[] m_points;
    }
}

void Rope::Update(float dt){

    // Verlet integration    
    for (int i = 0; i < m_numParticles; i++) {
        if (!m_points[i].fixed) {
            float x_tmp = m_points[i].position[0];
            float y_tmp = m_points[i].position[1];
            float z_tmp = m_points[i].position[2];


            // Calculating previous velocity
            float xVelocity = (m_points[i].position[0] - m_points[i].previous_position[0]) / m_timeStep;
            float yVelocity = (m_points[i].position[1] - m_points[i].previous_position[1]) / m_timeStep;
            float zVelocity = (m_points[i].position[2] - m_points[i].previous_position[2]) / m_timeStep;

            yVelocity += m_gravity * m_timeStep;
            
            // Update new position using Verlet
            m_points[i].position[0] += xVelocity * m_timeStep;
            m_points[i].position[1] += yVelocity * m_timeStep;
            m_points[i].position[2] += zVelocity * m_timeStep;

            // Calculating instant velocity
            //float xInstantVelocity = xVelocity + 0 * m_timeStep;
            //float yInstantVelocity = yVelocity + m_gravity * m_timeStep;

            // Taking the mean of the two
            //float xTrueVelocity = (xVelocity + xInstantVelocity) / 2.0f;
            //float yTrueVelocity = (yVelocity + yInstantVelocity) / 2.0f;

            // Updating particle's position
            //m_points[i].position[0] += xTrueVelocity * m_timeStep;
            //m_points[i].position[1] += yTrueVelocity * m_timeStep;
            // Z value
            // m_points[i].position[2] = 2.0f * m_points[i].position[2] - m_points[i].previous_position[2] * (m_timeStep * m_timeStep) * dt;

            m_points[i].previous_position[0] = x_tmp;
            m_points[i].previous_position[1] = y_tmp;
            m_points[i].previous_position[2] = z_tmp;
        }
    }


    // Enforce constraints
    for (int iterations = 0; iterations < jm_jakobsenIterations; iterations++) {
        
        for (int i = 1; i < m_numParticles; i++) {
            Point& previous = m_points[i - 1];
            Point& actual = m_points[i];
        
            float distance = sqrt(pow(previous.position[0] - actual.position[0], 2) + pow(previous.position[1] - actual.position[1], 2));
            float distanceError = distance - m_desiredDistance;

             // The direction in which particles should be pulled or pushed
            float xDifference = actual.position[0] - previous.position[0];
            float yDifference = actual.position[1] - previous.position[1];
            float zDifference = actual.position[2] - previous.position[2];

            // Make unit vector
            float distanceSquared = xDifference * xDifference + yDifference * yDifference + zDifference * zDifference;

            if (distanceSquared > 0.0f) {
                float distance = sqrt(distanceSquared);
                float distanceError = distance - m_desiredDistance;
                
                float xDirection = xDifference / distance;
                float yDirection = yDifference / distance;
                float zDirection = zDifference / distance;


                if (previous.fixed && !actual.fixed) {
                    // First with the second

                    actual.position[0] -= xDirection * distanceError;
                    actual.position[1] -= yDirection * distanceError;
                    actual.position[2] -= zDirection * distanceError;

                }
                else if (actual.fixed && !previous.fixed) {
                    // Second with the first

                    previous.position[0] += xDirection * distanceError;
                    previous.position[1] += yDirection * distanceError;
                    previous.position[2] += zDirection * distanceError;

                }
                else if (!previous.fixed && !actual.fixed) {
                    // All except first

                    actual.position[0] -= 0.5 * xDirection * distanceError;
                    actual.position[1] -= 0.5 * yDirection * distanceError;
                    actual.position[2] -= 0.5f * zDirection * distanceError;

                    previous.position[0] += 0.5 * xDirection * distanceError;
                    previous.position[1] += 0.5 * yDirection * distanceError;
                    previous.position[2] += 0.5f * zDirection * distanceError;
                }
            }
            //float xDirection = xDifference / sqrt(pow(xDifference, 2) + pow(yDifference, 2));
            //float yDirection = yDifference / sqrt(pow(xDifference, 2) + pow(yDifference, 2));

            
        }
    }



}

void Rope::GetPosition(int index, float& x, float& y, float& z){

    x = m_points[index].position[0];
    y = m_points[index].position[1];
    z = m_points[index].position[2];
}

void Rope::SetStepSize(float step){

}

void Rope::SetMass(float mass){

}
