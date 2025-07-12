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
float opacity = 0.2f, viewDepth = -7.0f, horizontalAngle = 0.0f, verticalAngle = 0.0f;
float rotationDegrees = 10.0f;
const char* title = "OpenGL Tests";
glm::mat4 view;

static void adjustOpacity(float adjust) {
	if (opacity > -0.2f && adjust < 0) {
		opacity += adjust;
	}
	if (opacity < 1.0f && adjust > 0) {
		opacity += adjust;
	}
}

/*		Y
	X---|/
	   /|
	  Z
	 vec3(x, y, z)
	 */

// glm::rotate vector directs it to rotate around an axis, vec3(0, 1, 0) would rotate around the y axis on the x/z plane

static void rotateAroundOrigin(float horizontalAngleChange) {
	horizontalAngle += horizontalAngleChange;
	if (horizontalAngle >= 360) horizontalAngle -= 360;
	if (horizontalAngle < 0) horizontalAngle += 360;

	float xPos = sin(glm::radians(horizontalAngle)) * viewDepth;
	float zPos = cos(glm::radians(horizontalAngle)) * viewDepth;
	float angleToOrigin = -atan2(-xPos, -zPos);
	view = glm::mat4(1.0f);
	view = glm::rotate(view, angleToOrigin, glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(xPos, 0.0f, zPos));
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
		adjustOpacity(0.1f);
	}
	if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
		adjustOpacity(-0.1f);
	}
	if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) && action == GLFW_PRESS) {
		//view = glm::translate(view, glm::vec3(0.0f, -0.1f, 0.0f));
		//view = glm::rotate(view, glm::radians(10.0f), glm::vec3(0.5f, 0.0f, 0.0f));
	}
	if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && action == GLFW_PRESS) {

		//view = glm::translate(view, glm::vec3(0.0f, 0.1f, 0.0f));
	}
	if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action == GLFW_PRESS) {
		rotateAroundOrigin(10);

	}
	if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action == GLFW_PRESS) {
		rotateAroundOrigin(-10);
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

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("shader.vs", "shader.fs");



	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};



	//int indices[] = {
	//	0, 1, 3,
	//	1, 2, 3
	//};


	//setting up vertex data buffers, Vertex Buffer Object
	//Vertex Array Object
	//Element Buffer Object
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

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
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//texture attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	int imgWidth, imgHeight, nrChannels;
	unsigned char* imgData = stbi_load("image-resources/container.jpg", &imgWidth, &imgHeight, &nrChannels, 0);

	unsigned int textureId1, textureId2;
	glGenTextures(1, &textureId1);
	glBindTexture(GL_TEXTURE_2D, textureId1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	//Translate tutorial
	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	//glm::mat4 model = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, viewDepth));
	glm::mat4 projection = glm::mat4(1.0f);

	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	projection = glm::perspective(glm::radians(45.0f), ((float)wWidth / (float)wHeight), 0.1f, 100.0f);


	unsigned int modelLocation = glGetUniformLocation(ourShader.ID, "model");
	unsigned int viewLocation = glGetUniformLocation(ourShader.ID, "view");
	unsigned int projectionLocation = glGetUniformLocation(ourShader.ID, "projection");

	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));


	float xPos = sin(glm::radians(horizontalAngle)) * abs(viewDepth);
	float zPos = cos(glm::radians(horizontalAngle)) * abs(viewDepth);
	float angleToOrigin = -atan2(-xPos, -zPos);
	std::cout << xPos << " : " << zPos << " : " << angleToOrigin << std::endl;



	while (!glfwWindowShouldClose(window)) {
		//input would be here, but glfwSetKeyCallback was used, so no calling 
		//created method

		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glm::mat4 trans = glm::mat4(1.0f);
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));

		//model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureId2);
		//view = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
		//view = glm::rotate(view, (float)glfwGetTime(), glm::vec3(0.1f, 0.1f, 0.0f));
		ourShader.setMat4("view", view);

		ourShader.use();
		//If a uniform is used
		//ourShader.setFloat("someUniform", 1.0f);
		ourShader.setFloat("opacity", opacity);
		//ourShader.setMat4("model", model);

		
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime() + glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		//check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}