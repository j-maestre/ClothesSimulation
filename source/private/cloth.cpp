#include "public/cloth.h"
#include "assert.h"


namespace JE {

    Cloth::Cloth(unsigned int rows, unsigned int columns, float length_x, float lenght_y) : m_rows(rows), m_columns(columns), m_length_per_rope(length_x) {

        // Get the distance between particles I need for each rope, basing on rows, columns and length
        const float desired_distance_per_particle = 1.0f; // Ajusta seg�n sea necesario
        m_num_particles_per_rope = columns;

        m_desired_distance_per_rope = length_x / (columns - 1);
        m_distance_between_rows = lenght_y / (rows - 1);

    }

    inline void Cloth::UpdateJaksobenParticlesPair(Cloth& cloth, Point* previous, Point* actual) {

        float distance = sqrtf(pow(previous->position.x - actual->position.x, 2.0f) + pow(previous->position.y - actual->position.y, 2.0f) + pow(previous->position.z - actual->position.z, 2.0f));
        float distanceError = distance - cloth.m_desired_distance_per_rope;

        // The direction in which particles should be pulled or pushed
        float xDifference = actual->position.x - previous->position.x;
        float yDifference = actual->position.y - previous->position.y;
        float zDifference = actual->position.z - previous->position.z;

        // Make unit vector
        float distanceSquared = (xDifference * xDifference) + (yDifference * yDifference) + (zDifference * zDifference);
        //float distanceSquared = (xDifference + yDifference + zDifference);

        if (distanceSquared > 0.0f) {
            float distance = sqrtf(distanceSquared);
            //float distance = distanceSquared;

            float xDirection = xDifference / distance;
            float yDirection = yDifference / distance;
            float zDirection = zDifference / distance;

            float totalMass = actual->mass + previous->mass;
            float correction_factor_previous = previous->mass / totalMass;
            float correction_factor_actual = actual->mass / totalMass;


            if (previous->fixed && !actual->fixed) {

                actual->position.x -= correction_factor_previous * (xDirection * distanceError);
                actual->position.y -= correction_factor_previous * (yDirection * distanceError);
                actual->position.z -= correction_factor_previous * (zDirection * distanceError);

            }
            else if (actual->fixed && !previous->fixed) {

                previous->position.x += correction_factor_actual * (xDirection * distanceError);
                previous->position.y += correction_factor_actual * (yDirection * distanceError);
                previous->position.z += correction_factor_actual * (zDirection * distanceError);

            }
            else if (!previous->fixed && !actual->fixed) {

                float x_distance = 0.5f * (xDirection * distanceError);
                float y_distance = 0.5f * (yDirection * distanceError);
                float z_distance = 0.5f * (zDirection * distanceError);

                actual->position.x -= x_distance;
                actual->position.y -= y_distance;
                actual->position.z -= z_distance;

                previous->position.x += x_distance;
                previous->position.y += y_distance;
                previous->position.z += z_distance;
            }
        }

    }

    Cloth::Cloth(const Cloth&) {

    }

    Cloth::Cloth(Cloth&&) {

    }

    Cloth::~Cloth() {

        for (Point* p : m_ropes) {
            delete[] p;
        }

        m_ropes.clear();
    }

    void Cloth::InitClothe(Vec3 first_pos, float mass, float friction_factor) {

        assert(m_num_particles_per_rope > 1 && "El numero de particulas debe ser mayor que 1");

        m_ropes.resize(m_rows);

        // Calcula la posici�n del �ltimo punto en la cuerda
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
                float z = first_pos.z; // +(m_desired_distance_per_rope * col);

                // Asigna la posici�n calculada al punto
                m_ropes[row][col].position = Vec3(x, y, z);
                m_ropes[row][col].previous_position = Vec3(x, y, z);
                m_ropes[row][col].mass = mass;
                m_ropes[row][col].friction_factor = friction_factor * 0.1f;
            }
        }


    }


    void Cloth::GetPosition(unsigned int posx, unsigned int posy, float& x, float& y, float& z) {

        x = m_ropes[posy][posx].position.x;
        y = m_ropes[posy][posx].position.y;
        z = m_ropes[posy][posx].position.z;
    }

    void Cloth::SetPosition(unsigned int posx, unsigned int posy, float x, float y, float z) {
        m_ropes[posy][posx].position.x = x;
        m_ropes[posy][posx].position.y = y;
        m_ropes[posy][posx].position.z = z;
    }

    void Cloth::SetFixed(unsigned int x, unsigned int y, bool fixed) {
        m_ropes[y][x].fixed = fixed;
    }

    /*
    void Cloth::RestrictHorizontalDistances(float dt) {
        for (unsigned int y = 0; y < m_rows; y++) {
            for (int x = 1; x < m_num_particles_per_rope; x++) {
                Point* previous = &m_ropes[y][x - 1];
                Point* current = &m_ropes[y][x];
                UpdateJaksobenParticlesPair(*this, previous, current);
            }
        }
    }

    void Cloth::RestrictVerticalDistances(float dt) {
        for (unsigned int y = 1; y < m_rows; y++) {
            for (int x = 0; x < m_num_particles_per_rope; x++) {
                Point* previous = &m_ropes[y - 1][x];
                Point* current = &m_ropes[y][x];
                UpdateJaksobenParticlesPair(*this, previous, current);
            }
        }
    }
    */

    void Cloth::Update(float dt/*, JobSystem& js*/) {

        //printf("UPDATE DT: %f\n", dt);
       

       for (unsigned int y = 0; y < m_rows; y++) {
            for (int x = 0; x < m_num_particles_per_rope; x++) {
                if (!m_ropes[y][x].fixed) {
                    Vec3 current_position = m_ropes[y][x].position;

                    // Apply velocity and check collision
                    Vec3 velocity = (m_ropes[y][x].position - m_ropes[y][x].previous_position) / dt;

                    // Apply gravity
                    velocity.y += m_gravity * dt;

                    // Apply friction to velocity
                    velocity *= (1.0f - m_ropes[y][x].friction_factor);

                    // Update position using Verlet integration
                    m_ropes[y][x].position += velocity * dt;

                    // Update previous position for the next frame
                    m_ropes[y][x].previous_position = current_position;
                }
            }

        }
        
        //float custom_dt = dt / m_jakobsenIterations;

        //unsigned int custom_jaksoben_iterations= (int)(m_jakobsenIterations * (dt / m_fixed_time_step));
        //custom_jaksoben_iterations = std::clamp(custom_jaksoben_iterations, m_min_jakobsenIterations, m_max_jakobsenIterations);
        
        //float timestep_factor = std::clamp(dt / m_fixed_time_step, 0.8f, 1.2f);
        //int custom_jaksoben_iterations = (int)(m_jakobsenIterations * timestep_factor);
        //printf("ITERATIONS: %d\n", custom_jaksoben_iterations);
        // Jakobsen Horizontal



        //float accumulator = dt;
        //printf("Accumulator %f/%f\n", accumulator, m_fixed_time_step);
        //static int frame = 0;
        //while (accumulator < m_fixed_time_step) {
            //printf("%d Accumulator %f/%f ----- dt: %f\n", frame, accumulator, m_fixed_time_step, dt);

            //float custom_dt = m_fixed_time_step / m_jakobsenIterations;


            for (int iterations = 0; iterations < m_jakobsenIterations; iterations++) {

                // Previous logic for explain in video (less fps)
                /*
                for (int y = 0; y < m_rows; y++) {
                    for (int x = 0; x < m_num_particles_per_rope; x++) {

                        if (x > 0) {
                            Point* previous = &m_ropes[y][x - 1];
                            Point* current = &m_ropes[y][x];
                            UpdateJaksobenParticlesPair(*this, previous, current, dt);
                        }

                        if (y > 0) {

                            Point* previous = &m_ropes[y - 1][x];
                            Point* current = &m_ropes[y][x];
                            UpdateJaksobenParticlesPair(*this, previous, current, dt);
                        }
                    }
                }


                /*
                js.add_task(std::bind(&Cloth::RestrictHorizontalDistances, this, dt));
                js.add_task(std::bind(&Cloth::RestrictVerticalDistances, this, dt));

                js.wait_until_finish();
                */


                // Restringir distancias horizontales (entre puntos en una misma fila)

                for (unsigned int y = 0; y < m_rows; y++) {
                    for (int x = 1; x < m_num_particles_per_rope; x++) {
                        Point* previous = &m_ropes[y][x - 1];
                        Point* current = &m_ropes[y][x];
                        UpdateJaksobenParticlesPair(*this, previous, current);
                    }
                }

                // Restringir distancias verticales (entre puntos en diferentes filas)
                for (unsigned int y = 1; y < m_rows; y++) {
                    for (int x = 0; x < m_num_particles_per_rope; x++) {
                        Point* previous = &m_ropes[y - 1][x];
                        Point* current = &m_ropes[y][x];
                        UpdateJaksobenParticlesPair(*this, previous, current);
                    }
                }

            }

            //accumulator += dt;
        //}
        //frame++;
    }

};