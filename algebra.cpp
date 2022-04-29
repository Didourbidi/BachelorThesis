#define _USE_MATH_DEFINES // To get M_PI defined
#include <math.h>
#include <stdio.h>

//for memset
#include <string.h>
#include "algebra.h"

#include "common.h"

namespace edt
{
	//function in order to translate degrees to rad cause rotation is expecting radians
	float degrees_to_rad(float angle)
	{
		return (angle * 3.14159265359f) / 180.0f;
	}

	Vector CrossProduct(Vector a, Vector b) {
		Vector v = { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
		return v;
	}

	float DotProduct(Vector a, Vector b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	Vector Subtract(Vector a, Vector b) {
		Vector v = { a.x - b.x, a.y - b.y, a.z - b.z };
		return v;
	}

	Vector Add(Vector a, Vector b) {
		Vector v = { a.x + b.x, a.y + b.y, a.z + b.z };
		return v;
	}

	//sqrtf otherwise it complains
	float Length(Vector a) {
		return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	}

	Vector Normalize(Vector a) {
		float len = Length(a);
		Vector v = { a.x / len, a.y / len, a.z / len };
		return v;
	}

	Vector ScalarVecMul(float t, Vector a) {
		Vector b = { t * a.x, t * a.y, t * a.z };
		return b;
	}

	// translation matrix
	Matrix create_Translation_mat(Vector b)
	{
		Matrix result;
		//set everything to zero (less code)
		memset(result.e, 0, sizeof(result.e));

		result.e[0] = 1.0f;
		result.e[5] = 1.0f;
		result.e[10] = 1.0f;
		result.e[15] = 1.0f;
		result.e[12] = b.x;
		result.e[13] = b.y;
		result.e[14] = b.z;

		return result;
	}

	//scaling matrix
	Matrix create_Scaling_mat(Vector b)
	{
		Matrix result;
		//set everything to zero (less code)
		memset(result.e, 0, sizeof(result.e));

		result.e[0] = b.x;
		result.e[5] = b.y;
		result.e[10] = b.z;
		result.e[15] = 1.0f;

		return result;
	}

	//rotation matrix
	Matrix create_Rotation_xmat(float b)
	{
		b = degrees_to_rad(b);

		Matrix rot_x;
		//set everything to zero (less code)
		memset(rot_x.e, 0, sizeof(rot_x.e));
		rot_x.e[0] = 1.0f;
		rot_x.e[5] = cosf(b);
		rot_x.e[10] = cosf(b);
		rot_x.e[15] = 1.0f;
		rot_x.e[9] = -sinf(b);
		rot_x.e[6] = sinf(b);

		return rot_x;
	}

	//rotation matrix
	Matrix create_Rotation_ymat(float b)
	{
		b = degrees_to_rad(b);

		Matrix rot_y;
		//set everything to zero (less code)
		memset(rot_y.e, 0, sizeof(rot_y.e));
		rot_y.e[0] = cosf(b);
		rot_y.e[5] = 1.0f;
		rot_y.e[10] = cosf(b);
		rot_y.e[15] = 1.0f;
		rot_y.e[8] = sinf(b);
		rot_y.e[2] = -sinf(b);

		return rot_y;
	}

	Matrix create_Rotation_zmat(float b)
	{
		b = degrees_to_rad(b);

		Matrix rot_z;
		//set everything to zero (less code)
		memset(rot_z.e, 0, sizeof(rot_z.e));
		rot_z.e[0] = cosf(b);
		rot_z.e[5] = cosf(b);
		rot_z.e[10] = 1.0f;
		rot_z.e[15] = 1.0f;
		rot_z.e[4] = -sinf(b);
		rot_z.e[1] = sinf(b);

		return rot_z;
	}

	//matrix to calculate the projection
	Matrix construct_perspective_matrix(float fov, float nearPlane, float farPlane, float aspectRatio)
	{
		fov = degrees_to_rad(fov);
		Matrix result;
		//set everything to zero (less code)
		memset(result.e, 0, sizeof(result.e));
		float S = 1.0f / tanf(fov / 2.0f);
		result.e[0] = S * 1.0f / aspectRatio;
		result.e[5] = S;
		result.e[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
		result.e[11] = -1.0f;
		result.e[14] = -2.0f * farPlane * nearPlane / (farPlane - nearPlane);


		return result;
	}

	//we need to rotate a vector
	//here the function calculates the vector rotated by cam rotation and return it
	Vector euler_rotate(const Vector& rotation, const Vector& input_vec)
	{
		Vector rot_radians = ScalarVecMul((3.141592f / 180.0f), rotation);

		// x axis rotation. roll eulers angle(rotation) 
		float x1 = input_vec.x;
		float y1 = input_vec.y * cos(rot_radians.x) - input_vec.z * sin(rot_radians.x);
		float z1 = input_vec.y * sin(rot_radians.x) + input_vec.z * cos(rot_radians.x);

		// y axis rotation. pitch eulers angle (rotation)
		float x2 = x1 * cos(rot_radians.y) + z1 * sin(rot_radians.y);
		float y2 = y1;
		float z2 = -x1 * sin(rot_radians.y) + z1 * cos(rot_radians.y);

		// z axis rotation. yaw eulers angle(rotation) 
		float x3 = x2 * cos(rot_radians.z) - y2 * sin(rot_radians.z);
		float y3 = x2 * sin(rot_radians.z) + y2 * cos(rot_radians.z);
		float z3 = z2;

		Vector v = { x3, y3, z3 };
		return v;
	}

	Matrix get_view_matrix(const Vector& position, const Vector& rotation)
	{
		Matrix v;
		Matrix t = create_Translation_mat(- position);
		Matrix rx = create_Rotation_xmat(-rotation.x);
		Matrix ry = create_Rotation_ymat(-rotation.y);
		Matrix rz = create_Rotation_zmat(-rotation.z);
#if 0 // This broke the camera
		v = MatMatMul(rz, ry);
		v = MatMatMul(v, rx);
		v = MatMatMul(v, t);
#else
		v = MatMatMul(rx, ry);
		v = MatMatMul(v, rz);
		v = MatMatMul(v, t);
#endif
		return v;
	}

	Matrix get_view_matrix_without_translate(const Vector& position, const Vector& rotation)
	{
		Matrix v;
		Matrix rx = create_Rotation_xmat(-rotation.x);
		Matrix ry = create_Rotation_ymat(-rotation.y);
		Matrix rz = create_Rotation_zmat(-rotation.z);

		v = MatMatMul(rx, ry);
		v = MatMatMul(v, rz);

		return v;
	}

	//matrix to calculate the parallel projection
	Matrix construct_parallel_matrix(float right, float left, float top, float bottom, float farPlane, float nearPlane)
	{
		Matrix result;
		//set everything to zero (less code)
		memset(result.e, 0, sizeof(result.e));
		result.e[0] = 2.0f / (right - left);
		result.e[5] = 2.0f / (top - bottom);
		result.e[10] = -2.0f / (farPlane - nearPlane);
		result.e[12] = -(right + left) / (right - left);
		result.e[13] = -(top + bottom) / (top - bottom);
		result.e[14] = -(farPlane + nearPlane) / (farPlane - nearPlane);
		result.e[15] = 1.0f;

		return result;
	}

	HomVector MatVecMul(Matrix a, Vector b) {
		HomVector h;
		h.x = b.x * a.e[0] + b.y * a.e[4] + b.z * a.e[8] + a.e[12];
		h.y = b.x * a.e[1] + b.y * a.e[5] + b.z * a.e[9] + a.e[13];
		h.z = b.x * a.e[2] + b.y * a.e[6] + b.z * a.e[10] + a.e[14];
		h.w = b.x * a.e[3] + b.y * a.e[7] + b.z * a.e[11] + a.e[15];
		return h;
	}

	Vector Homogenize(HomVector h) {
		Vector a;
		if (h.w == 0.0) {
			fprintf(stderr, "Homogenize: w = 0\n");
			a.x = a.y = a.z = 9999999;
			return a;
		}
		a.x = h.x / h.w;
		a.y = h.y / h.w;
		a.z = h.z / h.w;
		return a;
	}

	Matrix MatMatMul(Matrix a, Matrix b) {
		Matrix c;
		int i, j, k;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				c.e[j * 4 + i] = 0.0;
				for (k = 0; k < 4; k++)
					c.e[j * 4 + i] += a.e[k * 4 + i] * b.e[j * 4 + k];
			}
		}
		return c;
	}

	void PrintVector(char* name, Vector a) {
		printf("Vector(" << name << " " << a.x << " " << a.y << " " << a.z << ")");
	}

	void PrintHomVector(char* name, HomVector a) {
		printf("Vector(" << name << " " << a.x << " " << a.y << " " << a.z << " " << a.w << ")");
	}

	void PrintMatrix(char* name, Matrix a) {
		int i, j;
		std::stringstream ss;
		ss << "Matrix: " << name << std::endl;

		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				ss << a.e[j * 4 + i];
			}
			ss << std::endl;
		}
	}
}