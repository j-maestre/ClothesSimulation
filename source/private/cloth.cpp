#include "public/cloth.h"
#include "assert.h"

namespace JE{

    Cloth::Cloth(float lenght_per_rope, unsigned int particles_per_rope) : m_length_per_rope(lenght_per_rope), m_num_particles_per_rope(particles_per_rope) {

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

    // Quitar direction, hacer un cuadrado a pelo y au
    void Cloth::InitClothe(unsigned int rows, float distance_between_points, Vec3 first_pos, float mass, float friction_factor){

        m_distance_between_rows = distance_between_points; // Usar la misma distancia entre filas y columnas
        m_rows = rows;
        m_ropes.resize(rows);

        for (int row = 0; row < rows; row++) {
            // Crear una nueva fila de puntos
            m_ropes[row] = new Point[m_num_particles_per_rope];
            assert(m_ropes[row] && "New Points is null");

            // Posición inicial de la fila en la coordenada Y
            Vec3 current_row_pos = first_pos;
            current_row_pos.y += row * distance_between_points; // Incrementar la Y según la fila actual

            for (int i = 0; i < m_num_particles_per_rope; i++) {
                // Incrementar la X para crear los puntos de la fila, manteniendo la misma distancia
                float x = first_pos.x + i * distance_between_points;
                float y = current_row_pos.y; // La Y es fija para esta fila
                float z = first_pos.z; // Mantener Z constante (o ajustarlo según sea necesario)

                // Asignar las posiciones a cada punto
                m_ropes[row][i].position.x = x;
                m_ropes[row][i].position.y = y;
                m_ropes[row][i].position.z = z;

                m_ropes[row][i].previous_position.x = x;
                m_ropes[row][i].previous_position.y = y;
                m_ropes[row][i].previous_position.z = z;

                // Asignar otras propiedades del punto
                m_ropes[row][i].mass = mass;
                m_ropes[row][i].friction_factor = friction_factor * 0.1f;
                m_ropes[row][i].fixed = (row == 0 && i == 0); // Solo fijamos el primer punto del primer row
            }
        }

        // Calcular la distancia deseada entre puntos en la cuerda
        m_desired_distance_per_rope = distance_between_points;
    }

    void Cloth::GetPosition(unsigned int row, unsigned int index, float& x, float& y, float& z){

        x = m_ropes[row][index].position.x;
        y = m_ropes[row][index].position.y;
        z = m_ropes[row][index].position.z;
    }

};