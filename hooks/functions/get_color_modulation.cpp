#include "../hooks.hpp"
#include "../../menu/config/config.hpp"
#include "../../sdk/structs/materials.hpp"

void __fastcall sdk::hooks::get_color_modulation::get_color_modulation(i_material* material, void* edx, float* r, float* g, float* b) {
	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return ofunc(material, edx, r, g, b);

	ofunc(material, edx, r, g, b);

	if (!c::visuals::world::world_modulation::enable || !c::visuals::world::skybox_modulation::enable || !c::visuals::world::props_modulation::enable)
		return ofunc(material, edx, r, g, b);

	if (!material || material->is_error_material())
		return ofunc(material, edx, r, g, b);

	const auto group = material->get_texture_group_name();

	if (strstr(group, ("World textures")))
	{
		*r *= c::visuals::world::world_modulation::color[0];
		*g *= c::visuals::world::world_modulation::color[1];
		*b *= c::visuals::world::world_modulation::color[2];
	}

	if (strstr(group, ("SkyBox textures")))
	{
		*r *= c::visuals::world::skybox_modulation::color[0];
		*g *= c::visuals::world::skybox_modulation::color[1];
		*b *= c::visuals::world::skybox_modulation::color[2];
	}

	if (strstr(group, ("StaticProp textures")))
	{
		*r *= c::visuals::world::props_modulation::color[0];
		*g *= c::visuals::world::props_modulation::color[1];
		*b *= c::visuals::world::props_modulation::color[2];
	}
}

float __fastcall sdk::hooks::get_alpha_modulation::get_alpha_modulation(i_material* material, void* edx) {
	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return ofunc(material, edx);

	if (!c::visuals::world::world_modulation::enable || !c::visuals::world::skybox_modulation::enable || !c::visuals::world::props_modulation::enable)
		return ofunc(material, edx);

	if (!material || material->is_error_material())
		return ofunc(material, edx);

	const auto group = material->get_texture_group_name();

	if (strstr(group, ("World textures")))
	{
		return c::visuals::world::world_modulation::color[3];
	}

	if (strstr(group, ("SkyBox textures")))
	{
		return c::visuals::world::skybox_modulation::color[3];
	}

	if (strstr(group, ("StaticProp textures")))
	{
		return c::visuals::world::props_modulation::color[3];
	}

	return ofunc(material, edx);
}
