#pragma once
#include "rope.h"
#include "vector"

namespace JE {


	class Cloth {
	public:

		// Makes a square
		Cloth(const Cloth&);
		Cloth(Cloth&&);
		~Cloth();

		void InitClothe(unsigned int rows, float distance_between_points ,Vec3 first_pos, float mass = 1.0f, float friction_factor = 1.0f);

		void GetPosition(unsigned int row, unsigned int index, float& x, float& y, float& z);

		virtual void DrawClothe() = 0;

	protected:
		Cloth(float lenght_per_rope, unsigned int particles_per_rope);

		float m_length_per_rope;
		unsigned int m_num_particles_per_rope;
		float m_desired_distance_per_rope;
		float m_distance_between_rows;

		float m_rows;

		unsigned int m_width;
		unsigned int m_height;

		std::vector<Point*> m_ropes;
	private:

	};
};