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
	};

	public:

		Rope(const Rope&);
		Rope(Rope&&);
		~Rope();

		// Called every frame
		void Update(float dt);

		
		void InitRope();

		// Return position of the given particle
		void GetPosition(int index, float& x, float& y, float& z);


		void SetStepSize(float step);
		void SetMass(float mass);

		// You must call constructor and InitRope() in this func
		virtual void DrawRope() = 0;

protected:

		Rope(float lenght, unsigned int particles);

	private:

		// There will be a lot of more varables, maybe a class representing all of the variables or a structure
		float m_mass = 0.1f;
		float m_stepSize = 0.1f;

		Point* m_points;

public:
		float m_lenght;
		unsigned int m_numParticles;


};