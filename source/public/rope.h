#pragma once


class Rope {

	public:

		// Lenght o the rope, number of force points
		// Maybe memory allocation fails
		Rope* CreateRope(float lenght, unsigned int points);
		Rope(const Rope&);
		Rope(Rope&&);
		~Rope();


	private:
		Rope();

		// There will be a lot of more varables, maybe a class representing all of the variables or a structure
		float mass = 0.1f;




};