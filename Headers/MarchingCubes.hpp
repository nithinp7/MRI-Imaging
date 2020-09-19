// Preprocessor Directives
#ifndef MarchingCubes
#define MarchingCubes
#pragma once

// System Headers
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>



#include <glad/glad.h>

//#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>

#include <shader.hpp>
#include <camera.hpp>
#include <heightmap.hpp>


#include <iostream>
#include <string>
#include <limits>

#include <math.h>      


# define M_PI           3.14159265358979323846  /* pi */

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);
void set_lighting(Shader shader, glm::vec3* pointLightPositions);

// settings
unsigned int SCR_WIDTH = 960;
unsigned int SCR_HEIGHT = 960;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float framerate = 0.0f;

// booleans for doing different things
bool drawHeightmap = true;
bool drawBoxes = true;
bool drawNormals = false;
bool quaterians = false;

// Transformation Matrices
glm::vec3 translation   = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 rotation_rate = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 rotation_euler      = glm::vec3(0.0f, 0.0f, 0.0f);
glm::quat rotation   =   glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
glm::vec3 scale         = glm::vec3(1.0f, 1.0f, 1.0f);

// Step size of transformations
float step_multiplier = 0.1f;

// Last Press
float last_pressed = 0.0f;

struct Texture {
	unsigned int id;
	std::string type;
	aiString path;
};


#endif //~ MarchingCubes Header
