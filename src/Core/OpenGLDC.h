#include "glm/glm.hpp" 
#include "OrthoCamera.h" 
#include "../Shader/ShaderManager.h"
#include "../Shader/Shader_P3_C3.h"
#include <map>
#include <string>
#include <vector>

typedef struct Vertex
{
	float x, y, z;
	float r, g, b;

} Vertex;

class OpenGLDC
{
public:
	OpenGLDC(OrthoCamera& camera)   // ← 引用
		: m_camera(camera) {
	}
	~OpenGLDC() {};

public:
	void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color)
	{
		auto lineShader = std::dynamic_pointer_cast<Shader_P3_C3>(m_shaderMgr.GetShader("Shader_P3_C3"));

		lineShader->Begin();

		std::vector<Vertex> m_vertices;
		m_vertices.push_back({ start.x, start.y, start.z, color.r, color.g, color.b });
		m_vertices.push_back({ end.x, end.y, end.z, color.r, color.g, color.b });
		 
		glBindVertexArray(m_vaoMgr["lineBuffer"]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboMgr["lineBuffer"]);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_DYNAMIC_DRAW); 	// 传递数据， 
		glUniformMatrix4fv(lineShader->m_mvp, 1, GL_FALSE, glm::value_ptr(m_camera.GetViewProjectionMatrix()));
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_vertices.size()));
		lineShader->End();

	};

	void DrawCircle(const glm::vec3& center, float radius, const glm::vec3& color)
	{
		   
	};

public :
	void Initialize()
	{   
		auto shader = std::make_shared<Shader_P3_C3>();
		shader->Initialize();
		m_shaderMgr.AddShader("Shader_P3_C3", shader);

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW); // 先分配空间
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_vboMgr["lineBuffer"] = vbo;
		 
		// Shader应用绑定顶点缓冲区数据
		GLuint vao;
		glGenVertexArrays(1, &vao);			
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);  // 公用 VBO
		glVertexAttribPointer(shader->m_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(shader->m_position);
		glVertexAttribPointer(shader->m_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(shader->m_color); 		
		glBindVertexArray(0);	 

	
		m_vaoMgr["lineBuffer"] = vao;

	}
private:
	OrthoCamera& m_camera;
	ShaderManager m_shaderMgr;
	std::map<std::string, GLuint> m_vaoMgr;
	std::map<std::string, GLuint> m_vboMgr;

};
