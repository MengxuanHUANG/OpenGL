#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"


namespace test {
	TestTexture2D::TestTexture2D()
		:m_Proj(glm::ortho(0.0f, 1280.0f, 0.0f, 960.0f, -1.0f, 1.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Translation_A(0, 200, 0), m_Translation_B(600, 200, 0)
	{
		float square[] = {
			0.0f,   500.0f, 0.0f, 0.0f, 1.0f, //0
			500.0f, 500.0f, 0.0f, 1.0f, 1.0f, //1
			500.0f, 0.0f,   0.0f, 1.0f, 0.0f, //2
			0.0f,   0.0f,   0.0f, 0.0f, 0.0f  //3
		};
		unsigned int squareIndices[] = { 1, 0, 3, 3, 2, 1 };

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_shared<VertexArray>();

		m_VB = std::make_shared<VertexBuffer>(square, sizeof(square));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VB, layout);

		m_IB = std::make_shared<IndexBuffer>(squareIndices, 6);

		m_Shader = std::make_shared<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();

		m_Texture = std::make_shared<Texture>("res/texture/texture_test.png");
		m_Texture->Bind();
		m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		m_Shader->SetUniform1i("u_Texture", 0);
	}
	TestTexture2D::~TestTexture2D()
	{
	}
	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}
	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Shader->Bind();

		//draw first image
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation_A);
			//projection * view * model
			glm::mat4 mvp = m_Proj * m_View * model;

			glm::vec4 temp(-50.0f, -50.0f, 0.0f, 1.0f);
			glm::vec4 result = temp * mvp;

			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IB, *m_Shader);
		}

		//draw second image
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation_B);
			//projection * view * model
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IB, *m_Shader);
		}
	}
	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation_A", &m_Translation_A.x, 0.0f, 500.0f);
		ImGui::SliderFloat3("Translation_B", &m_Translation_B.x, 0.0f, 500.0f);
	}
}