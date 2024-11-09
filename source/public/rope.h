#pragma once
#include "JEMath.h"
#include "sphere_collision.h"
#include "collision_manager.h"


namespace JE {

class WindTurbine;


	

	class Rope {

		enum class RopeErrorCode {
			OK = 0,
			NoMemory = 1,
			BadSize = 2,
			UnknowError = 3,
		};

	public:

		Rope(const Rope&);
		Rope(Rope&&);
		~Rope();

		// First point position and second point position
		void InitRope(Vec3 first_pos, Vec3 direction, float mass = 1.0f, float friction_factor = 1.0f, bool enable_collision = false, CollisionManager* cm = nullptr);

		// Called every frame
		void Update(float dt);

		void ApplyWindTurbine(WindTurbine w, float dt);

		// Return position of the given particle
		void GetPosition(int index, float& x, float& y, float& z);
		void SetPointPosition(int index, float x, float y, float z);
		void TranslateRope(float x, float y, float z);

		void SetFixed(unsigned int index, bool fixed = true);
		
		void SetFriction(int index, float friction);
		void SetAllFriction(float friction);
		void SetMass(int index, float mass);
		void SetAllMass(float mass);

		void SetStepSize(float step);

		virtual void DrawRope() = 0;

	protected:

		Rope(float lenght, unsigned int particles);

	private:

		float m_timeStep = 0.01f;
		unsigned int m_jakobsenIterations = 50;
		float m_gravity = -9.81f;
		float m_desiredDistance = 0.0f;

		bool m_enabled_collision = false;

		Point* m_points;
		CollisionManager* m_cm;

	public:
		float m_lenght;
		unsigned int m_numParticles;


	};
};