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
#include <render.hpp>


#include <iostream>
#include <string>
#include <limits>

#include <math.h>      


# define M_PI           3.14159265358979323846  /* pi */

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void print_screen();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);
void set_lighting(Shader shader, glm::vec3* pointLightPositions);

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 960;

// tif export related
const unsigned int HEADER_SIZE = 8;
const unsigned int STRIP_SIZE = SCR_WIDTH * SCR_HEIGHT * 3;
const unsigned int IFD_SIZE = 174;
const unsigned int IFD_OFFS = STRIP_SIZE + HEADER_SIZE;
const unsigned int EXTRA_OFFS = IFD_OFFS + IFD_SIZE;
const unsigned int EXTRA_0 = EXTRA_OFFS;
const unsigned int EXTRA_1 = EXTRA_OFFS + 6;
const unsigned int EXTRA_2 = EXTRA_OFFS + 12;
const unsigned int EXTRA_3 = EXTRA_OFFS + 18;

const char TIF_HEADER[] = { 'M', 'M', 0x00, 0x2a, IFD_OFFS >> 24, IFD_OFFS >> 16 & 0xff, IFD_OFFS >> 8 & 0xff, IFD_OFFS & 0xff };
const char TIF_FOOTER[] = {
	// IFD begin
	0x00, 0x0e,

	// IFD entries
	0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, SCR_WIDTH >> 24, SCR_WIDTH >> 16 & 0xff, SCR_WIDTH >> 8 & 0xff, SCR_WIDTH & 0xff,
	0x01, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, SCR_HEIGHT >> 24, SCR_HEIGHT >> 16 & 0xff, SCR_HEIGHT >> 8 & 0xff, SCR_HEIGHT & 0xff,
	0x01, 0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, EXTRA_0 >> 24, EXTRA_0 >> 16 & 0xff, EXTRA_0 >> 8 & 0xff, EXTRA_0 & 0xff,
	0x01, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00,
	0x01, 0x06, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00,
	0x01, 0x11, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08,
	0x01, 0x12, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00,
	0x01, 0x15, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00,
	0x01, 0x16, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, SCR_HEIGHT >> 24, SCR_HEIGHT >> 16 & 0xff, SCR_HEIGHT >> 8 & 0xff, SCR_HEIGHT & 0xff,
	0x01, 0x17, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, STRIP_SIZE >> 24, STRIP_SIZE >> 16 & 0xff, STRIP_SIZE >> 8 & 0xff, STRIP_SIZE & 0xff,
	0x01, 0x18, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, EXTRA_1 >> 24, EXTRA_1 >> 16 & 0xff, EXTRA_1 >> 8 & 0xff, EXTRA_1 & 0xff,
	0x01, 0x19, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, EXTRA_2 >> 24, EXTRA_2 >> 16 & 0xff, EXTRA_2 >> 8 & 0xff, EXTRA_2 & 0xff,
	0x01, 0x1c, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00,
	0x01, 0x53, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, EXTRA_3 >> 24, EXTRA_3 >> 16 & 0xff, EXTRA_3 >> 8 & 0xff, EXTRA_3 & 0xff,
	
	//IFD end 
	0x00, 0x00, 0x00, 0x00,
	
	// extra
	0x00, 0x08, 0x00, 0x08, 0x00, 0x08,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xff, 0x00, 0xff, 0x00, 0xff,
	0x00, 0x01, 0x00, 0x01, 0x00, 0x01
};

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

// Save Frame to File
unsigned int printID = 1;
bool print = false;

// Animate and save each frame
bool animate = false;

int threshold = 1;
bool updateGeom = false;

struct Texture {
	unsigned int id;
	std::string type;
	aiString path;
};


#endif //~ MarchingCubes Header
