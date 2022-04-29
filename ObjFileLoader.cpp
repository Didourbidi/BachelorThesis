#include "ObjFileLoader.h"
#include "algebra.h"

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

//https://github.com/opengl-tutorials/ogl/blob/master/common/objloader.cpp
// Very, VERY simple OBJ loader.

namespace edt
{
	bool loadOBJ(const char* path, std::vector<Vector>& out_vertices, std::vector<Vector>* out_normals) 
	{
		printf("Loading OBJ file %s\n", path);

		std::vector<unsigned int> vertexIndices, normalIndices;
		std::vector<Vector> temp_vertices;
		std::vector<Vector> temp_normals;


		FILE* file = nullptr;
		auto error_nr = fopen_s( &file, path, "r");

		if (error_nr) {
			printf("Impossible to open the file !\n");
			getchar();
			return false;
		}

		while (1) {

			char lineHeader[128];
			lineHeader[127] = 0;
			// read the first word of the line
			int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

			// else : parse lineHeader

			if (strcmp(lineHeader, "v") == 0) {
				Vector vertex;
				fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				Vector normal;
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], 
					&uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read.\n");
					fclose(file);
					return false;
				}
				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);
				normalIndices.push_back(normalIndex[0] - 1);
				normalIndices.push_back(normalIndex[1] - 1);
				normalIndices.push_back(normalIndex[2] - 1);
			}
			else 
			{
				char stupidBuffer[1000];
				fgets(stupidBuffer, 1000, file);
			}
		}

		// For each vertex of each triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++) {

			// Get the indices of its attributes
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int normalIndex = normalIndices[i];

			// Get the attributes thanks to the index
			Vector vertex = temp_vertices[vertexIndex];
			Vector normal = temp_normals[normalIndex];

			// Put the attributes in buffers
			out_vertices.push_back(vertex);
			if(out_normals)
				out_normals->push_back(normal);

		}
		fclose(file);
		return true;
	}
}