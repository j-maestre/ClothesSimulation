#include "cloth.h"
#include <raylib.h>

namespace JE {

	class ClotheRaylib : public Cloth {
		public:
			ClotheRaylib(float lenght_per_rope, unsigned int particles_per_rope);
			ClotheRaylib(const ClotheRaylib&);
			ClotheRaylib(ClotheRaylib&&);
			~ClotheRaylib();

			void SetColor(Color color);
			virtual void DrawClothe();

		private:
			Color m_color = GREEN;
	};

}