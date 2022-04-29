#pragma once

//https://github.com/opengl-tutorials/ogl/blob/master/common/objloader.hpp
#include <vector>

namespace edt
{
	struct Vector;

	bool loadOBJ(const char* path, std::vector<Vector>& out_vertices, std::vector<Vector>* out_normals);
}