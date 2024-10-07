#include "public/cloth.h"

namespace JE{

    Cloth::Cloth(unsigned int width, unsigned int heigth, Vec3 pos1, Vec3 pos2) {

        m_width = width;
        m_height = heigth;

        unsigned int real_size = width * heigth;
        m_ropes.reserve(real_size);
        for (int i = 0; i < real_size; i++) {
            m_ropes.emplace_back();

            // How close are we to the last point?
            float w = (float)i / (width - 1);

            float x = w * pos2.x + (1 - w) * pos1.x;
            float y = w * pos2.y + (1 - w) * pos1.y;
            float z = w * pos2.z + (1 - w) * pos1.z;


            /*m_ropes[i].position[0] = x;
            m_points[i].position[1] = y;
            m_points[i].position[2] = z;

            m_points[i].previous_position[0] = x;
            m_points[i].previous_position[1] = y;
            m_points[i].previous_position[2] = z;

            m_points[i].fixed = i == 0; // We fix only the first point

            unsigned numberOfSegments = m_numParticles - 1;

            float ropeLength = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
            m_desiredDistance = ropeLength / (m_numParticles - 1);*/
        }

    }

    Cloth::Cloth(const Cloth&) {

    }

    Cloth::Cloth(Cloth&&) {

    }

    Cloth::~Cloth() {

    }

    void Cloth::InitClothe() {

    }

};