/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#define GLM_FORCE_RADIANS
#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "allmodels.h"
#include "myCube.h"
#include "myTeapot.h"
#include "swiat.h"
#include "mobki.h"
#include "objloader.hpp"

float speed_x=0; //angular speed in radians
float speed_y=0; //angular speed in radians
float camSpeedX = 0;
float camSpeedY = 0;
float camSpeedZ = 0;
float camSpeedRotateH = 0;
float camSpeedRotateV = 0;

float aspectRatio=1;
ShaderProgram* sp; //Pointer to the shader program
float amount = -1;

//Uncomment to draw a cube
float* vertices = myCubeVertices;
float* texCoords= myCubeTexCoords;
float* colors = myCubeColors;
float* normals = myCubeNormals;
int vertexCount = myCubeVertexCount;

std::vector<glm::vec3> verticesNew;
std::vector<glm::vec2> colorsNew;
std::vector<glm::vec3> normalsNew;

//Uncomment to draw a teapot
/*float* vertices = myTeapotVertices;
float* texCoords = myTeapotTexCoords;
float* colors = myTeapotColors;
float* normals = myTeapotVertexNormals;
int vertexCount = myTeapotVertexCount;*/







//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
        if (key==GLFW_KEY_LEFT) speed_x=-PI/2;
        if (key==GLFW_KEY_RIGHT) speed_x=PI/2;
        if (key==GLFW_KEY_UP) speed_y=PI/2;
        if (key==GLFW_KEY_DOWN) speed_y=-PI/2;
		if (key == GLFW_KEY_W) camSpeedZ = 2 * PI;
		if (key == GLFW_KEY_A) camSpeedX = 2 * PI;
		if (key == GLFW_KEY_S) camSpeedZ = -2 * PI;
		if (key == GLFW_KEY_D) camSpeedX = -2 * PI;
		if (key == GLFW_KEY_Z) camSpeedY = -2 * PI;
		if (key == GLFW_KEY_X) camSpeedY = 2 * PI;
		if (key == GLFW_KEY_Q) camSpeedRotateH = PI / 2;
		if (key == GLFW_KEY_E) camSpeedRotateH = -PI / 2;
		if (key == GLFW_KEY_R) camSpeedRotateV = PI / 2;
		if (key == GLFW_KEY_F) camSpeedRotateV = -PI / 2;
    }
    if (action==GLFW_RELEASE) {
        if (key==GLFW_KEY_LEFT) speed_x=0;
        if (key==GLFW_KEY_RIGHT) speed_x=0;
        if (key==GLFW_KEY_UP) speed_y=0;
        if (key==GLFW_KEY_DOWN) speed_y=0;
		if (key == GLFW_KEY_W) camSpeedZ = 0;
		if (key == GLFW_KEY_A) camSpeedX = 0;
		if (key == GLFW_KEY_S) camSpeedZ = 0;
		if (key == GLFW_KEY_D) camSpeedX = 0;
		if (key == GLFW_KEY_Z) camSpeedY = 0;
		if (key == GLFW_KEY_X) camSpeedY = 0;
		if (key == GLFW_KEY_Q) camSpeedRotateH = 0;
		if (key == GLFW_KEY_E) camSpeedRotateH = 0;
		if (key == GLFW_KEY_R) camSpeedRotateV = 0;
		if (key == GLFW_KEY_F) camSpeedRotateV = 0;
    }
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}


//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	//************Place any code here that needs to be executed once, at the program start************
	glClearColor(0.5,0.5,0.5,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);
	sp = new ShaderProgram("v_lambert2.glsl", NULL, "f_lambert.glsl");
	loadOBJ("tr.obj", verticesNew, colorsNew, normalsNew);
	//sp = new ShaderProgram("v_simplest.glsl", "g_simplest.glsl", "f_simplest.glsl");
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Place any code here that needs to be executed once, after the main loop ends************
	delete sp;
}

//Drawing procedure
void drawScene(GLFWwindow* window,float angle_x,float angle_y, float CamPosX, float CamPosY, float CamPosZ, float camAngleH, float camAngleV, float &mobPosX, float &mobPosY, float &mobPosZ) {
	//************Place any code here that draws something inside the window******************l

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	glm::mat4 V=glm::lookAt(
        glm::vec3(0.0f,0.0f,-3.0f),
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f)); //compute view matrix*/
	glm::mat4 V = glm::lookAt(
		glm::vec3(CamPosX, CamPosY, CamPosZ),
		glm::vec3(CamPosX + sin(camAngleH), CamPosY + camAngleV, CamPosZ + cos(camAngleH)),
		glm::vec3(0.0f, 1.0f, 0.0f)); //compute view matrix
    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, 1.0f, 1.0f, 50.0f); //compute projection matrix

    sp->use();//activate shading program
    //Send parameters to graphics card
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	
    glm::mat4 M=glm::mat4(1.0f);
	M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Compute model matrix
	M=glm::rotate(M,angle_x,glm::vec3(0.0f,1.0f,0.0f)); //Compute model matrix
    glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));

	glUniform4f(sp->u("light"), 10, 10, 10, 1);
	glEnableVertexAttribArray(sp->a("vertex")); //Enable sending data to the attribute vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Specify source of the data for the attribute vertex

	glEnableVertexAttribArray(sp->a("color")); //Enable sending data to the attribute color
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors2); //Specify source of the data for the attribute color

	glEnableVertexAttribArray(sp->a("normal")); //Enable sending data to the attribute normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); //Specify source of the data for the attribute normal

	mobki(sp, mobPosX, mobPosY, mobPosZ);
	surface(sp);


	/*
	glm::mat4 M1 = glm::mat4(1.0f);
	M1 = glm::rotate(M1, 30.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M1 = glm::translate(M1, glm::vec3(0.0f, 1.0f, 0.0f));
	M1 = glm::scale(M1, glm::vec3(0.5f, 0.5f, 0.5f));
	M1 = glm::scale(M1, glm::vec3(0.5f, 0.25f, 0.5f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M1));
	glUniform4f(spLambert->u("color"), 1, 1, 0, 1);
	Models::cube.drawSolid();
	*/

	glDisableVertexAttribArray(sp->a("vertex")); //Disable sending data to the attribute vertex
	glDisableVertexAttribArray(sp->a("color")); //Disable sending data to the attribute vertex
	glDisableVertexAttribArray(sp->a("normal")); //Disable sending data to the attribute normal

    glfwSwapBuffers(window); //Copy back buffer to front buffer
}

int main(void)
{


	GLFWwindow* window; //Pointer to object that represents the application window

	glfwSetErrorCallback(error_callback);//Register error processing callback procedure

	if (!glfwInit()) { //Initialize GLFW library
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it.

	if (!window) //If no window is opened then close the program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Since this moment OpenGL context corresponding to the window is active and all OpenGL calls will refer to this context.
	glfwSwapInterval(1); //During vsync wait for the first refresh

	GLenum err;
	if ((err = glewInit()) != GLEW_OK) { //Initialize GLEW library
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Call initialization procedure


	float angle_x = 0; //current rotation angle of the object, x axis
	float angle_y = 0; //current rotation angle of the object, y axis

	float camPosX;
	float camPosY;
	float camPosZ;
	float camAngleH;
	float camAngleV;
	float mobPosX = 1.0f;
	float mobPosY = 1.0f;
	float mobPosZ = 1.0f;
	float mobSpeedX = 1;
	float mobSpeedY = 0;
	float mobSpeedZ = 0;

	int cam = 2;
	// cam 1 // widok z rogu
	// cam 2 // widok z boku wysoko
	// cam 3 // widok prawie z góry

	if (cam == 1)
	{
		// Widok z rogu	
		camPosX = -1;
		camPosY = 2;
		camPosZ = -1;
		camAngleH = PI / 4;
		camAngleV = -PI / 6;
	}
	else if (cam == 2)
	{
		// Widok z boku wysoko
		camPosX = 10;
		camPosY = 15;
		camPosZ = -8;
		camAngleH = 0;
		camAngleV = -PI / 3;
	}
	else if (cam == 3)
		// Widok prawie z góry
	{
		camPosX = 10;
		camPosY = 20;
		camPosZ = 2;
		camAngleH = 0;
		camAngleV = -PI;
	}
	glfwSetTime(0); //Zero the timer
	//Main application loop
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{
		angle_x += speed_x * glfwGetTime(); //Add angle by which the object was rotated in the previous iteration
		angle_y += speed_y * glfwGetTime(); //Add angle by which the object was rotated in the previous iteration
		camPosX += camSpeedX * glfwGetTime();
		camPosY += camSpeedY * glfwGetTime();
		camPosZ += camSpeedZ * glfwGetTime();
		camAngleH += camSpeedRotateH * glfwGetTime();
		camAngleV += camSpeedRotateV * glfwGetTime();
		if (camAngleV > PI )
			camAngleV = PI ;
		if (camAngleV < -PI )
			camAngleV = -PI ;
		
		mobPosX += mobSpeedX * glfwGetTime();
		mobPosY += mobSpeedY * glfwGetTime();
		mobPosZ += mobSpeedZ * glfwGetTime();
		if (mobPosX > 9 && mobPosZ < 19) { mobPosX = 9; mobSpeedX = 0; mobSpeedZ = 1;}
		if (mobPosZ > 19) { mobPosZ = 19; mobSpeedZ = 0; mobSpeedX = 1; }
		if (mobPosX > 19 && mobPosZ == 19) { mobPosX = 19; mobSpeedX = 0; }
		amount += glfwGetTime();
		glfwSetTime(0); //Zero the timer
		drawScene(window,angle_x,angle_y, camPosX, camPosY, camPosZ, camAngleH, camAngleV, mobPosX, mobPosY, mobPosZ); //Execute drawing procedure
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
