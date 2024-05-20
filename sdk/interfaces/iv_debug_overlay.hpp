#pragma once
#include "../math/vec3.hpp"
#include "../utils/vfunc.hpp"
#include "../structs/color.hpp"

#define get_virtual_fn( index, function_name, type, ... )  auto function_name { return CallVFunction<type>( this, index )( this, __VA_ARGS__ ); };

class iv_debug_overlay {
public:
	bool world_to_screen(const vec3_t& in, vec3_t& out) {
		using original_fn = int(__thiscall*)(iv_debug_overlay*, const vec3_t&, vec3_t&);
		int return_value = (*(original_fn**)this)[13](this, in, out);
		return static_cast<bool>(return_value != 1);
	}

	get_virtual_fn(24, add_capsule_overlay(vec3_t& mins, vec3_t& maxs, float radius, color_t color, float duration), void(__thiscall*)(void*, vec3_t&, vec3_t&, float&, float, float, float, float, float), mins, maxs, radius, color.r(), color.g(), color.b(), color.a(), duration);

};
