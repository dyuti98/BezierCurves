#include "Shaders.h"

void addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType)
{
	/**
	 *  Adds a shader.
	 */
	GLuint theShader = glCreateShader(shaderType);

	const GLchar *theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::printf("Error compiling %d shader: %s", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

GLuint compileShaders()
{
	/**
	 *  Compiles the shader.
	 */
	GLuint shader = glCreateProgram();

	if (!shader)
	{
		printf("Error creating shader program");
		return 0;
	}

	std::string vshader;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			vshader = vshader + "\n" + Line;
		VertexShaderStream.close();
	}
	else
	{
		printf("Cant open %s\n", vertex_file_path);
		getchar();
		return 0;
	}

	std::string fshader;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open())
	{
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			fshader = fshader + "\n" + Line;
		FragmentShaderStream.close();
	}
	else
	{
		printf("Cant open %s\n", vertex_file_path);
		getchar();
		return 0;
	}

	addShader(shader, vshader.c_str(), GL_VERTEX_SHADER);
	addShader(shader, fshader.c_str(), GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: %s", eLog);
		return 0;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: %s", eLog);
		return 0;
	}

	return shader;
}