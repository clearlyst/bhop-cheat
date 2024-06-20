#include "../visuals/visuals.hpp"
#include "../../menu/config/config.hpp"

void features::visuals::glow::run()
{
	if (!c::visuals::glow::enable)
	{
		return;
	}

	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	for (auto i = 0; i < interfaces::glow_manager->size; i++) 
	{
		auto& glow = interfaces::glow_manager->objects[i];

		if (glow.unused())
		{
			continue;
		}

		auto glow_entity = reinterpret_cast<entity_t*>(glow.entity);

		auto client_class = glow_entity->client_class();

		if (!glow_entity)
		{
			continue;
		}

		if (c::visuals::glow::players::visible || c::visuals::glow::players::invisible)
		{
			if (client_class->class_id == C_CS_PLAYER)
			{
				const auto player = static_cast<player_t*>(glow.entity);

				if (player->dormant())
				{
					continue;
				}

				if (player->team() == g::local->team())
				{
					continue;
				}

				if (c::visuals::glow::players::visible && player->visible())
				{
					glow.glow_style = c::visuals::glow::players::type;
					glow.set(c::visuals::glow::players::color::visible[0], c::visuals::glow::players::color::visible[1], c::visuals::glow::players::color::visible[2], c::visuals::glow::players::color::visible[3]);
				}

				if (c::visuals::glow::players::invisible && !player->visible())
				{
					glow.set(c::visuals::glow::players::color::invisible[0], c::visuals::glow::players::color::invisible[1], c::visuals::glow::players::color::invisible[2], c::visuals::glow::players::color::invisible[3]);
				}
			}
		}

		if (c::visuals::glow::bomb_and_defuse_kit::enable)
			if (client_class->class_id == C_BASE_ANIMATING || client_class->class_id == C_C4)
				glow.set(c::visuals::glow::bomb_and_defuse_kit::color[0], c::visuals::glow::bomb_and_defuse_kit::color[1], c::visuals::glow::bomb_and_defuse_kit::color[2], c::visuals::glow::bomb_and_defuse_kit::color[3]);

		if (c::visuals::glow::planted_bomb::enable)
			if (client_class->class_id == C_PLANTED_C4)
				glow.set(c::visuals::glow::planted_bomb::color[0], c::visuals::glow::planted_bomb::color[1], c::visuals::glow::planted_bomb::color[2], c::visuals::glow::planted_bomb::color[3]);

		if (c::visuals::glow::hostage::enable)
			if (client_class->class_id == C_HOSTAGE)
				glow.set(c::visuals::glow::hostage::color[0], c::visuals::glow::hostage::color[1], c::visuals::glow::hostage::color[2], c::visuals::glow::hostage::color[3]);

		if (c::visuals::glow::grenade::enable)
			if (client_class->class_id == C_MOLOTOV_GRENADE || client_class->class_id == C_INCENDIARY_GRENADE || client_class->class_id == C_HE_GRENADE || client_class->class_id == C_SMOKE_GRENADE || client_class->class_id == C_FLASHBANG || client_class->class_id == C_DECOY_GRENADE || client_class->class_id == C_MOLOTOV_PROJECTILE || client_class->class_id == C_SMOKE_GRENADE_PROJECTILE || client_class->class_id == C_SENSOR_GRENADE || client_class->class_id == C_SENSOR_GRENADE_PROJECTILE || client_class->class_id == C_BASE_GRENADE && glow_entity && glow_entity->explode_effect_tick_begin() < 1)
				glow.set(c::visuals::glow::grenade::color[0], c::visuals::glow::grenade::color[1], c::visuals::glow::grenade::color[2], c::visuals::glow::grenade::color[3]);

		if (c::visuals::glow::weapon::enable)	
			if (glow_entity->is_weapon()) 
				glow.set(c::visuals::glow::weapon::color[0], c::visuals::glow::weapon::color[1], c::visuals::glow::weapon::color[2], c::visuals::glow::weapon::color[3]);
	}
}