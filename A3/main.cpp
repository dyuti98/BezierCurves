#include <stdio.h>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shaders.h"
#include "variables.h"
#include "MidpointCircle.h"

#include <cstdlib>
#include <ctime>

//Windows Dimensions
const GLint WIDTH = 640, HEIGHT = 480;
const int radius = 10;

GLuint VAO, VBO, programID;

GLuint projectionviewPos;
glm::mat4 projection;

std::vector<GLfloat> vertexPositions;

/**
 *  A struct representing a point on a 2D plane.
 */
typedef struct pointStruct
{
	float x, y;
} Point;

/**
 *  the variable that denotes whether we are in edit mode (for deleting/modifying control points) or not.
 *  It's value is -1 when we are not in edit mode, and the index of the highlighted point when we are in edit mode.
 */
int toEdit = -1;

std::vector<Point> points;

void deCasteljauPoint(std::vector<Point> points, float t);
void deCasteljauCurve(std::vector<Point> points, int numPointsToDraw);
void AddPixel(int x, int y);
void drawPoints();
void mouseCallBack(GLFWwindow *window, int button, int action, int mods);
void keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods);

int main(void)
{

	cout << "******************************************************************************\n";
	cout << "* Left click to add a control point;\n";
	cout << "* Right click a control point to highlight it;\n";
	cout << "* Once a point is highlighted, press `delete` to remove it;\n";
	cout << "* Once a point is highlighted, left click anywhere to move it;\n";
	cout << "* Once a point is highlighted, right click anywhere else to un-highlight it.\n";
	cout << "******************************************************************************\n";

	//Initailize GLFW
	if (!glfwInit())
	{
		printf("GLFW initialization failed. Exiting..\n");
		glfwTerminate();
		return 1;
	}

	//Setup GLFW Windows Properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//No backward compatibilty
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Forward Compatibilty
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//Create a window
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Main Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("Window Not Created. Exiting..\n");
		glfwTerminate();
		return 1;
	}

	glfwSetMouseButtonCallback(mainWindow, mouseCallBack);
	glfwSetKeyCallback(mainWindow, keyCallBack);

	glfwSetWindowSizeLimits(mainWindow, WIDTH, HEIGHT, WIDTH, HEIGHT);

	//Get Buffer Size Info
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Allow Modern Features
	glewExperimental = GL_TRUE;

	//Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed. Exiting..\n");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//Setup Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	//Compile Shaders and link it to the current program(programID)
	programID = compileShaders();

	while (!glfwWindowShouldClose(mainWindow))
	{

		//Get and Handle User Events
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		glm::mat4 projection = glm::ortho(0.0f, (GLfloat)WIDTH, 0.0f, (GLfloat)HEIGHT, -1.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, &projection[0][0]);

		deCasteljauCurve(points, 2000);

		glBindVertexArray(VAO);

		glDrawArrays(GL_POINTS, 0, vertexPositions.size());

		glEnable(GL_PROGRAM_POINT_SIZE);

		glBindVertexArray(0);

		vertexPositions.clear();

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}

void deCasteljauPoint(std::vector<Point> points, float t)
{
	/**
	 *  Adds a Bezier Curve's point to be drawn using de Casteljau's algorithm, given that point's parametric value.
	 *  The vector of control points and the parametric value must be provided.
	 */
	if (points.size() <= 0)
	{
		return;
	}
	else if (points.size() == 1) // base case
	{
		AddPixel((int)points[0].x, (int)points[0].y);
	}
	else
	{
		std::vector<Point> newPoints;
		for (int i = 0; i < points.size() - 1; i++)
		{
			Point p;
			p.x = points[i].x * t + points[i + 1].x * (1 - t);
			p.y = points[i].y * t + points[i + 1].y * (1 - t);
			newPoints.push_back(p);
		}
		deCasteljauPoint(newPoints, t);
	}
}

void deCasteljauCurve(std::vector<Point> points, int numPointsToDraw)
{
	/**
	 *  Draws a Bezier Curve using de Casteljau's algorithm.
	 *  The vector of control points and the number of points to draw must be provided.
	 */
	for (float t = 0.0f; t < 1.0f; t += 1.0f / numPointsToDraw)
		deCasteljauPoint(points, t);
	for (int i = 0; i < points.size(); i++)
	{
		CircleDraw(points[i].x, points[i].y, radius);
		if (toEdit == i)
			CircleDraw(points[i].x, points[i].y, radius + 3);
	}
	drawPoints();
}

void AddPixel(int x, int y)
{
	/**
	 *  Adds a pixel to the vector who's points are to be drawn.
	 */
	vertexPositions.push_back(x);
	vertexPositions.push_back(y);
	vertexPositions.push_back(0.0);
}

void drawPoints()
{
	/**
	 *  Draws the points stored in the vector `vertexPositions`
	 */

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(GLfloat), &vertexPositions[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void mouseCallBack(GLFWwindow *window, int button, int action, int mods)
{
	/**
	 *  Handles mouse clicks to make sure control points can added, highlighted and moved.
	 */
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		Point p;
		p.x = (float)xpos;
		p.y = (float)(HEIGHT - ypos);
		if (toEdit == -1)
		{
			points.push_back(p);
		}
		else
		{
			points[toEdit] = p;
			toEdit = -1;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		Point p;
		float x = (float)xpos;
		float y = (float)(HEIGHT - ypos);
		if (toEdit == -1)
		{
			for (int i = 0; i < points.size(); i++)
			{
				Point p = points[i];
				if (abs(x - p.x) * abs(x - p.x) + abs(y - p.y) * abs(y - p.y) < radius * radius)
				{
					toEdit = i;
					return;
				}
			}
		}
		else
		{
			toEdit = -1;
		}
	}
}

void keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	/**
	 *  Handles Keypresses to make sure control points can be deleted
	 */
	if (action == GLFW_PRESS && key == GLFW_KEY_DELETE && toEdit != -1)
	{
		points.erase(points.begin() + toEdit);
		toEdit = -1;
	}
}
