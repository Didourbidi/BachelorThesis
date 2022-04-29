#pragma once
#include "algebra.h"
#include <math.h>

namespace edt
{
	class Camera {
	public:
		Vector position; //camera position
		float FoV; //field of view
		Vector rotation;
		float near_clip;
		float far_clip;
		float aspect_ratio;
		Vector up_direction;
		Vector forward_direction;
		Vector right_direction;

	public:

		Camera(const Vector& pos, const Vector& rotation, const float& fov, const float& near_clip, const float& far_clip, const float& aspect_ratio)
			: position(pos), rotation(rotation), FoV(fov), near_clip(near_clip), far_clip(far_clip), aspect_ratio(aspect_ratio)
		{
			rotation_fixer();
		}

		//camera x,y,z positions

		void set_position(Vector new_position)
		{
			position = new_position;
		}

		void move(Vector position_adjustment)
		{
			position = Add(position, position_adjustment);
		}

		//move with camera coordinates
		//transform from local to global coordinates
		void move_camera_relative(Vector position_adjustment)
		{
			Vector world_adjustment = euler_rotate(rotation, position_adjustment);
			position = Add(position, world_adjustment);
		}

		void set_rotation(Vector new_rotation)
		{
			rotation = new_rotation;
			rotation_fixer();
		}

		void rotate(Vector rotation_adjustment)
		{
			rotation = Add(rotation, rotation_adjustment);
			rotation_fixer();
		}

		void rotation_fixer()
		{
			//modulo for floats. fixes rotation when angle is bigger than 360
			if (rotation.x > 85.0f)
				rotation.x = 85.0f;
			else if (rotation.x < -85.0f)
				rotation.x = -85.0f;

			//rotation.x = fmodf(rotation.x, 360.0f);

			rotation.y = fmodf(rotation.y, 360.0f);
			rotation.z = fmodf(rotation.z, 360.0f);

			Vector forw_vec = { 0.0f, 0.0f, -1.0f };
			forward_direction = euler_rotate(rotation, forw_vec);

			Vector up_vec = { 0.0f, 1.0f, 0.0f };
			right_direction = CrossProduct(forward_direction, up_vec);
			right_direction = Normalize(right_direction);

			up_direction = CrossProduct(right_direction, forward_direction);
		}

		Matrix get_view_matrix()
		{
			//:: neutral namespace (global function)
			return ::get_view_matrix(position, rotation);
		}

		Matrix get_projection_matrix()
		{
			return PerspectiveProjection(FoV, far_clip, near_clip, aspect_ratio);
		}

		Matrix get_view_matrix_without_translate()
		{
			//:: neutral namespace (global function)
			return ::get_view_matrix_without_translate(position, rotation);
		}

	};
}