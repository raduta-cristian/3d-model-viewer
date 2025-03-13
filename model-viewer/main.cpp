#include "GUI.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <QtWidgets/QApplication>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define WINDOW_W 1440
#define WINDOW_H 810


Camera camera;


unsigned int renderMode = 1;

float lastX = WINDOW_W / 2;
float lastY = WINDOW_H / 2;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);


	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		renderMode = 1;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		renderMode = 2;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		renderMode = 3;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



GLFWwindow* setupWindow()
{
	// init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_W, WINDOW_H, "3D Model Viewer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);


	// init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	// window size setup
	glViewport(0, 0, WINDOW_W, WINDOW_H);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	return window;
}

unsigned int setupLightVAO() {
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	return VAO;
}

int main_viewer(string path)
{
	GLFWwindow* window = setupWindow();
	if (window == NULL)
		return -1;

	glEnable(GL_DEPTH_TEST);

	Shader phongShader("vertex.glsl", "fragment_phong.glsl");
	Shader pureColorShader("vertex.glsl", "light_frag.glsl");
	Shader litShader("vertex.glsl", "fragment_lit.glsl");

	Model mainModel(path);

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 10.0f),
		glm::vec3(5.3f, -3.3f, 5.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(5.0f, 0.0f, -3.0f)
	};

	unsigned int lightBoxVAO = setupLightVAO();

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_W / (float)WINDOW_H, 0.1f, 100.0f);
		if (renderMode == 1) {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			phongShader.use();

			glm::mat4 model = glm::mat4(1.0f);
			phongShader.setMat4("model", model);

			phongShader.setMat4("view", view);

			phongShader.setMat4("projection", projection);

			

			phongShader.setFloat("material.shininess", 32.0f);

			phongShader.setVec3("dirLight.direction", glm::vec3(0.2f, -1.0f, -0.3f));
			
			phongShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			phongShader.setVec3("dirLight.diffuse", glm::vec3(0.7f));
			phongShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

			for (int i = 0; i < 4; i++) {
				//point lights
				//position
				std::string lightStr = "pointLights[" + std::to_string(i) + "].";
				phongShader.setVec3(lightStr + "position", pointLightPositions[i]);
				//colors
				phongShader.setVec3(lightStr + "ambient", glm::vec4(0.0));
				phongShader.setVec3(lightStr + "diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
				phongShader.setVec3(lightStr + "specular", glm::vec3(0.5f, 0.5f, 0.5f));
				//attenuation
				phongShader.setFloat(lightStr + "constant", 1.0f);
				phongShader.setFloat(lightStr + "linear", 0.001f);
				phongShader.setFloat(lightStr + "quadratic", 0.002f);
			}

			phongShader.setVec3("viewPos", camera.Position);
			
			mainModel.Draw(phongShader);
		}
		else if(renderMode == 2) {
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glm::mat4 model = glm::mat4(1.0f);
			litShader.use();
			litShader.setMat4("model", model);

			litShader.setMat4("view", view);

			litShader.setMat4("projection", projection);

			mainModel.Draw(litShader);
		}
		else {
			// draw wireframe
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glm::mat4 model = glm::mat4(1.0f);
			litShader.use();
			litShader.setMat4("model", model);

			litShader.setMat4("view", view);

			litShader.setMat4("projection", projection);

			mainModel.Draw(litShader, true);
		}



		// make point light positions visible
		/*pureColorShader.use();

		pureColorShader.setMat4("view", view);
		pureColorShader.setMat4("projection", projection);
		pureColorShader.setVec4("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		glm::mat4 model;
		for (int i = 0; i < 4; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.5f));
			pureColorShader.setMat4("model", model);
			glBindVertexArray(lightBoxVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}*/


		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

int main(int argc, char *argv[])
{
	while(true) {
		QApplication a(argc, argv);
		GUI gui;
		gui.show();
		a.exec();
		if (gui.toClose)
			break;

		string path = gui.getFilePath();
		if (path.size() > 0) {
			renderMode = 1;
			firstMouse = true;
			camera = Camera(glm::vec3(0.5f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -10.0f);
			main_viewer(path);
		}
		
	}
    return 0;
}
