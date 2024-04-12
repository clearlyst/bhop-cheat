#include "misc.hpp"
#include "../../menu/menu.hpp"
#include "../../includes/imgui/imgui.h"
#include "../../includes/imgui/imgui_internal.h"
#include "../../features/visuals/visuals.hpp"
#include "../../features/notifications/notifications.hpp"
#include "../../sdk/sdk.hpp"
#include "../../sdk/steam/steamclientpublic.h"

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

std::deque<std::pair<vec3_t, vec3_t>> checkpoints;

int current_checkpoint = 0;
float press_delay = 0.0f;

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

	static bool reset = true;
	static auto lasttime = 0.0f;
	static std::string torotate = c::misc::misc_clantag_text;
	static std::string rotating = c::misc::misc_clantag_text;

	if (c::misc::misc_clantag_type == 0)
	{
		if (interfaces::globals->realtime - lasttime < 1.0f)
		{
			return;
		}

		apply_clan_tag(xs("ozungaware "), xs("ozungaware "));

		lasttime = interfaces::globals->realtime;
	}
	else if (c::misc::misc_clantag_type == 1) {
		if (c::misc::misc_animated_clantag) {
			if (interfaces::globals->realtime - lasttime < c::misc::misc_clantag_speed)
				return;

			if (torotate != std::string(c::misc::misc_clantag_text)) {
				torotate = c::misc::misc_clantag_text;
				rotating = c::misc::misc_clantag_text;
			}

			if (c::misc::misc_clantag_rotation) {
				if (!rotating.empty()) {
					char last = rotating.back();
					rotating.pop_back();
					rotating.insert(rotating.begin(), last);
					apply_clan_tag(rotating.c_str(), torotate.c_str());
				}
			}
			else {
				if (!rotating.empty()) {
					std::rotate(rotating.begin(), rotating.begin() + (rotating.size() - 1), rotating.end());
					apply_clan_tag(rotating.c_str(), torotate.c_str());
				}
			}

			lasttime = interfaces::globals->realtime;
		}
	}
}

int hitmarker_time = 0;

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
	//auto user_id = interfaces::engine->get_player_for_user_id(event->get_int("userid"));

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
				/*
			if (c::misc::misc_hitchams)
			{
				const auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(user_id));

				if (!entity)
					return;

				if (!entity->index())
					return;

				matrix_t bone_matrix[MAXSTUDIOBONES];

				if (!entity->setup_bones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, interfaces::globals->cur_time))
					return;

				studiohdr_t* studio_hdr = g_MdlInfo->GetStudiomodel(entity->GetModel());

				if (!studio_hdr)
					return;

				const auto hitbox_set = studio_hdr->GetHitboxSet(entity->hitbox_set());

				if (!hitbox_set)
					return;

				for (int i = 0; i < hitbox_set->numhitboxes; i++)
				{
					mstudiobbox_t* hitbox = hitbox_set->GetHitbox(i);

					if (!hitbox)
						continue;

					vec3_t min_actual, max_actual;

					math::vector_transform(hitbox->bbmin, bone_matrix[hitbox->bone], min_actual);
					math::vector_transform(hitbox->bbmax, bone_matrix[hitbox->bone], max_actual);

					interfaces::debug_overlay->add_capsule_overlay(min_actual, max_actual, hitbox->m_flRadius, color_t(c::misc::misc_hitchams_color), 2.5f);
				}*/
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
	}
	else
	{
		interfaces::console->get_convar("weapon_debug_spread_show")->set_value(0);
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

enum spectators_mode
{

	OBS_MODE_NONE,
	OBS_MODE_DEATHCAM,
	OBS_MODE_FREEZECAM,
	OBS_MODE_FIXED,
	OBS_MODE_IN_EYE,
	OBS_MODE_CHASE,
	OBS_MODE_ROAMING,
};

void CopyConvert(const uint8_t* rgba, uint8_t* out, const size_t size)
{
	auto in = reinterpret_cast<const uint32_t*>(rgba);
	auto buf = reinterpret_cast<uint32_t*>(out);
	for (auto i = 0u; i < (size / 4); ++i)
	{
		const auto pixel = *in++;
		*buf++ = (pixel & 0xFF00FF00) | ((pixel & 0xFF0000) >> 16) | ((pixel & 0xFF) << 16);
	}
}

LPDIRECT3DTEXTURE9 SteamImageBySteamID(CSteamID steam)
{
	LPDIRECT3DTEXTURE9 asdgsdgadsg;

	int iImage = interfaces::steam_friend->get_large_friend_avatar(steam);
	if (iImage == -1)
		return nullptr;

	uint32 uAvatarWidth, uAvatarHeight;
	if (!interfaces::steam_utils->get_image_size(iImage, &uAvatarWidth, &uAvatarHeight))
		return nullptr;

	const int uImageSizeInBytes = uAvatarWidth * uAvatarHeight * 4;
	uint8* pAvatarRGBA = new uint8[uImageSizeInBytes];

	if (!interfaces::steam_utils->get_image_rgba(iImage, pAvatarRGBA, uImageSizeInBytes))
	{
		delete[] pAvatarRGBA;
		return nullptr;
	}

	auto res = interfaces::device->CreateTexture(uAvatarWidth, uAvatarHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &asdgsdgadsg, nullptr);

	std::vector<uint8_t> texData;
	texData.resize(uAvatarWidth * uAvatarHeight * 4u);
	CopyConvert(pAvatarRGBA, texData.data(), uAvatarWidth * uAvatarHeight * 4u);

	D3DLOCKED_RECT rect;
	res = asdgsdgadsg->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD);
	auto src = texData.data();
	auto dst = reinterpret_cast<uint8_t*>(rect.pBits);
	for (auto y = 0u; y < uAvatarHeight; ++y)
	{
		std::copy(src, src + (uAvatarWidth * 4), dst);
		src += uAvatarWidth * 4;
		dst += rect.Pitch;
	}

	res = asdgsdgadsg->UnlockRect(0);
	delete[] pAvatarRGBA;

	return asdgsdgadsg;
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
		case OBS_MODE_IN_EYE:
			mode = "first person";
			break;
		case OBS_MODE_CHASE:
			mode = "3rd person";
			break;
		case OBS_MODE_ROAMING:
			mode = "noclip";
			break;
		case OBS_MODE_DEATHCAM:
			mode = "deathcam";
			break;
		case OBS_MODE_FREEZECAM:
			mode = "freezecam";
			break;
		case OBS_MODE_FIXED:
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