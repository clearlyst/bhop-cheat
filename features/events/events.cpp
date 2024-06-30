#include "events.hpp"
#include "../misc/misc.hpp"
#include "../../features/movement/movement.hpp"
#include "../visuals/visuals.hpp"

#pragma comment(lib, "Winmm.lib")

hooked_events events;

void hooked_events::fire_game_event(i_game_event* event) 
{
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	if (!strcmp(event->get_name(), "player_hurt"))
	{
		features::misc::hitmarker::event(event);
		features::misc::hit_info(event);	


		auto user_id = interfaces::engine->get_player_for_user_id(event->get_int("userid"));
		auto attacker_id = interfaces::engine->get_player_for_user_id(event->get_int("attacker"));
		auto local_id = interfaces::engine->get_local_player(); event->get_int("attacker") == local_id && event->get_int("userid") != local_id;

		player_info_t user, attacker;

		interfaces::engine->get_player_info(interfaces::engine->get_player_for_user_id(user_id), &user);
		interfaces::engine->get_player_info(interfaces::engine->get_player_for_user_id(attacker_id), &attacker);

		if (attacker_id == local_id && user_id != local_id)
		{
			if (c::misc::misc_hitchams)
			{
				const auto entity = reinterpret_cast<entity_t*>(interfaces::ent_list->get_client_entity(user_id));

				if (!entity)
					return;

				if (!entity->index())
					return;

				matrix_t matrix[128];

				if (!entity->setup_bones(matrix, 128, BONE_USED_BY_HITBOX, interfaces::globals->cur_time))
					return;

				studio_hdr_t* studio_model = interfaces::model_info->get_studio_model(entity->model());
				if (!studio_model)
					return;

				studio_hitbox_set_t* set = studio_model->hitbox_set(entity->hitbox_set());
				if (!set)
					return;

				for (int i = 0; i < set->hitbox_count; i++)
				{
					studio_box_t* hitbox = set->hitbox(i);

					if (!hitbox)
						continue;

					vec3_t min_actual, max_actual;

					math::vector_transform(hitbox->mins, matrix[hitbox->bone], min_actual);
					math::vector_transform(hitbox->maxs, matrix[hitbox->bone], max_actual);

					interfaces::debug_overlay->add_capsule_overlay(min_actual, max_actual, hitbox->radius, (int)c::misc::misc_hitchams_clr[0] * 255.0f, (int)c::misc::misc_hitchams_clr[1] * 255.0f, (int)c::misc::misc_hitchams_clr[2] * 255.0f, (int)c::misc::misc_hitchams_clr[3] * 255.0f, 2.5f);
				}
			}
		}
	}


	if (!strcmp(event->get_name(), "round_start"))
	{
		g::round_start = true;
	}

	if (!strcmp(event->get_name(), "bullet_impact"))
	{
		features::misc::bullet_impact::event(event);
	}
}

int hooked_events::get_event_debug_id(void) {
	return EVENT_DEBUG_ID_INIT;
}

void hooked_events::setup() {
	debug_id = EVENT_DEBUG_ID_INIT;

	interfaces::event_manager->add_listener(this, "round_start", false);
	interfaces::event_manager->add_listener(this, "round_end", false);
	interfaces::event_manager->add_listener(this, "player_death", false);
	interfaces::event_manager->add_listener(this, "player_hurt", false);
	interfaces::event_manager->add_listener(this, "bullet_impact", false);
	interfaces::event_manager->add_listener(this, "vote_cast", false);
	interfaces::event_manager->add_listener(this, "item_purchase", false);
	interfaces::event_manager->add_listener(this, "bomb_beginplant", false);
	interfaces::event_manager->add_listener(this, "bomb_abortplant", false);

	printf("events initialized!\n");
}

void hooked_events::unload() {
	interfaces::event_manager->remove_listener(this);
}
