#pragma once
#include "rope.h"
#include "vector"

namespace JE {


	class Cloth {
	public:

		// Makes a square
		Cloth(unsigned int width, unsigned int heigth, Vec3 pos1, Vec3 pos2);
		Cloth(const Cloth&);
		Cloth(Cloth&&);
		~Cloth();

		void InitClothe();

		virtual void DrawClothe() = 0;
	private:

		unsigned int m_width;
		unsigned int m_height;
		std::vector<Point> m_ropes;

	};
};