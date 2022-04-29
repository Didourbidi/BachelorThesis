#pragma once

namespace edt
{
	struct Vector
	{ 
		bool operator == (const Vector& other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}
		float x, y, z; 

		bool operator - (const Vector& other) const
		{
			return x - other.x && y - other.y && z - other.z;
		}
		float x, y, z;
	};

	struct HomVector
	{ 
		float x, y, z, w; 
	};

	/* Column-major order are used for the matrices here to be compatible with OpenGL.
	** The indices used to access elements in the matrices are shown below.
	**  _                _
	** |                  |
	** |   0   4   8  12  |
	** |                  |
	** |   1   5   9  13  |
	** |                  |
	** |   2   6  10  14  |
	** |                  |
	** |   3   7  11  15  |
	** |_                _|
	*/

	struct Matrix 
	{ 
		float e[16]; 
	};

	float degrees_to_rad(float angle);

	Vector Add(Vector a, Vector b);
	Vector Subtract(Vector a, Vector b);
	Vector CrossProduct(Vector a, Vector b);
	float DotProduct(Vector a, Vector b);
	float Length(Vector a);
	Vector Normalize(Vector a);
	Vector ScalarVecMul(float t, Vector a);

	Matrix create_Translation_mat(Vector b);
	Matrix create_Scaling_mat(Vector b);
	//Matrix create_Rotation_mat(Vector b);
	Matrix create_Rotation_xmat(float b);
	Matrix create_Rotation_ymat(float b);
	Matrix create_Rotation_zmat(float b);

	Matrix construct_perspective_matrix(float fov, float nearPlane, float farPlane, float aspectRatio);
	Matrix construct_parallel_matrix(float right, float left, float top, float bottom, float farPlane, float nearPlane);

	Vector euler_rotate(const Vector& rotation, const Vector& input_vec);
	Matrix get_view_matrix(const Vector& position, const Vector& rotation);
	Matrix get_view_matrix_without_translate(const Vector& position, const Vector& rotation);

	HomVector MatVecMul(Matrix a, Vector b);
	Vector Homogenize(HomVector a);
	Matrix MatMatMul(Matrix a, Matrix b);
	void PrintMatrix(char* name, Matrix m);
	void PrintVector(char* name, Vector v);
	void PrintHomVector(char* name, HomVector h);
}