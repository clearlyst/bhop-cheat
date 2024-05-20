#include "../hooks.hpp"
#include "../../menu/config/config.hpp"
#include "../../sdk/structs/materials.hpp"
#include "../../utils/fnv.hpp"

void __fastcall sdk::hooks::get_color_modulation::get_color_modulation(i_material* material, void* edx, float* r, float* g, float* b) {
	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return ofunc(material, edx, r, g, b);

	ofunc(material, edx, r, g, b);

	if (!material || material->is_error_material())
		return ofunc(material, edx, r, g, b);

	const auto name = material->get_name();
	const auto group = material->get_texture_group_name();
	const auto is_world = group == strstr(group, ("World textures"));
	const auto is_sky = group == strstr(group, ("SkyBox textures"));
	const auto is_prop = group == strstr(group, ("StaticProp textures"));

	if (c::visuals::world::world_modulation::enable && is_world)
	{
		*r *= c::visuals::world::world_modulation::color[0];
		*g *= c::visuals::world::world_modulation::color[1];
		*b *= c::visuals::world::world_modulation::color[2];
	}

	if (c::visuals::world::skybox_modulation::enable && is_sky)
	{
		*r *= c::visuals::world::skybox_modulation::color[0];
		*g *= c::visuals::world::skybox_modulation::color[1];
		*b *= c::visuals::world::skybox_modulation::color[2];
	}

	if (c::visuals::world::props_modulation::enable && is_prop)
	{
		*r *= c::visuals::world::props_modulation::color[0];
		*g *= c::visuals::world::props_modulation::color[1];
		*b *= c::visuals::world::props_modulation::color[2];
	} 
	
}

bool __stdcall sdk::hooks::is_using_static_prop_debug_modes::is_using_static_prop_debug_modes() {
	return c::visuals::world::props_modulation::enable;
}

float __fastcall sdk::hooks::get_alpha_modulation::get_alpha_modulation(i_material* material, void* edx) {
	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return ofunc(material, edx);

	if (!material || material->is_error_material())
		return ofunc(material, edx);

	const auto group = material->get_texture_group_name();

	if (c::visuals::world::world_modulation::enable && strstr(group, ("World textures")))
	{
		return c::visuals::world::world_modulation::color[3];
	}

	if (c::visuals::world::skybox_modulation::enable && strstr(group, ("SkyBox textures")))
	{
		return c::visuals::world::skybox_modulation::color[3];
	}

	if (c::visuals::world::props_modulation::enable && strstr(group, ("StaticProp textures")))
	{
		return c::visuals::world::props_modulation::color[3];
	}

	return ofunc(material, edx);
}
