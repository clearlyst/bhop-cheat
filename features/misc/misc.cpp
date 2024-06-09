#include "misc.hpp"
#include "../../menu/menu.hpp"
#include "../../includes/imgui/imgui.h"
#include "../../includes/imgui/imgui_internal.h"
#include "../../features/visuals/visuals.hpp"
#include "../../features/notifications/notifications.hpp"
#include "../../sdk/sdk.hpp"
#include "../../sdk/steam/steamclientpublic.h"

int current_checkpoint = 0;
float press_delay = 0.0f;
std::deque<std::pair<vec3_t, vec3_t>> checkpoints;

struct client_hit_data_t
{
	vec3_t position;
	float time;
	float expires;
};

int hitmarker_time = 0;

void features::misc::fix_mouse_delta(c_usercmd* cmd)
{
	if (!c::misc::mousefix)
	{
		return;
	}

	static vec3_t delta_viewangles{ };
	vec3_t delta = cmd->view_angles - delta_viewangles;
	delta.clamp();

	static convar* sensitivity = interfaces::console->get_convar("sensitivity");

	if (!sensitivity)
	{
		return;
	}

	if (delta.x != 0.f) 
	{
		static convar* m_pitch = interfaces::console->get_convar("m_pitch");

		if (!m_pitch)
		{
			return;
		}

		int final_dy = static_cast<int>((delta.x / m_pitch->get_float()) / sensitivity->get_float());

		if (final_dy <= 32767) 
		{
			if (final_dy >= -32768) 
			{
				if (final_dy >= 1 || final_dy < 0)
				{
					if (final_dy <= -1 || final_dy > 0)
					{
						final_dy = final_dy;
					}
					else
					{
						final_dy = -1;
					}
				}
				else 
				{
					final_dy = 1;
				}
			}
			else 
			{
				final_dy = 32768;
			}
		}
		else 
		{
			final_dy = 32767;
		}

		cmd->mouse_dy = static_cast<short>(final_dy);
	}

	if (delta.y != 0.f) 
	{
		static convar* m_yaw = interfaces::console->get_convar("m_yaw");

		if (!m_yaw)
		{
			return;
		}

		int final_dx = static_cast<int>((delta.y / m_yaw->get_float()) / sensitivity->get_float());

		if (final_dx <= 32767) 
		{
			if (final_dx >= -32768) 
			{
				if (final_dx >= 1 || final_dx < 0) 
				{
					if (final_dx <= -1 || final_dx > 0)
					{
						final_dx = final_dx;
					}
					else
					{
						final_dx = -1;
					}
				}
				else 
				{
					final_dx = 1;
				}
			}
			else 
			{
				final_dx = 32768;
			}
		}
		else 
		{
			final_dx = 32767;
		}

		cmd->mouse_dx = static_cast<short>(final_dx);
	}

	delta_viewangles = cmd->view_angles;
}

void features::misc::checkpoint_system()
{
	if (!c::misc::practice)
	{
		checkpoints.clear();
		current_checkpoint = 0;
		press_delay = 0.0f;
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game() || !g::local || !g::local->is_alive())
	{
		checkpoints.clear();
		current_checkpoint = 0;
		press_delay = 0.0f;
		return;
	}

	if (g::local->move_type() == movetype_noclip)
	{
		checkpoints.clear();
		current_checkpoint = 0;
		press_delay = 0.0f;
		return;
	}

	if (interfaces::console->get_convar("sv_cheats")->get_int() == 0)
	{
		checkpoints.clear();
		current_checkpoint = 0;
		press_delay = 0.0f;
		return;
	}

	vec3_t hAng = vec3_t(0, 0, 0);

	if (press_delay < interfaces::globals->realtime)
	{
		if (menu::checkkey(c::misc::savepos, c::misc::savepos_s))
		{
			interfaces::engine->get_view_angles(hAng);

			checkpoints.push_front({ vec3_t(hAng.x, hAng.y, hAng.z), g::local->origin() });

			current_checkpoint = 0;

			if (checkpoints.size() > 99)
			{
				checkpoints.pop_back();
			}

			for (int i = 1; i <= 1; ++i)
			{
				features::notification::run("succesfully created & saved checkpoint", "#_print_created_checkpoint", true, true, true);
			}

			press_delay = interfaces::globals->realtime + 0.2f;
		}

		if (!checkpoints.empty())
		{
			if (menu::checkkey(c::misc::loadpos, c::misc::loadpos_s))
			{
				auto& cur = checkpoints.at(current_checkpoint);

				std::string cmd = ("setpos_exact ") + std::to_string(cur.second.x) + (" ") + std::to_string(cur.second.y) + (" ") + std::to_string(cur.second.z);
				interfaces::engine->execute_cmd(cmd.c_str());

				cmd = ("setang_exact ") + std::to_string(cur.first.x) + (" ") + std::to_string(cur.first.y) + (" ") + std::to_string(cur.first.z);
				interfaces::engine->execute_cmd(cmd.c_str());

				for (int i = 1; i <= 1; ++i)
				{
					features::notification::run("succesfully teleported to checkpoint", "#_print_teleport_checkpoint", true, true, true);
				}

				press_delay = interfaces::globals->realtime + 0.2f;
			}
		}
	}
}

void features::misc::viewfov()
{
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	static auto save_viewmodel_fov = interfaces::console->get_convar("viewmodel_fov")->get_float();

	interfaces::console->get_convar("viewmodel_fov")->callbacks.clear();

	if (c::misc::enable_fov)
	{
		interfaces::console->get_convar("viewmodel_fov")->set_value(c::misc::field_of_view);
	}
	else
	{
		interfaces::console->get_convar("viewmodel_fov")->set_value(save_viewmodel_fov);
	}
}

void features::misc::viewmodel()
{
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	static auto save_view_model_x = interfaces::console->get_convar("viewmodel_offset_x")->get_float();
	static auto save_view_model_y = interfaces::console->get_convar("viewmodel_offset_y")->get_float();
	static auto save_view_model_z = interfaces::console->get_convar("viewmodel_offset_z")->get_float();

	interfaces::console->get_convar("viewmodel_offset_x")->callbacks.clear();
	interfaces::console->get_convar("viewmodel_offset_y")->callbacks.clear();
	interfaces::console->get_convar("viewmodel_offset_z")->callbacks.clear();

	if (c::misc::view_model)
	{
		interfaces::console->get_convar("viewmodel_offset_x")->set_value(c::misc::view_model_x);
		interfaces::console->get_convar("viewmodel_offset_y")->set_value(c::misc::view_model_y);
		interfaces::console->get_convar("viewmodel_offset_z")->set_value(c::misc::view_model_z);
	}
	else
	{
		interfaces::console->get_convar("viewmodel_offset_x")->set_value(save_view_model_x);
		interfaces::console->get_convar("viewmodel_offset_y")->set_value(save_view_model_y);
		interfaces::console->get_convar("viewmodel_offset_z")->set_value(save_view_model_z);
	}
}

void features::misc::swayscale()
{
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	static auto save_sway_scale = interfaces::console->get_convar("cl_wpn_sway_scale")->get_float();

	interfaces::console->get_convar("cl_wpn_sway_scale")->callbacks.clear();

	if (c::misc::swayscale)
	{
		interfaces::console->get_convar("cl_wpn_sway_scale")->set_value(c::misc::swayscale_value);
	}
	else
	{
		interfaces::console->get_convar("cl_wpn_sway_scale")->set_value(save_sway_scale);
	}
}

void apply_clan_tag(const char* tag, const char* name) 
{
	using fn = int(__fastcall*)(const char*, const char*);
	static auto apply_clan_tag_fn = reinterpret_cast<fn>(find_pattern("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
	apply_clan_tag_fn(tag, name);
};

void features::misc::clantag_spammer() 
{
	if (!c::misc::misc_clantag_spammer)
	{
		return;
	}

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
	{
		return;
	}

	if (!g::local)
	{
		return;
	}

	static float lasttime = 0.0f;
	static std::string torotate = c::misc::misc_clantag_text;
	static std::string rotating = c::misc::misc_clantag_text;
	float latency = interfaces::engine->get_net_channel_info()->get_latency(FLOW_INCOMING) + interfaces::engine->get_net_channel_info()->get_latency(FLOW_OUTGOING);

	if (c::misc::misc_clantag_type == 0)
	{
		apply_clan_tag(" ", "bhop cheat");
	}
	else if (c::misc::misc_clantag_type == 1)
	{
		apply_clan_tag("\n ", "bhop cheat");
	}
	else if (c::misc::misc_clantag_type == 2)
	{
		apply_clan_tag(std::to_string(g::local->velocity().length_2d()).c_str(), "bhop cheat");
	}
	else if (c::misc::misc_clantag_type == 3)
	{
		apply_clan_tag("[bhop cheat] ", "bhop cheat");
	}
	else if (c::misc::misc_clantag_type == 4)
	{
		if (interfaces::globals->realtime - lasttime < c::misc::misc_clantag_speed)
			return;

		if (torotate != std::string(c::misc::misc_clantag_text)) {
			torotate = c::misc::misc_clantag_text;
			rotating = c::misc::misc_clantag_text;
		}

		if (c::misc::misc_clantag_reverse_rolling) {
			char last = rotating.back();
			rotating.pop_back();
			rotating.insert(rotating.begin(), last);

			apply_clan_tag(rotating.c_str(), torotate.c_str());
		}
		else {
			std::rotate(rotating.begin(), std::next(rotating.begin()), rotating.end());

			apply_clan_tag(rotating.c_str(), torotate.c_str());
		}

		lasttime = interfaces::globals->realtime;
	}
	else if (c::misc::misc_clantag_type == 5) {
		if (interfaces::globals->cur_time * 0.8f + latency != lasttime)
		{
			if (interfaces::globals->realtime - interfaces::globals->cur_time > 1.f)
			{
				switch (int(interfaces::globals->cur_time * 0.8f) % 20)
				{
				case 0: apply_clan_tag("$$$$$$$$$$", "bhop cheat"); break;
				case 1: apply_clan_tag("$$$$$$$$$t", "bhop cheat"); break;
				case 2: apply_clan_tag("$$$$$$$$at", "bhop cheat"); break;
				case 3: apply_clan_tag("$$$$$$$eat", "bhop cheat"); break;
				case 4: apply_clan_tag("$$$$$$heat", "bhop cheat"); break;
				case 5: apply_clan_tag("$$$$$cheat", "bhop cheat"); break;
				case 6: apply_clan_tag("$$$$ cheat", "bhop cheat"); break;
				case 7: apply_clan_tag("$$$p cheat", "bhop cheat"); break;
				case 8: apply_clan_tag("$$op cheat", "bhop cheat"); break;
				case 9: apply_clan_tag("$hop cheat", "bhop cheat"); break;
				case 10: apply_clan_tag("bhop cheat", "bhop cheat"); break;
				case 11: apply_clan_tag("$hop cheat", "bhop cheat"); break;
				case 12: apply_clan_tag("$$op cheat", "bhop cheat"); break;
				case 13: apply_clan_tag("$$$p cheat", "bhop cheat"); break;
				case 14: apply_clan_tag("$$$$ cheat", "bhop cheat"); break;
				case 15: apply_clan_tag("$$$$$cheat", "bhop cheat"); break;
				case 16: apply_clan_tag("$$$$$$heat", "bhop cheat"); break;
				case 17: apply_clan_tag("$$$$$$$eat", "bhop cheat"); break;
				case 18: apply_clan_tag("$$$$$$$$at", "bhop cheat"); break;
				case 19: apply_clan_tag("$$$$$$$$$t", "bhop cheat"); break;
				}
			}

			lasttime = interfaces::globals->cur_time * 0.8f + latency;
		}
	}

	if (g::clantag_update)
	{
		g::clantag_update = false;
	}
}

void features::misc::hitmarker::event(i_game_event* event)
{
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	auto event_name = fnv::hash(event->get_name());
	auto attacker = interfaces::ent_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));
	auto user_id = interfaces::engine->get_player_for_user_id(event->get_int("userid"));

	if (!attacker || !event_name)
	{
		return;
	}

	switch (event_name) 
	{
		case fnv::hash("player_hurt"):
			if (attacker == g::local)
			{
				hitmarker_time = 500;

				if (c::misc::misc_hitmarker_sound)
				{
					switch (c::misc::misc_hitmarker_sound_type)
					{
					case 0: interfaces::surface->play_sound("buttons\\arena_switch_press_02.wav"); break;
					case 1: interfaces::surface->play_sound("buttons\\button22.wav"); break;
					case 2: interfaces::surface->play_sound("survival\\money_collect_01.wav"); break;
					case 3: interfaces::surface->play_sound("Ui\\beep07.wav"); break;
					}
				}				
			}
		break;
	}
}

const char* hitgroup_name(int hitgroup) {
	switch (hitgroup) {
	case hitgroup_head:
		return "head";
	case hitgroup_leftleg:
		return "left leg";
	case hitgroup_rightleg:
		return "right leg ";
	case hitgroup_stomach:
		return "stomach";
	case hitgroup_leftarm:
		return "left arm";
	case hitgroup_rightarm:
		return "right arm";
	default:
		return "body";
	}
}

void features::misc::hit_info(i_game_event* event)
{
	if (!c::misc::misc_hit_info)
	{
		return;
	}

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	if (!event)
	{
		return;
	}

	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player)
	{
		return;
	}

	auto victim = interfaces::engine->get_player_for_user_id(event->get_int("userid"));

	if (!victim)
	{
		return;
	}

	auto attacker = interfaces::ent_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));

	if (!attacker)
	{
		return;
	}

	if (attacker != local_player)
	{
		return;
	}

	auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(victim));

	if (!entity)
	{
		return;
	}

	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);

	hitinfo.damage = event->get_int("dmg_health");

	if (!hitinfo.damage)
	{
		return;
	}

	hitinfo.health = event->get_int("health");

	if (!hitinfo.health && hitinfo.health != 0)
	{
		return;
	}

	auto hitbox = event->get_int("hitgroup");

	if (!hitbox)
	{
		hitinfo.player_name = info.name;

		features::notification::run((std::stringstream{ } << "it " << hitinfo.player_name.c_str() << " for " << hitinfo.damage << " damage (" << hitinfo.health << " health remaining).").str().c_str(), "#_print_hit_1", c::misc::misc_hit_info[2], c::misc::misc_hit_info[0], c::misc::misc_hit_info[1]);

		return;
	}

	hitinfo.hitgroup = hitgroup_name(hitbox);

	hitinfo.player_name = info.name;

	features::notification::run((std::stringstream{ } << "hit " << hitinfo.player_name.c_str() << " in the " << hitinfo.hitgroup << " for " << hitinfo.damage << " damage (" << hitinfo.health << " health remaining).").str().c_str(), "#_print_hit_2", c::misc::misc_hit_info[2], c::misc::misc_hit_info[0], c::misc::misc_hit_info[1]);
}

void features::misc::hitmarker::draw()
{
	if (!c::misc::misc_hitmarker)
	{
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	float hitmarker_size = 5.0f;

	if (hitmarker_time > 0)
	{
		if (hitmarker_time > 100)
		{
			if (hitmarker_size < 8.0f)
			{
				hitmarker_size++;
			}
		}

		im_render.drawline(g::width / 2, g::height / 2, (g::width / 2) + hitmarker_size, (g::height / 2) + hitmarker_size, color_t(255, 255, 255, hitmarker_time), 1.0f);
		im_render.drawline(g::width / 2, g::height / 2, (g::width / 2) - hitmarker_size, (g::height / 2) - hitmarker_size, color_t(255, 255, 255, hitmarker_time), 1.0f);
		im_render.drawline(g::width / 2, g::height / 2, (g::width / 2) + hitmarker_size, (g::height / 2) - hitmarker_size, color_t(255, 255, 255, hitmarker_time), 1.0f);
		im_render.drawline(g::width / 2, g::height / 2, (g::width / 2) - hitmarker_size, (g::height / 2) + hitmarker_size, color_t(255, 255, 255, hitmarker_time), 1.0f);

		hitmarker_time -= 2;
		hitmarker_size -= 1.0f;

		if (hitmarker_size < 1.0f)
		{
			hitmarker_size = 0.0f;
		}
	}
}

void features::misc::force_crosshair()
{
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	if (g::local->is_scoped())
	{
		return;
	}

	if (c::misc::force_crosshair)
	{
		interfaces::console->get_convar("weapon_debug_spread_show")->set_value(3);
		interfaces::console->get_convar("cl_crosshairdot")->set_value(1);
	}
	else
	{
		interfaces::console->get_convar("weapon_debug_spread_show")->set_value(0);
		interfaces::console->get_convar("cl_crosshairdot")->set_value(0);
	}
}

void features::misc::recoil_crosshair()
{
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	if (g::local->is_scoped())
	{
		return;
	}

	if (c::misc::recoil_crosshair)
	{
		interfaces::console->get_convar("cl_crosshair_recoil")->set_value(1);
	}
	else
	{
		interfaces::console->get_convar("cl_crosshair_recoil")->set_value(0);
	}
}

void features::misc::sniper_crosshair() 
{
	if (!c::misc::sniper_crosshair)
	{
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	const auto weapon = g::local->active_weapon();

	if (!weapon)
	{
		return;
	}

	const auto data = interfaces::weapon_system->get_weapon_data(weapon->item_definition_index());

	if (!data)
	{
		return;
	}

	if (data->m_iWeaponType != WEAPONTYPE_SNIPER_RIFLE)
	{
		return;
	}

	if (g::local->is_scoped())
	{
		return;
	}

	im_render.drawrectfilled((g::width / 2) - 2, (g::height / 2) - 2, 2, 2, color_t(255, 255, 255));
}

void features::misc::spectators_list()
{
	if (!c::misc::spectators_list)
	{
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}
	
	std::string name;

	for (int i = 1; i < interfaces::globals->max_clients; i++)
	{
		auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(i));

		if (!entity || entity->is_alive() || entity->dormant())
		{
			continue;
		}

		auto target = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity_handle(entity->observer_target()));

		if (!target || target != g::local)
		{
			continue;
		}

		if (entity == target)
		{
			continue;
		}

		player_info_t spectating_info;
		interfaces::engine->get_player_info(entity->index(), &spectating_info);

		if (spectating_info.ishltv)
		{
			continue;
		}

		std::string mode;

		switch (entity->observer_mode())
		{
		case obs_mode_in_eye:
			mode = "first person";
			break;
		case obs_mode_chase:
			mode = "3rd person";
			break;
		case obs_mode_roaming:
			mode = "noclip";
			break;
		case obs_mode_deathcam:
			mode = "deathcam";
			break;
		case obs_mode_freezecam:
			mode = "freezecam";
			break;
		case obs_mode_fixed:
			mode = "fixed";
			break;
		default:
			mode = "";
			break;
		}

		name += c::misc::spectators_list_mode ? (spectating_info.fakeplayer ? ("[bot] ") + std::string(spectating_info.name) + (" | ") + mode + "\n" : std::string(spectating_info.name) + (" | ") + mode + "\n") : std::string(spectating_info.name);
	}

	if (c::misc::spectators_list_avatars)
	{

	}

	im_render.text(c::misc::spectators_list_avatars ? 25 : 5, features::notification::notify_list.empty() ? 5 : 5 + (features::notification::offset_message * 14.0f), c::fonts::scene_size, fonts::scene_font, name.c_str(), false, color_t(255, 255, 255, 255), c::fonts::scene_flag[9], c::fonts::scene_flag[10]);
}

void features::misc::spread_circle()
{
	if (!c::misc::spread_circle::enable)
	{
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	const auto weapon = g::local->active_weapon();

	if (!weapon)
	{
		return;
	}

	float radius = (weapon->inaccuracy() + weapon->get_spread()) * g::width / 2;

	im_render.drawcircle(g::width / 2, g::height / 2, radius, 64, c::misc::spread_circle::color, 1.0f);
}

void features::misc::big_scene_indicators()
{
	if (!c::misc::debug_information::enable)
	{
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}


	auto aim_text_size = im_render.get_text_size("aim", fonts::debug_information_font, 0.f, c::fonts::debug_information_size);
	auto sp_text_size = im_render.get_text_size("can sp", fonts::debug_information_font, 0.f, c::fonts::debug_information_size);
	auto tp_text_size = im_render.get_text_size("can tp", fonts::debug_information_font, 0.f, c::fonts::debug_information_size);
	auto warn_text_size = im_render.get_text_size("setup sv_cheats at true", fonts::debug_information_font, 0.f, c::fonts::debug_information_size);

	int offset_bottom = 0;

	if (c::misc::debug_information::can_fire::enable && c::aimbot::enable)
	{
		if (c::misc::debug_information::can_fire::style == 1)
		{
			im_render.text((g::width - 5) - aim_text_size, g::height / 2, c::fonts::debug_information_size, fonts::debug_information_font, "aim", false, aimbot::can_fire_checking() ? color_t(c::misc::debug_information::can_fire::active_color, 1.0f) : color_t(c::misc::debug_information::can_fire::inactive_color, 1.0f), c::fonts::debug_information_flag[9], c::fonts::debug_information_flag[10]);

			offset_bottom += c::fonts::debug_information_size + 3;
		}
		else
		{
			im_render.text(g::width / 2, g::height / 2, c::fonts::debug_information_size, fonts::debug_information_font, aimbot::can_fire_checking() ? "can shoot" : "no", true, aimbot::can_fire_checking() ? color_t(c::misc::debug_information::can_fire::active_color, 1.0f) : color_t(c::misc::debug_information::can_fire::inactive_color, 1.0f), c::fonts::debug_information_flag[9], c::fonts::debug_information_flag[10]);
		}
	}

	if (c::misc::debug_information::check_point_system::enable && c::misc::practice)
	{
		if (interfaces::console->get_convar("sv_cheats")->get_int() == 0)
		{
			im_render.text(g::width / 2, g::height / 2, c::fonts::debug_information_size, fonts::debug_information_font, "setup sv_cheats at true!", true, color_t(1.0f, 1.0f, 1.0f, 1.0f), c::fonts::debug_information_flag[9], c::fonts::debug_information_flag[10]);
		}
		else
		{
			im_render.text((g::width - 5) - sp_text_size, (g::height / 2) + offset_bottom, c::fonts::debug_information_size, fonts::debug_information_font, "can sp", false, color_t(1.0f, 1.0f, 1.0f, 1.0f), c::fonts::debug_information_flag[9], c::fonts::debug_information_flag[10]);
			im_render.text((g::width - 5) - tp_text_size, (g::height / 2) + (offset_bottom * 2), c::fonts::debug_information_size, fonts::debug_information_font, "can tp", false, color_t(1.0f, 1.0f, 1.0f, 1.0f), c::fonts::debug_information_flag[9], c::fonts::debug_information_flag[10]);
		}
	}
}

void features::misc::preserve_killfeed()
{
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	static uintptr_t* death_notice = nullptr;

	if (!death_notice)
		death_notice = find_hud_element("CCSGO_HudDeathNotice");

	if (death_notice)
	{
		auto local_death_notice = (float*)((uintptr_t)death_notice + 0x50);

		if (local_death_notice)
			*local_death_notice = c::misc::preverse_killfeed::enable ? c::misc::preverse_killfeed::time : 1.5f;

		if (g::round_start)
		{
			g::round_start = false;

			using Fn = void(__thiscall*)(uintptr_t);
			static auto clear_notices = (Fn)find_pattern_2("client.dll", "55 8B EC 83 EC 0C 53 56 8B 71 58");

			clear_notices((uintptr_t)death_notice - 0x14);
		}
	}

}

void features::misc::bullet_impact::event(i_game_event* event)
{
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	auto event_name = fnv::hash(event->get_name());
	auto user_id = interfaces::engine->get_player_for_user_id(event->get_int("userid"));

	if (!event_name)
	{
		return;
	}

	switch (event_name)
	{
	case fnv::hash("bullet_impact"):
		if (user_id == interfaces::engine->get_local_player())
		{
			vec3_t position(event->get_float("x"), event->get_float("y"), event->get_float("z"));

			server(position);
		}
		break;
	}
}

void features::misc::bullet_impact::client()
{
	if (!c::misc::bullet_impacts::enable || !c::misc::bullet_impacts::client::enable)
	{
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	static auto last_count = INT_MAX;
	auto& client_impact_list = *(CUtlVector<client_hit_data_t>*)((uintptr_t)g::local + 0x11C50);

	for (auto i = client_impact_list.Count(); i > last_count; --i)
	{
		interfaces::debug_overlay->box_overlay(client_impact_list[i - 1].position, vec3_t(-2.0f, -2.0f, -2.0f), vec3_t(2.0f, 2.0f, 2.0f), vec3_t(0.0f, 0.0f, 0.0f), (int)c::misc::bullet_impacts::client::color[0] * 255.0f, (int)c::misc::bullet_impacts::client::color[1] * 255.0f, (int)c::misc::bullet_impacts::client::color[2] * 255.0f, (int)c::misc::bullet_impacts::client::color[3] * 255.0f, c::misc::bullet_tracers::time);
	}

	if (client_impact_list.Count() != last_count)
	{
		last_count = client_impact_list.Count();
	}
}

void features::misc::bullet_impact::server(vec3_t position)
{
	if (!c::misc::bullet_impacts::enable || !c::misc::bullet_impacts::server::enable)
	{
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	interfaces::debug_overlay->box_overlay(position, vec3_t(-2.0f, -2.0f, -2.0f), vec3_t(2.0f, 2.0f, 2.0f), vec3_t(0.0f, 0.0f, 0.0f), (int)c::misc::bullet_impacts::server::color[0] * 255.0f, (int)c::misc::bullet_impacts::server::color[1] * 255.0f, (int)c::misc::bullet_impacts::server::color[2] * 255.0f, (int)c::misc::bullet_impacts::server::color[3] * 255.0f, c::misc::bullet_tracers::time);
}

void features::misc::bullet_impact::trace(vec3_t position)
{

}

void features::misc::penetration_crosshair()
{
	if (!c::misc::penetration_reticle::enable)
	{
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	} 

	color_t color = color_t(255, 0, 0, 255);

	const auto weapon = g::local->active_weapon();

	if (!weapon)
	{
		return;
	}

	const auto data = interfaces::weapon_system->get_weapon_data(weapon->item_definition_index());

	if (!data)
	{
		return;
	}

	if (!aimbot::can_fire_checking())
	{
		return;
	}

	vec3_t view_angles;
	interfaces::engine->get_view_angles(view_angles);

	vec3_t direction;
	math::angle_vectors(view_angles, &direction, nullptr, nullptr);

	trace_t trace;
	trace_filter filter;
	ray_t ray;

	ray.initialize(g::local->get_eye_pos(), g::local->get_eye_pos() + direction * data->m_flWeaponRange);
	filter.skip = g::local;

	interfaces::trace_ray->trace_ray(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &trace);

	if (trace.flFraction < 1.0f) 
	{
		//auto penetration_info = autowall::penetration->run(g::local->get_eye_pos(), g::local->get_eye_pos() + direction * data->m_flWeaponRange, nullptr, true);
		//
		//if (!penetration_info.visible)
		//{
		//	color = color_t(0, 255, 0, 255);
		//}
	}

	im_render.drawrectfilled(g::width / 2 - 1, g::height / 2 - 1, g::width / 2 + 2, g::height / 2 + 2, color);
}

void features::misc::unlock_cvars()
{
	void* v0; 
	int v1; 
	void* v2; 
	void* v3; 
	int Flags;

	v0 = interfaces::console;
	v1 = (*(int(__thiscall**)(void*))(*(DWORD*)v0 + 180))(v0);
	(**(void(__thiscall***)(int))v1)(v1);
	while ((*(unsigned __int8(__thiscall**)(int))(*(DWORD*)v1 + 8))(v1))
	{
		v2 = (void*)(*(int(__thiscall**)(int))(*(DWORD*)v1 + 12))(v1);
		v3 = v2;
		if (v2)
		{
			Flags = *((DWORD*)v2 + 5);
			if ((Flags & 0x12) != 0)
				Flags = Flags & 0x7FFFFFED | 0x80000000;

			int v2; // eax

			v2 = Flags | *((DWORD*)v3 + 2) & 1;
			*((DWORD*)v3 + 5) = Flags;
			*((DWORD*)v3 + 2) = v2;

		}
		(*(void(__thiscall**)(int))(*(DWORD*)v1 + 4))(v1);
	}
}