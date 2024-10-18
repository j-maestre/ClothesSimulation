#include "public/clothe_raylib.h"
#include "public/cloth.h"
#include "raylib.h"

namespace JE {

	ClotheRaylib::ClotheRaylib(float lenght_per_rope, unsigned int particles_per_rope) : Cloth(lenght_per_rope, particles_per_rope){

	}

	ClotheRaylib::~ClotheRaylib()
	{
	}

	void ClotheRaylib::SetColor(Color color){
		m_color = color;
	}

	void ClotheRaylib::DrawClothe(){

		float x, y, z;
		float x2, y2, z2;

		for (unsigned int row = 0; row < m_rows; row++) {
			for (int i = 0; i < m_num_particles_per_rope; i++) {

				GetPosition(row, i, x, y, z);
				DrawSphere(Vector3{ x, y, z }, 0.01f, RED);

				if (i < m_num_particles_per_rope - 1) {
					Vector3 startPos = { x,y,z };
					GetPosition(row, i + 1, x2, y2, z2);
					Vector3 endPos = { x2,y2,z2 };
					DrawLine3D(startPos, endPos, m_color);

				}
				if (row < m_rows - 1) {
					Vector3 startPos = { x,y,z };
					GetPosition(row + 1, i, x2, y2, z2);
					Vector3 endPos = { x2,y2,z2 };
					DrawLine3D(startPos, endPos, m_color);
				}
			}
		}
	}

}