#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#include "Shader_format.h"

int wWidth = 640, wHeight = 480;
const char* title = "OpenGL Tests";


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main(void) {

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, title, NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//built in method to set what method is called when a key is pressed
	glfwSetKeyCallback(window, key_callback);

	//setting context and GL settings
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	Shader ourShader("shader.vs", "shader.fs");

	float vertices[] = {
		//positions			// colors
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //bottom left
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //bottom right
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f //top right
	};


	//setting up vertex data buffers, Vertex Buffer Object
	//Vertex Array Object
	//Element Buffer Object
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//bind buffer and send buffer data to bound buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//STREAM DRAW is set once, used few times
	//STATIC DRAW is set once, used many times
	//DYNAMIC DRAW is changed multiple times, used many times

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		//input would be here, but glfwSetKeyCallback was used, so no calling 
		//created method

		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Example changing color value based on time
		float timeValue = glfwGetTime();
		float xOffset = (sin(timeValue) / 2.0f);
		float yOffset = (cos(timeValue) / 2.0f);

		ourShader.use();
		//If a uniform is used
		//ourShader.setFloat("someUniform", 1.0f);
		ourShader.setFloat("xOffset", xOffset);
		ourShader.setFloat("yOffset", yOffset);
		
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Example that uses mutiple points in the vertices array
		// and multiple shaders
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glUseProgram(shaderProgram2);
		//glDrawArrays(GL_TRIANGLES, 3, 3);

		//glBindVertexArray(0);


		//check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}