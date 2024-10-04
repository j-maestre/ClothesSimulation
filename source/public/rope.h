#pragma once


class Rope {

	enum class RopeErrorCode {
		OK = 0,
		NoMemory = 1,
		BadSize = 2,
		UnknowError = 3,
	};

	struct Point {
		float position[3] = { 0.0f };
		float previous_position[3] = { 0.0f };
		bool fixed = false;
	};

	public:

		Rope(const Rope&);
		Rope(Rope&&);
		~Rope();

		// Called every frame
		void Update(float dt);

		// First point position and second point position
		void InitRope(float x1, float y1, float z1, float x2, float y2, float z2);

		// Return position of the given particle
		void GetPosition(int index, float& x, float& y, float& z);

		void SetPointPosition(int index, float x, float y, float z);


		void SetStepSize(float step);
		void SetMass(float mass);

		// You must call constructor and InitRope() in this func
		virtual void DrawRope() = 0;

protected:

		Rope(float lenght, unsigned int particles);

	private:

		// There will be a lot of more varables, maybe a class representing all of the variables or a structure
		float m_mass = 0.1f;
		
		float m_timeStep = 0.01f;
		unsigned int jm_jakobsenIterations = 50;
		float m_gravity = -9.81f;
		float m_desiredDistance = 0.0f;

		Point* m_points;

public:
		float m_lenght;
		unsigned int m_numParticles;


};