#include <stdio.h>
#include <string.h>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//using namespace std;

static const char *vertex_file_path = "common/shaders/vertexshader.glsl";
static const char *fragment_file_path = "common/shaders/fragmentshader.glsl";

void addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);
GLuint compileShaders();