#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include <vector>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Windows Dimensions
extern const GLint WIDTH, HEIGHT;

extern GLuint VAO, VBO, programID;

extern GLuint projectionviewPos;
extern glm::mat4 projection;

extern std::vector<GLfloat> vertexPositions;

#endif
