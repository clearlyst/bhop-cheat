#include "aimbot.hpp"
#include "../../sdk/math/math.hpp"
#include "../../menu/config/config.hpp"

c::aimbot::aimbot_value_t aimbot::get_current_settings()
{
	const auto general_settings = c::aimbot::general;
	const auto pistol_settings = c::aimbot::pistols;
	const auto pistol_heavy_settings = c::aimbot::heavy_pistols;
	const auto smgs_settings = c::aimbot::smg;
	const auto shotguns_settings = c::aimbot::shotguns;
	const auto rifle_settings = c::aimbot::rifles;
	const auto sniper_settings = c::aimbot::snipers;

	const auto active_weapon = g::local->active_weapon();

	if (!active_weapon)
	{
		return general_settings;
	}

	const auto weapon_type = active_weapon->get_weapon_type();

	if (!weapon_type)
	{
		return general_settings;
	}

	const auto is_pistol = weapon_type == WEAPON_TYPE_PISTOL;
	const auto is_heavy_pistol = weapon_type == WEAPON_TYPE_HEAVYPISTOL;
	const auto is_smg = weapon_type == WEAPON_TYPE_SMG;
	const auto is_shotgun = weapon_type == WEAPON_TYPE_SHOTGUN;
	const auto is_rifle = weapon_type == WEAPON_TYPE_RIFLE;
	const auto is_sniper = weapon_type == WEAPON_TYPE_SNIPER;

	if (is_pistol && pistol_settings.override_general)
	{
		return pistol_settings;
	}

	if (is_heavy_pistol && pistol_heavy_settings.override_general)
	{
		return pistol_heavy_settings;
	}

	if (is_smg && smgs_settings.override_general)
	{
		return smgs_settings;
	}

	if (is_shotgun && shotguns_settings.override_general)
	{
		return shotguns_settings;
	}

	if (is_rifle && rifle_settings.override_general)
	{
		return rifle_settings;
	}

	if (is_sniper && sniper_settings.override_general)
	{
		return sniper_settings;
	}

	return general_settings;
}

bool can_fire(weapon_t* weap, c_usercmd* cmd) 
{
	float flServerTime = (float)(g::local->get_tick_base() * interfaces::globals->interval_per_tick);

	if (g::local->next_attack() < flServerTime)
	{
		return (weap->next_primary_attack() < flServerTime);
	}

	return false;
}

bool aimbot::hitscan(vec3_t eye_pos, player_t* player, vec3_t& ret_hitbox, float minimum_damage, float minimum_damage_visible) {
	float mindmg = min(minimum_damage, player->health());
	float mindmgvis = min(minimum_damage_visible, player->health());
	float currdmg = 0;
	vec3_t currhitbox = vec3_t{};

	std::vector<int> hitbox_ints;

	for (auto hitbox : hitboxes)
	{
		hitbox_ints.push_back(hitbox);
	}

	std::vector<vec3_t> hitbox_points, head_multipoint, all_points;
	if (hitbox_ints.size()) {
		for (auto muie_snek : hitbox_ints) {
			hitbox_points.push_back(player->get_hitbox_position(muie_snek));
		}
		for (auto cacat : hitbox_points) {
			all_points.push_back(cacat);
		}
	}

	bool visible = false;
	if (all_points.size()) {
		for (auto hitbox : all_points) {
			float damage = autowall::damage(hitbox);
			if (damage > currdmg) {
				currdmg = damage;
				currhitbox = hitbox;
				visible = false;
				if (g::local->can_see_player_pos(player, hitbox))
					visible = true;
			}
		}

		float bestdmg = visible ? mindmgvis : mindmg;
		if (currdmg > bestdmg && currhitbox.x != 0.f && currhitbox.y != 0.0f) {
			ret_hitbox = currhitbox;
			return true;
		}
	}

	return false;
}

vec3_t calculate_angle(const vec3_t& source, const vec3_t& destination, const vec3_t& viewangles) {
	vec3_t delta = source - destination;
	vec3_t angles;
	angles.x = math::rad2deg(atanf(delta.z / std::hypotf(delta.x, delta.y))) - viewangles.x;
	angles.y = math::rad2deg(atanf(delta.y / delta.x)) - viewangles.y;
	angles.z = 0.0f;

	if (delta.x >= 0.0)
		angles.y += 180.0f;

	angles.normalize();
	return angles;
}

static inline int aimbone;
int get_nearest_bone(player_t* entity, c_usercmd* user_cmd) {
	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return false;

	float best_dist = 360.f;

	matrix_t matrix[MAXSTUDIOBONES];

	if (!entity->setup_bones(matrix, 128, BONE_USED_BY_HITBOX, 0.0f))
		return -1;

	studio_hdr_t* studio_model = interfaces::model_info->get_studio_model(entity->model());
	if (!studio_model)
		return -1;

	studio_hitbox_set_t* set = studio_model->hitbox_set(entity->hitbox_set());
	if (!set)
		return -1;

	for (int i = 0; i < set->hitbox_count; i++) {
		if (i >= hitbox_max)
			continue;

		studio_box_t* hitbox = set->hitbox(i);

		if (!hitbox)
			continue;

		vec3_t angle = calculate_angle(local_player->get_eye_pos(), vec3_t(matrix[hitbox->bone][0][3], matrix[hitbox->bone][1][3], matrix[hitbox->bone][2][3]), user_cmd->view_angles);
		float this_dist = std::hypotf(angle.x, angle.y);

		if (best_dist > this_dist) {
			best_dist = this_dist;
			aimbone = hitbox->bone;
			continue;
		}
	}
	return aimbone;
}

player_t* aimbot::get_best_target(c_usercmd* cmd)
{
	const auto current_settings = get_current_settings();

    float ofov = current_settings.fov;
    float nfov = 0;
    player_t* target = nullptr;

    for (int i = 0; i < interfaces::globals->max_clients; i++) {
        auto player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(i));

        if (!player || player->dormant() || player == g::local || player->team() == g::local->team() ||
            !(player->is_alive() && player->health() > 0) || player->has_gun_game_immunity())
			continue;

        vec3_t eyepos = g::local->get_eye_pos();

		for (auto hitbox : hitboxes)
		{
			vec3_t position = player->get_hitbox_position(hitbox);

            auto entity_bone_pos = player->get_bone_position(get_nearest_bone(player, cmd));
            auto angle = calculate_angle(g::local->get_eye_pos(), entity_bone_pos, cmd->view_angles);
            nfov = std::hypotf(angle.x, angle.y);
	        
            if (nfov < ofov)
		    {
                if (current_settings.autowall && hitscan(eyepos, player, position, current_settings.autowall_damage, 1.f))
		    	{
                    ofov = nfov;
                    target = player;
                }
                else if (!current_settings.autowall && g::local->can_see_player_pos(player, player->get_eye_pos()))
		    	{
                    ofov = nfov;
                    target = player;
                }
            }
		}
    }
    return target;
}

enum Hitboxes
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_STOMACH,
	HITBOX_LOWER_CHEST,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX
};

void aimbot::run(c_usercmd* cmd) 
{
	const auto current_settings = get_current_settings();

	if (!c::aimbot::enable)
	{
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive() || g::local->is_defusing())
	{
		return;
	}

	if (menu::open)
	{
		return;
	}

	auto weapon = g::local->active_weapon();

	if (!weapon)
	{
		return;
	}

	auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
	{
		return;
	}

	if (current_settings.autopistol)
	{
		if (weapon->get_weapon_data()->m_iWeaponType == WEAPONTYPE_PISTOL)
		{
			const float server_time = g::local->get_tick_base() * interfaces::globals->interval_per_tick;
			const float next_shot = weapon->next_primary_attack() - server_time;

			if (next_shot > 0)
			{
				if (weapon->item_definition_index() == WEAPON_REVOLVER)
				{
					cmd->buttons &= ~in_attack2;
				}
				else
				{
					cmd->buttons &= ~in_attack;
				}
			}
		}
	}

	if (!(can_fire(weapon, cmd)))
	{
		return;
	}

	if (c::aimbot::shoot_at_aim_key ? cmd->buttons & in_attack : menu::checkkey(c::aimbot::key, c::aimbot::key_s))
	{
		hitboxes.clear();

		if (current_settings.head)
		{
			hitboxes.push_back(HITBOX_HEAD);
		}

		if (current_settings.neck)
		{
			hitboxes.push_back(HITBOX_NECK);
		}

		if (current_settings.chest)
		{
			hitboxes.push_back(HITBOX_UPPER_CHEST);
			hitboxes.push_back(HITBOX_CHEST);
			hitboxes.push_back(HITBOX_LOWER_CHEST);
		}

		if (current_settings.stomach)
		{
			hitboxes.push_back(HITBOX_STOMACH);
		}

		if (current_settings.pelvis)
		{
			hitboxes.push_back(HITBOX_PELVIS);
		}

		if (current_settings.foot)
		{
			hitboxes.push_back(HITBOX_LEFT_FOOT);
			hitboxes.push_back(HITBOX_RIGHT_FOOT);
		}

		if (hitboxes.empty())
		{
			return;
		}

	    player_t* target = get_best_target(cmd);
	    static auto weapon_recoil_scale = interfaces::console->get_convar("weapon_recoil_scale")->get_float();
	    
	    if (target)
		{
			vec3_t eyepos = g::local->get_eye_pos();
			vec3_t position;

			for (auto hitbox : hitboxes)
			{
				if (c::backtrack::enable && c::aimbot::aim_at_backtrack)
				{
					int bone_id = 8;
					auto& record = backtrack.data[target->index()];

					if (auto modelStudio = interfaces::model_info->get_studio_model(target->model()); modelStudio != nullptr)
					{
						auto hitboxSet = modelStudio->hitbox_set(0);

						if (hitboxSet != nullptr)
						{
							auto hitbox_ = hitboxSet->hitbox(hitbox);

							if (hitbox_ != nullptr)
							{
								bone_id = hitbox_->bone;
							}
						}
					}

					float best_record = FLT_MAX;
					int best_index = -1;

					for (int j = 0; j < static_cast<int>(record.size()) - 2; j++)
					{
						auto& a = record[j];
						auto angle_a = calculate_angle(g::local->get_eye_pos(), a.m_matrix[bone_id].get_origin(), cmd->view_angles);
						auto hypot_a = angle_a.length_2d();

						if (hypot_a < best_record)
						{
							best_record = hypot_a;
							best_index = j;
						}
					}

					if (best_index != -1)
					{
						position = record[best_index].m_matrix[bone_id].get_origin();
					}
					else
					{
						position = target->get_hitbox_position(hitbox);
					}
				}
				else
				{
					position = target->get_hitbox_position(hitbox);
				}
			}

			vec3_t viewangles = math::calc_angle(eyepos, position);
			viewangles.clamp();
			vec3_t delta = cmd->view_angles - viewangles;
			delta.clamp();
			vec3_t finalang = cmd->view_angles - delta;
			finalang.clamp();

			if (current_settings.recoil_compensate)
			{
				cmd->view_angles = finalang - (g::local->aim_punch_angle() * weapon_recoil_scale);
			}
			else
			{
				cmd->view_angles = finalang;
			}

			if (!current_settings.silent)
			{
				interfaces::engine->set_view_angles(cmd->view_angles);
			}
		}
	}
}
