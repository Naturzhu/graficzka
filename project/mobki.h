#ifndef MOBKI_H_INCLUDED
#define MOBKI_H_INCLUDED

#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "allmodels.h"
#include "myCube.h"
#include "myTeapot.h"
#include "swiat.h"

void mobki(ShaderProgram* sp, float &mobPosX, float &mobPosY, float &mobPosZ)
{
	int col = 10; // liczba kolumn
	int row = 10; // liczba wierszy
	

	glm::mat4 K = glm::mat4(1.0f);
	K = glm::translate(K, glm::vec3(mobPosX, mobPosY, mobPosZ));
	K = glm::scale(K, glm::vec3(0.2f, 0.2f, 0.2f));
	glm::mat4 Kn;

	/*for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			Kn = glm::translate(K, glm::vec3(i * 2.0f, 0.0f, j * 2.0f));
			glm::mat4 M = Kn;
			glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
			glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount); //Draw the object
			//glDrawArrays(GL_TRIANGLES, 18, 6); //Tylko gorna sciana
		}
	}
	*/
	Kn = K;
	glm::mat4 M = Kn;
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);
}

#endif // MOBKI_H_INCLUDED