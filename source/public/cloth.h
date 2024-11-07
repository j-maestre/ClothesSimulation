#pragma once
#include "JEMath.h"
#include "vector"

namespace JE {


	class Cloth {
	public:

		// Makes a square
		Cloth(const Cloth&);
		Cloth(Cloth&&);
		~Cloth();

		void InitClothe(Vec3 first_pos, float mass, float friction_factor);
		
		void GetPosition(unsigned int row, unsigned int index, float& x, float& y, float& z);
		void SetPosition(unsigned int row, unsigned int index, float x, float y, float z);
		void SetFixed(unsigned int x, unsigned int y, bool fixed = true);


		void Update(float dt);
		virtual void DrawClothe() = 0;

	protected:
		Cloth(unsigned int rows, unsigned int columns, float length_x, float length_y);

		float m_length_per_rope;
		unsigned int m_num_particles_per_rope;
		float m_desired_distance_per_rope;
		float m_distance_between_rows;

		unsigned int m_rows;
		unsigned int m_columns;


		unsigned int m_width;
		unsigned int m_height;

		float m_timeStep = 0.01f;
		unsigned int m_jakobsenIterations = 50;
		float m_gravity = -9.81f;

		std::vector<Point*> m_ropes;
	private:

		static inline void UpdateJaksobenParticlesPair(Cloth& c, Point* first, Point* second, float dt);
	};
};