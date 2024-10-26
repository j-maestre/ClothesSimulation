#include "public/cloth.h"
#include "assert.h"

namespace JE{

    Cloth::Cloth(unsigned int rows, unsigned int columns, float length_x, float lenght_y) : m_rows(rows), m_columns(columns), m_length_per_rope(length_x){

        // Get the distance between particles I need for each rope, basing on rows, columns and length
        const float desired_distance_per_particle = 1.0f; // Ajusta según sea necesario
        m_num_particles_per_rope = columns;

        m_desired_distance_per_rope = length_x / (columns - 1);
        m_distance_between_rows = lenght_y / (rows - 1);

    }

    inline void Cloth::UpdateJaksobenParticlesPair(Cloth& cloth, Point* previous, Point* actual, float dt) {

            float distance = sqrt(pow(previous->position.x - actual->position.x, 2) + pow(previous->position.y - actual->position.y, 2));
            float distanceError = distance - cloth.m_desired_distance_per_rope;

            // The direction in which particles should be pulled or pushed
            float xDifference = actual->position.x - previous->position.x;
            float yDifference = actual->position.y - previous->position.y;
            float zDifference = actual->position.z - previous->position.z;

            // Make unit vector
            float distanceSquared = (xDifference * xDifference) + (yDifference * yDifference) + (zDifference * zDifference);

            if (distanceSquared > 0.0f) {
                float distance = sqrt(distanceSquared);

                float xDirection = xDifference / distance;
                float yDirection = yDifference / distance;
                float zDirection = zDifference / distance;

                float totalMass = actual->mass + previous->mass;
                float correction_factor_previous = previous->mass / totalMass;
                float correction_factor_actual = actual->mass / totalMass;


                if (previous->fixed && !actual->fixed) {
                    // First with the second

                    actual->position.x -= correction_factor_previous * (xDirection * distanceError);
                    actual->position.y -= correction_factor_previous * (yDirection * distanceError);
                    actual->position.z -= correction_factor_previous * (zDirection * distanceError);

                }
                else if (actual->fixed && !previous->fixed) {
                    // Second with the first

                    previous->position.x += correction_factor_actual * (xDirection * distanceError);
                    previous->position.y += correction_factor_actual * (yDirection * distanceError);
                    previous->position.z += correction_factor_actual * (zDirection * distanceError);

                }
                else if (!previous->fixed && !actual->fixed) {
                    // All except first

                    actual->position.x -= 0.5 * (xDirection * distanceError);
                    actual->position.y -= 0.5 * (yDirection * distanceError);
                    actual->position.z -= 0.5f * (zDirection * distanceError);

                    previous->position.x += 0.5 * (xDirection * distanceError);
                    previous->position.y += 0.5 * (yDirection * distanceError);
                    previous->position.z += 0.5f * (zDirection * distanceError);
                }
            }
            //float xDirection = xDifference / sqrt(pow(xDifference, 2) + pow(yDifference, 2));
            //float yDirection = yDifference / sqrt(pow(xDifference, 2) + pow(yDifference, 2));
        
    }
    
    Cloth::Cloth(const Cloth&) {

    }

    Cloth::Cloth(Cloth&&) {

    }

    Cloth::~Cloth() {

        for (Point* p : m_ropes) {
            delete[] p;
        }

    }

    void Cloth::InitClothe(Vec3 first_pos, float mass, float friction_factor) {

        assert(m_num_particles_per_rope > 1 && "El número de partículas debe ser mayor que 1");

        m_ropes.resize(m_rows);

        // Calcula la posición del último punto en la cuerda
        Vec3 second_pos = first_pos;
        second_pos.x += m_desired_distance_per_rope;

        for (int row = 0; row < m_rows; row++) {
            m_ropes[row] = new Point[m_columns];
            assert(m_ropes[row] && "New Points is null");

            Vec3 current_row_pos = first_pos;
            current_row_pos.y -= row * m_distance_between_rows;

            for (int col = 0; col < m_columns; col++) {

                // Interpolation factor between first and second point of the row
                float w = (float)col / (m_columns - 1);

                // Linear interpolation between first_pos and second_pos
                // float x = w * second_pos.x + (1.0f - w) * first_pos.x;
                float x = first_pos.x + (m_desired_distance_per_rope * col);
                float y = current_row_pos.y;
                float z = first_pos.z;

                // Asigna la posición calculada al punto
                m_ropes[row][col].position = Vec3(x, y, z);
                m_ropes[row][col].previous_position = Vec3(x, y, z);
                m_ropes[row][col].mass = mass;
                m_ropes[row][col].friction_factor = friction_factor * 0.1f;

                // Fijar puntos en la primera fila
                //m_ropes[row][col].fixed = (row == 0 && col == 0);
            }
        }

        // Calcula la distancia deseada entre puntos basada en el tamaño de la malla
        //m_desired_distance_per_rope = m_length_per_rope / (m_num_particles_per_rope - 1);
    }


    void Cloth::GetPosition(unsigned int row, unsigned int index, float& x, float& y, float& z){

        x = m_ropes[row][index].position.x;
        y = m_ropes[row][index].position.y;
        z = m_ropes[row][index].position.z;
    }

    void Cloth::SetPosition(unsigned int row, unsigned int index, float x, float y, float z){
        m_ropes[row][index].position.x = x;
        m_ropes[row][index].position.y = y;
        m_ropes[row][index].position.z = z;
    }

    void Cloth::SetFixed(unsigned int x, unsigned int y, bool fixed){
        m_ropes[y][x].fixed = fixed;
    }

    void Cloth::Update(float dt) {
        for (unsigned int y = 0; y < m_rows; y++) {
            for (int x = 0; x < m_num_particles_per_rope; x++) {
                if (!m_ropes[y][x].fixed) {
                    // Guardar posición actual para el siguiente cálculo de velocidad
                    Vec3 current_position = m_ropes[y][x].position;

                    // Calcular velocidades en cada eje
                    Vec3 velocity = (m_ropes[y][x].position - m_ropes[y][x].previous_position) / dt;

                    // Aplicar gravedad solo en el eje y
                    velocity.y += m_gravity * dt;

                    // Aplicar fricción a la velocidad en todos los ejes
                    velocity *= (1.0f - m_ropes[y][x].friction_factor);

                    // Actualizar la posición usando integración de Verlet
                    m_ropes[y][x].position += velocity * dt;

                    // Actualizar la posición previa para el próximo frame
                    m_ropes[y][x].previous_position = current_position;
                }
            }
        }

        // Jakobsen Horizontal
        for (int iterations = 0; iterations < m_jakobsenIterations; iterations++) {
            // Restringir distancias horizontales (entre puntos en una misma fila)
            for (unsigned int y = 0; y < m_rows; y++) {
                for (int x = 1; x < m_num_particles_per_rope; x++) {
                    Point* previous = &m_ropes[y][x - 1];
                    Point* current = &m_ropes[y][x];
                    UpdateJaksobenParticlesPair(*this, previous, current, dt);
                }
            }

            // Restringir distancias verticales (entre puntos en diferentes filas)
            for (unsigned int y = 1; y < m_rows; y++) {
                for (int x = 0; x < m_num_particles_per_rope; x++) {
                    Point* previous = &m_ropes[y - 1][x];
                    Point* current = &m_ropes[y][x];
                    UpdateJaksobenParticlesPair(*this, previous, current, dt);
                }
            }
        }
    }

};