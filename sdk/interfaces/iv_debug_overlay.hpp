#pragma once
#include "../math/vec3.hpp"
#include "../utils/vfunc.hpp"
#include "../structs/color.hpp"

class iv_debug_overlay {
public:
	bool world_to_screen(const vec3_t& in, vec3_t& out) {
		using original_fn = int(__thiscall*)(iv_debug_overlay*, const vec3_t&, vec3_t&);
		int return_value = (*(original_fn**)this)[13](this, in, out);
		return static_cast<bool>(return_value != 1);
	}

	void box_overlay(const vec3_t& origin, const vec3_t& mins, const vec3_t& maxs, const vec3_t& orientation, int r, int g, int b, int a, float duration)
	{
		using original_fn = void(__thiscall*)(void*, const vec3_t&, const vec3_t&, const vec3_t&, const vec3_t&, int, int, int, int, float);
		CallVFunction<original_fn>(this, 1)(this, origin, mins, maxs, orientation, r, g, b, a, duration);
	}

	void add_capsule_overlay(const vec3_t& mins, const vec3_t& maxs, float radius, int r, int g, int b, int a, float duration)
	{
		using original_fn = void(__thiscall*)(void*, const vec3_t&, const vec3_t&, float, int, int, int, int, float);
		CallVFunction<original_fn>(this, 24)(this, mins, maxs, radius, r, g, b, a, duration);
	}
};
