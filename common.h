#pragma once

#include <sstream>
#include <iostream>
#include <vector>

#include <glew.h>
#include <freeglut.h>

namespace edt
{
	struct MeshInstance;

	struct
	{
		struct
		{
			int screen_width = 1024;
			int screen_height = 768;
			bool is_orthographic = false;
			float right = 20;
			float left = -20;
			float top = 10;
			float bottom = -10;
		} viewport;

		std::vector<MeshInstance*> walls;
		std::vector<MeshInstance*> machines;

		struct
		{
			GLuint wall;
			GLuint machine;
		} shaders;

	} global;

	//the name of the function that is being called
#define print(expression) \
	{\
		std::stringstream ss;\
		ss << expression;\
		std::cout << ss.str();\
	}

#define printf(expression) {print( "[" << __FUNCTION__ << "] " << expression << std::endl);}

}