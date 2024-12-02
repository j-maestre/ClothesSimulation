#include "cloth.h"
#include <raylib.h>
#include <string>
#include <GL/glew.h>

namespace JE {

	class ClotheRaylib : public Cloth {
		public:
			ClotheRaylib(unsigned int rows, unsigned int columns, float length_x, float lenght_y);
			ClotheRaylib(const ClotheRaylib&);
			ClotheRaylib(ClotheRaylib&&);
			~ClotheRaylib();

			virtual void InitClothe(Vec3 first_pos, float mass, float friction_factor) override;

			void SetColor(Color color);
			void DrawWireFrame();
			virtual void DrawClothe();

			bool SetTexture(const char* path);
			bool SetShader(const char* fragment, const char* vertex);

			void SetCamera(Camera& cam) { m_cam = cam; };
		private:
			Color m_color = GREEN;

			std::string m_vertex_shader;
			std::string m_fragment_shader;

			Mesh m_sphere_mesh;
			Material m_mat_default;

			Mesh m_mesh;
			Model m_model;
			Texture2D m_tex;
			Shader m_shader;

			std::vector<Matrix> m_sphere_matrices;

			std::vector<float> position;
			std::vector<float> normal;
			std::vector<float> uv;
			std::vector<unsigned short> indices;
			Camera m_cam;
	};



}