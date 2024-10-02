#pragma once
#include "rope.h"

class RopeRaylib : public Rope {

	public:

		RopeRaylib(float lenght, unsigned int particles);
		RopeRaylib(const RopeRaylib&) = delete;
		RopeRaylib(RopeRaylib&&) = delete;
		~RopeRaylib();
		
		virtual void DrawRope();
	
	private:

};