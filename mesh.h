#pragma once

#include "common.h"
#include "algebra.h"

#include <vector>
#include <functional>

namespace edt
{
	struct Triangle
	{
		int indices[3];
	};

	struct Mesh
	{
		struct
		{
			std::vector<Vector> positions;
			std::vector<Vector> normals;
			std::vector<Triangle> triangles;
		} geometry;

		struct
		{
			GLuint shader_id;
			//std::function<void(void)> render_function;
		} material;

		unsigned int vertex_buffer_object, index_buffer_object, vertex_array_object; // OpenGL handles for rendering
	};

	struct MeshInstance
	{
		Mesh* model;

		Vector translation;
		Vector rotation;
		Vector scaling;

		//unsigned int vertex_buffer_object, index_buffer_object, vertex_array_object; // OpenGL handles for rendering
	};
}