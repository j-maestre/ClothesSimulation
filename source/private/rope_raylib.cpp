#include "public/rope_raylib.h"

namespace JE {

	RopeRaylib::RopeRaylib(float lenght, unsigned int particles) : Rope(lenght, particles) {

		m_color = GREEN;
	}




	RopeRaylib::~RopeRaylib() {}

	void RopeRaylib::SetColor(Color color) {
		m_color = color;
	}


	void RopeRaylib::DrawRope() {



		float x, y, z;
		for (int i = 0; i < m_numParticles; i++) {

			GetPosition(i, x, y, z);
			DrawSphere(Vector3{ x, y, z }, 0.01f, m_color);

			if (i < m_numParticles - 1) {
				Vector3 startPos = { x,y,z };
				GetPosition(i + 1, x, y, z);
				Vector3 endPos = { x,y,z };
				DrawLine3D(startPos, endPos, m_color);
			}
		}

	}
};
