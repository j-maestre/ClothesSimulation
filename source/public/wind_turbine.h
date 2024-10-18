#pragma once
#include "rope.h"

namespace JE {

	class WindTurbine {

	public:
		WindTurbine(Vec3 position, Vec3 direction, float strength, float spread_angle, float max_distance, bool enabled = true);
		WindTurbine(const WindTurbine&);
		WindTurbine(WindTurbine&&);
		~WindTurbine();


		Vec3 m_position;
		Vec3 m_direction;
		float m_strength;
		float m_spreadAngle;
		float m_maxDistance;
		bool m_enabled = true;

	private:


	};
}