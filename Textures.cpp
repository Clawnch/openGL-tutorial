#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader_format.h"

int wWidth = 800, wHeight = 600;
float opacity = 0.2f;
const char* title = "OpenGL Tests";

static void adjustOpacity(float adjust) {
	if (opacity > 0.0f && adjust < 0) {
		opacity += adjust;
	}
	if (opacity < 1.0f && adjust > 0) {
		opacity += adjust;
	}
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		adjustOpacity(0.1f);
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		adjustOpacity(-0.1f);
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
		//positions			// colors			// texture coords
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	//top right
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	//bottom right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	//bottom left
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// top left
	};

	int indices[] = {
		0, 1, 3,
		1, 2, 3
	};


	//setting up vertex data buffers, Vertex Buffer Object
	//Vertex Array Object
	//Element Buffer Object
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//bind buffer and send buffer data to bound buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//STREAM DRAW is set once, used few times
	//STATIC DRAW is set once, used many times
	//DYNAMIC DRAW is changed multiple times, used many times

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//start of texture tutorial

	int imgWidth, imgHeight, nrChannels;
	unsigned char* imgData = stbi_load("image-resources/container.jpg", &imgWidth, &imgHeight, &nrChannels, 0);

	unsigned int textureId1, textureId2;
	glGenTextures(1, &textureId1);
	glBindTexture(GL_TEXTURE_2D, textureId1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (imgData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture 1" << std::endl;
	}
	stbi_image_free(imgData);

	stbi_set_flip_vertically_on_load(true);
	imgData = stbi_load("image-resources/awesomeface.png", &imgWidth, &imgHeight, &nrChannels, 0);

	glGenTextures(1, &textureId2);
	glBindTexture(GL_TEXTURE_2D, textureId2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (imgData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture 2" << std::endl;
	}

	stbi_image_free(imgData);

	ourShader.use();

	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	ourShader.setInt("texture2", 1);

	//Translate tutorial
	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));



	while (!glfwWindowShouldClose(window)) {
		//input would be here, but glfwSetKeyCallback was used, so no calling 
		//created method

		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Example changing color value based on time
		//float timeValue = glfwGetTime();
		//float xOffset = (sin(timeValue) / 2.0f);
		//float yOffset = (cos(timeValue) / 2.0f);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
		
		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureId2);

		ourShader.use();
		//If a uniform is used
		//ourShader.setFloat("someUniform", 1.0f);
		ourShader.setFloat("opacity", opacity);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		float scale = abs(sin(glfwGetTime() + 1));
		trans = glm::scale(trans, glm::vec3(scale, scale, 0.0f));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Example that uses mutiple points in the vertices array
		// and multiple shaders
		//glDrawArrays(GL_TRIANGLES, 0, 3);
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