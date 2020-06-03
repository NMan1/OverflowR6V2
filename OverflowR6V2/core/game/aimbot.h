#pragma once
#include "../driver/driver.h"
#include "../helpers/settings.h"
#include "game.h"


namespace game
{
	static uintptr_t camera;
	static bool get_camera()
	{
		camera = driver::read<uintptr_t>(base_address + profile_off);
		camera = driver::read<uintptr_t>(camera + 0x78);
		camera = driver::read<uintptr_t>(camera);
		camera = driver::read<uintptr_t>(camera + 0x130);
		camera = driver::read<uintptr_t>(camera + 0x420);
		if (!camera)
			return false;
		return true;
	}

	static vec3_t get_view_right()
	{
		if (!camera)
			return {};
		return driver::read<vec3_t>(camera + 0x7c0);
	}
	
	static vec3_t get_view_up()
	{
		if (!camera)
			return {};
		return driver::read<vec3_t>(camera + 0x7d0);
	}
	
	static vec3_t get_view_forward()
	{
		if (!camera)
			return {};
		return driver::read<vec3_t>(camera + 0x7e0);
	}
	
	static vec3_t get_view_translation()
	{
		if (!camera)
			return {};
		return driver::read<vec3_t>(camera + 0x7f0);
	}
		
	static float get_view_x()
	{
		if (!camera)
			return {};
		return driver::read<float>(camera + 0x800);
	}
		
	static float get_view_y()
	{
		if (!camera)
			return {};
		return driver::read<float>(camera + 0x814);
	}

	static std::uintptr_t closest_to_fov(vec3_t source_head, vec3_t entity_head, vec3_t source_angle)
	{
		const auto calc_angle = [](vec3_t src, vec3_t dest)
		{
			auto delta = src - dest;

			auto y = -asin(delta.z / src.Distance(dest)) * 57.2957795131f;
			auto x = (atan2(delta.y, delta.x) * 57.2957795131f) + 90.f;

			if (x > 180.f) x -= 360.f;
			else if (x < -180.f) x += 360.f;

			auto clamp = [](vec3_t& angle)
			{
				if (angle.y > 75.f) angle.y = 75.f;
				else if (angle.y < -75.f) angle.y = -75.f;
				if (angle.x < -180.f) angle.x += -360.f;
				else if (angle.x > 180.f) angle.x -= 360.f;

				angle.z = 0.f;
			};

			auto angle = vec3_t(x, y, 0.f);
			clamp(angle);

			return angle;
		};

		auto aim_angle = calc_angle(source_head, entity_head);

		auto calc_fov = [](vec3_t src, vec3_t aim)
		{
			aim -= src;

			if (aim.x > 180.f)
				aim.x -= 360.f;
			else if (aim.x < -180.f)
				aim.x += 360.f;
			if (aim.y > 180.f)
				aim.y -= 360.f;
			else if (aim.y < -180.f)
				aim.y += 360.f;

			return aim;
		};

		auto angle = calc_fov(source_angle, aim_angle);

		angle.x = std::abs(angle.x);
		angle.y = std::abs(angle.y);

		return angle.x + angle.y;
	}

	static vec4_t calculate_quaternion(vec3_t euler)
	{
		vec4_t result{};

		auto yaw = (euler.z * 0.01745329251f) * 0.5f;
		auto sy = std::sin(yaw);
		auto cy = std::cos(yaw);

		auto roll = (euler.x * 0.01745329251f) * 0.5f;
		auto sr = std::sin(roll);
		auto cr = std::cos(roll);

		constexpr auto sp = 0.f;
		constexpr auto cp = 1.f;

		result.x = cy * sr * cp - sy * cr * sp;
		result.y = cy * cr * sp + sy * sr * cp;
		result.z = sy * cr * cp - cy * sr * sp;
		result.w = cy * cr * cp + sy * sr * sp;

		return result;
	}

	static vec3_t calculate_euler(vec4_t quat)
	{
		auto y_p2 = quat.y * quat.y;

		auto x = std::atan2(2.f * (quat.w * quat.z + quat.x * quat.y), (1.f - 2.f * (y_p2 + quat.z * quat.z))) * 57.2957795131f;
		auto y = std::atan2(2.f * (quat.w * quat.x + quat.y * quat.z), (1.f - 2.f * (quat.x * quat.x + y_p2))) * 57.2957795131f;

		return vec3_t(x, y, 0.f);
	}

	static vec4_t get_view_angle(uintptr_t local)
	{
		uintptr_t view_angle = driver::read<uintptr_t>(local + 0x20);
		view_angle = driver::read<uintptr_t>(view_angle + 0x1200);
		return driver::read<vec4_t>(view_angle + 0xC0);
	}

	static vec3_t calc_angle(vec3_t enemypos, vec3_t camerapos)
	{
		float r2d = 57.2957795131f;
		vec3_t dir = enemypos - camerapos;

		float x = asin(dir.z / dir.Length()) * r2d;
		float z = atan(dir.y / dir.x) * r2d;

		if (dir.x >= 0.f) z += 180.f;
		if (x > 180.0f) x -= 360.f;
		else if (x < -180.0f) x += 360.f;

		return vec3_t(x, 0.f, z + 90.f);
	}

	bool w2s(vec3_t position, vec_t& Screen)
	{
		if (!camera)
			return false;

		vec3_t temp = position - get_view_translation();

		float x = temp.Dot(get_view_right());
		float y = temp.Dot(get_view_up());
		float z = temp.Dot(get_view_forward() * -1.f);

		Screen.x = (1920 / 2.f) * (1.f + x / get_view_x() / z);
		Screen.y = (1080 / 2.f) * (1.f - y / get_view_y() / z);

		return z >= 1.0f ? true : false;
	}

	float CrosshairDistance(vec_t vScreenPos)
	{
		return sqrt(pow(vScreenPos.y - (1080 / 2.f), 2.f) + pow(vScreenPos.x - (1920 / 2.f), 2.f));
	}

	static vec3_t get_closest_enemy()
	{
		auto best_fov = settings::aim_fov;
		vec3_t return_bone = {};

		static uintptr_t game_manager = driver::read<uintptr_t>(base_address + game_off);
		if (!game_manager)
			return {};

		uintptr_t entity_list = driver::read<uintptr_t>(game_manager + entity_list_off);
		int entity_count = driver::read<DWORD>(game_manager + entity_count_off) & 0x3fffffff;
		if (entity_count == NULL)
			return {};

		for (auto i = 0; i < entity_count; i++)
		{
			uintptr_t entity = driver::read<uintptr_t>(entity_list + i * 0x8);
			if (!entity)
				continue;

			if (!is_enemy(entity))
				continue;

			auto bone_pos = get_bone(entity);
			vec_t out;
			if (!w2s(bone_pos, out))
				continue;

			float distance = CrosshairDistance(out);
			if (distance < best_fov)
			{
				best_fov = distance;
				return_bone = bone_pos;
			}
		}

		return return_bone;
	}

	static bool set_view_angle(uintptr_t local, int silent, vec4_t angle)
	{
		uintptr_t view_angle = driver::read<uintptr_t>(local + 0x20);
		view_angle = driver::read<uintptr_t>(view_angle + 0x1200);
		if (!view_angle)
			return false;

		return driver::write<vec4_t>(view_angle + 0xC0, angle);
	}

	static bool run_aimbot()
	{
		static auto do_once = true;
		if (do_once)
		{
			if (!get_camera())
				return false;
			do_once = false;
		}

		auto bone_pos = get_closest_enemy();
		if (bone_pos.x == 0 && bone_pos.y == 0 && bone_pos.z == 0)
			return false;

		auto angle = calc_angle(bone_pos, get_view_translation());
		angle.clamp();

		if (settings::smoothing_value > 0)
			angle /= settings::smoothing_value;

		return set_view_angle(get_local(), 0xc0, calculate_quaternion(angle)); // 0x134 silent
	}
}