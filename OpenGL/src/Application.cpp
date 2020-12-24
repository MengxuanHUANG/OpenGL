#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 960, "OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	float square[] = {
		100.0f, 100.0f, 0.0f, 0.0f, 1.0f, //0
		200.0f, 100.0f, 0.0f, 1.0f, 1.0f, //1
		200.0f, 200.0f, 0.0f, 1.0f, 0.0f, //2
		100.0f, 200.0f, 0.0f, 0.0f, 0.0f //3
	};
	unsigned int squareIndices[] = { 1, 0, 3, 3, 2, 1 };

	float triangle[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,//0
		 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,//1
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f //2
	};
	unsigned int indices[] = { 0, 1, 2 };
	//set up vao (2 ways of using vao)
	// 1. each object has a vao
	// 2. all bojects shared the same vao
	//unsigned int vao;
	//GLCall(glGenVertexArrays(1, &vao));
	//GLCall(glBindVertexArray(vao));

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	{
		//set vertex info(position, normal, texture, etc.)
		VertexBuffer vb(square, sizeof(square));

		//set Vertex Array(tell GPU how to read data)
		VertexArray va;
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		//set indices(indicate order of vertexes)
		IndexBuffer ib(squareIndices, 6);

		//model, view, projection(MVP) matrix
		glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 960.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

		//projection * view * model
		glm::mat4 mvp =  proj * view * model;

		//shaders
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		Texture texture("res/texture/ChernoLogo.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);
		shader.SetUniformMat4f("u_MVP", mvp);

		//texture.UnBind();
		va.UnBind();
		shader.UnBind();
		vb.UnBind();
		ib.UnBind();

		Renderer renderer;

		float r = 0.8f;
		float speed = 0.01f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

			renderer.Draw(va, ib, shader);

			r += speed;
			if (r > 0.99f || r < 0)
			{
				speed = -speed;
			}
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}