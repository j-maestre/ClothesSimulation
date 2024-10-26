#include "cloth.h"
#include <raylib.h>

namespace JE {

	class ClotheRaylib : public Cloth {
		public:
			ClotheRaylib(unsigned int rows, unsigned int columns, float length_x, float lenght_y);
			ClotheRaylib(const ClotheRaylib&);
			ClotheRaylib(ClotheRaylib&&);
			~ClotheRaylib();

			void SetColor(Color color);
			virtual void DrawClothe();

		private:
			Color m_color = GREEN;
	};

}