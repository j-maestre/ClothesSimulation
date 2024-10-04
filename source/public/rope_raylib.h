#pragma once
#include "rope.h"
#include <raylib.h>
#include "raymath.h"


class RopeRaylib : public Rope {

	public:

		// Rope lenght and number of particles
		RopeRaylib(float lenght, unsigned int particles);
		RopeRaylib(const RopeRaylib&) = delete;
		RopeRaylib(RopeRaylib&&) = delete;
		~RopeRaylib();
		virtual void DrawRope();
		void SetColor(Color color = GREEN);
	
	private:

		Color m_color;

};