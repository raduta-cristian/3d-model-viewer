
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

#define WINDOW_W 800
#define WINDOW_H 600



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLFWwindow* setupWindow();
unsigned int setupVAO();
unsigned int setupLightVAO();

static class Viewer {



public:
	int main_viewer(string path)
	{
		GLFWwindow* window = setupWindow();
		if (window == NULL)
			return -1;

		glEnable(GL_DEPTH_TEST);

		Shader shader("vertex.glsl", "fragment_phong.glsl");

		Model skull(path);

		glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f, 0.2f, 10.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f, 2.0f, -12.0f),
			glm::vec3(0.0f, 0.0f, -3.0f)
		};

		unsigned int lightBoxVAO = setupLightVAO();
		Shader lightBoxShader("vertex.glsl", "light_frag.glsl");

		// render loop
		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// input
			processInput(window);

			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader.use();

			glm::mat4 model = glm::mat4(1.0f);
			shader.setMat4("model", model);

			glm::mat4 view = camera.GetViewMatrix();
			shader.setMat4("view", view);

			glm::mat4 projection;
			projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_W / (float)WINDOW_H, 0.1f, 100.0f);
			shader.setMat4("projection", projection);

			// material / lighting

			shader.setFloat("material.shininess", 32.0f);

			//directional light
			//direction
			shader.setVec3("dirLight.direction", glm::vec3(0.2f, -1.0f, -0.3f));
			//colors
			shader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			shader.setVec3("dirLight.diffuse", glm::vec3(0.7f));
			shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

			for (int i = 0; i < 4; i++) {
				//point lights
				//position
				std::string lightStr = "pointLights[" + std::to_string(i) + "].";
				shader.setVec3(lightStr + "position", pointLightPositions[i]);
				//colors
				shader.setVec3(lightStr + "ambient", glm::vec4(0.0));
				shader.setVec3(lightStr + "diffuse", glm::vec3(1.0f, 0.5f, 0.5f));
				shader.setVec3(lightStr + "specular", glm::vec3(1.0f, 1.0f, 1.0f));
				//attenuation
				shader.setFloat(lightStr + "constant", 1.0f);
				shader.setFloat(lightStr + "linear", 0.001f);
				shader.setFloat(lightStr + "quadratic", 0.002f);
			}

			shader.setVec3("viewPos", camera.Position);

			skull.Draw(shader);


			// make point light positions visible
			lightBoxShader.use();
			lightBoxShader.setMat4("view", view);
			lightBoxShader.setMat4("projection", projection);
			lightBoxShader.setVec4("lightColor", glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));
			for (int i = 0; i < 4; i++) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, pointLightPositions[i]);
				model = glm::scale(model, glm::vec3(0.5f));
				lightBoxShader.setMat4("model", model);
				glBindVertexArray(lightBoxVAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}


			// check and call events and swap the buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;
	}
};



