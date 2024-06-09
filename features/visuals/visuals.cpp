#include "../visuals/visuals.hpp"
#include "../../menu/config/config.hpp"
#include "../../menu/menu.hpp"

int rgba_to_int(float red, float green, float blue, float alpha)
{
	int r = static_cast<int>(red * 255.0f) & 0xFF;
	int g = static_cast<int>(green * 255.0f) & 0xFF;
	int b = static_cast<int>(blue * 255.0f) & 0xFF;
	int a = static_cast<int>(alpha * 255.0f) & 0xFF;

	return (a << 24 | b << 16 | g << 8 | r);
}

void features::visuals::particles()
{
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
	{
		return;
	}

	for (int i = 1; i <= interfaces::ent_list->get_highest_index(); i++)
	{
		auto entity = reinterpret_cast<entity_t*>(interfaces::ent_list->get_client_entity(i));

		if (!entity || entity->is_player())
			continue;

		const auto client_class = entity->client_class();

		if (!client_class)
			continue;

		[&]() {
			if (client_class->class_id != C_FOG_CONTROLLER)
				return;

			color_t color(c::visuals::world::fog::color);

			entity->enable() = c::visuals::world::fog::enable ? true : false;
			entity->start() = c::visuals::world::fog::start;
			entity->end() = c::visuals::world::fog::end;
			entity->density() = c::visuals::world::fog::density;
			entity->color_primary() = rgba_to_int(color.r(), color.g(), color.b(), color.a());
			entity->color_secondary() = rgba_to_int(color.r(), color.g(), color.b(), color.a());

			if (!c::visuals::world::fog::enable)
				return;
		}();

		[&]() {
			if (!c::visuals::world::shadow::enable)
				return;

			if (client_class->class_id != class_ids::C_CASCADE_LIGHT)	
				return;

			entity->shadow_direction() = vec3_t(c::visuals::world::shadow::x, c::visuals::world::shadow::y, c::visuals::world::shadow::z);
			entity->light_shadow_direction() = vec3_t(c::visuals::world::shadow::x, c::visuals::world::shadow::y, c::visuals::world::shadow::z);
		}();

		[&]() {
			if (client_class->class_id != class_ids::C_ENV_TONEMAP_CONTROLLER)
				return;

			entity->use_custom_bloom_scale() = true;
			entity->custom_bloom_scale() = c::visuals::world::bloom::enable ? c::visuals::world::bloom::scale : 0.0f;

			entity->use_custom_auto_exposure_min() = c::visuals::world::bloom::enable ? true : false;
			entity->custom_auto_exposure_min() = c::visuals::world::bloom::exposure_min;

			entity->use_custom_auto_exposure_max() = c::visuals::world::bloom::enable ? true : false;
			entity->custom_auto_exposure_max() = c::visuals::world::bloom::exposure_max;

			if (!c::visuals::world::bloom::enable)
				return;
		}();
	}

	static auto mat_force_tonemap_scale = interfaces::console->get_convar("mat_force_tonemap_scale")->get_float();
	static auto mat_fullbright = interfaces::console->get_convar("mat_fullbright")->get_float();
	static auto r_3dsky = interfaces::console->get_convar("r_3dsky")->get_float();
	static auto r_aspectratio = interfaces::console->get_convar("r_aspectratio")->get_float();

	static bool* disable_postprocessing = *reinterpret_cast<bool**>(find_pattern("client.dll", "83 EC 4C 80 3D") + 0x5);
	*disable_postprocessing = c::visuals::post_processing::enable;

	interfaces::console->get_convar("mat_force_tonemap_scale")->callbacks.clear();
	interfaces::console->get_convar("mat_fullbright")->callbacks.clear();
	interfaces::console->get_convar("r_3dsky")->callbacks.clear();
	interfaces::console->get_convar("r_aspectratio")->callbacks.clear();

	if (c::visuals::world::brightness::enable)
	{
		interfaces::console->get_convar("mat_force_tonemap_scale")->set_value(c::visuals::world::brightness::amount);
	}
	else
	{
		interfaces::console->get_convar("mat_force_tonemap_scale")->set_value(mat_force_tonemap_scale);
	}

	if (c::visuals::world::fullbright::enable)
	{
		interfaces::console->get_convar("mat_fullbright")->set_value(1);
	}
	else
	{
		interfaces::console->get_convar("mat_fullbright")->set_value(mat_fullbright);
	}

	if (c::visuals::world::skybox::enable)
	{
		interfaces::console->get_convar("r_3dsky")->set_value(0);
	}
	else
	{
		interfaces::console->get_convar("r_3dsky")->set_value(r_3dsky);
	}

	if (c::misc::sv_cheats::enable)
	{
		interfaces::console->get_convar("sv_cheats")->set_value(1);
	}

	if (c::visuals::world::aspect_ratio_modulation::enable)
	{
		interfaces::console->get_convar("r_aspectratio")->set_value(c::visuals::world::aspect_ratio_modulation::amount);
	}
	else
	{
		interfaces::console->get_convar("r_aspectratio")->set_value(r_aspectratio);
	}
}

void features::visuals::entities_ragdoll()
{
	if (!c::visuals::world::ragdoll::enable)
	{
		return;
	}

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
	{
		return;
	}

	for (int i = 1; i <= interfaces::ent_list->get_highest_index(); i++)
	{
		auto entity = reinterpret_cast<entity_t*>(interfaces::ent_list->get_client_entity(i));

		if (!entity)
			continue;

		if (entity->dormant())
			continue;

		const auto client_class = entity->client_class();

		if (!client_class)
			continue;

		if (client_class->class_id != C_CS_RAGDOLL)
			continue;


		switch (c::visuals::world::ragdoll::style)
		{
			case 0: entity->force_ragdoll() *= 0, entity->ragdoll_velocity() *= 0; break;
			case 1: entity->force_ragdoll() *= INT_MAX, entity->ragdoll_velocity() *= 0; break;
			case 2: entity->force_ragdoll() *= INT_MIN, entity->ragdoll_velocity() *= 0; break;
			case 3: entity->force_ragdoll().z *= INT_MAX, entity->ragdoll_velocity().z *= INT_MAX;  break;
		}
	}
}

void features::visuals::skybox_changer(client_frame_stage_t stage)
{
	static auto load_skybox = reinterpret_cast<void(__fastcall*)(const char*)>(find_pattern("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));

	if (!c::visuals::world::skybox::enable)
	{
		return;
	}

	if (stage != frame_render_start && stage != frame_render_end)
	{
		return;
	}

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
	{
		return;
	}

	constexpr std::array skybox_list{ "default", "cs_baggage_skybox_", "cs_tibet", "embassy", "italy", "jungle", "nukeblank", "office", "sky_cs15_daylight01_hdr", "sky_cs15_daylight02_hdr", "sky_cs15_daylight03_hdr", "sky_cs15_daylight04_hdr", "sky_csgo_cloudy01", "sky_csgo_night_flat", "sky_csgo_night02", "sky_day02_05_hdr", "sky_day02_05", "sky_dust", "sky_l4d_rural02_ldr", "sky_venice", "vertigo_hdr", "vertigo", "vertigoblue_hdr", "vietnam", "sky_lunacy", "sky_hr_aztec" };

	if (stage == frame_render_start && c::visuals::world::skybox::style > 0 && static_cast<std::size_t>(c::visuals::world::skybox::style) < skybox_list.size())
	{
		load_skybox(skybox_list[c::visuals::world::skybox::style]);
	}
	else {
		static const auto sv_skyname = interfaces::console->get_convar("sv_skyname");
		load_skybox(sv_skyname->get_string());
	}
}

void features::visuals::flashalpha()
{
	static auto flash = interfaces::material_system->find_material("effects/flashbang", TEXTURE_GROUP_CLIENT_EFFECTS);
	static auto flash_white = interfaces::material_system->find_material("effects/flashbang_white", TEXTURE_GROUP_CLIENT_EFFECTS);

	if (!c::visuals::flash::enable)
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

	g::local->flash_alpha() = 255.0f - c::visuals::flash::alpha * 2.55f;

	flash->set_material_var_flag(material_var_no_draw, c::visuals::flash::full);
	flash_white->set_material_var_flag(material_var_no_draw, c::visuals::flash::full);
}

void features::visuals::nosmoke()
{
	static auto linegoesthrusmoke = find_pattern("client.dll", "A3 ? ? ? ? 57 8B CB");
	static bool set = true;

	std::vector<const char*> material_of_smoke =
	{
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	if (!c::visuals::smoke::enable)
	{
		if (set)
		{
			for (auto material_name : material_of_smoke)
			{
				i_material* smoke = interfaces::material_system->find_material(material_name, TEXTURE_GROUP_OTHER);
				smoke->set_material_var_flag(material_var_wireframe, false);
			}
			set = false;
		}

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

	set = true;

	for (auto mat_s : material_of_smoke)
	{
		i_material* smoke = interfaces::material_system->find_material(mat_s, TEXTURE_GROUP_OTHER);
		smoke->set_material_var_flag(material_var_wireframe, true);
	}

	static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x1);
	*(int*)(smokecout) = 0;
}

void features::visuals::draw_scope()
{
	if (!c::visuals::scope::enable)
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

	if (!g::local->is_scoped())
	{
		return;
	}

	im_render.drawline(0, g::height / 2, g::width, g::height / 2, color_t(c::visuals::scope::color));
	im_render.drawline(g::width / 2, 0, g::width / 2, g::height, color_t(c::visuals::scope::color));
}

void features::visuals::dlights(player_t* entity) 
{
	vec3_t getorig = entity->origin();
	vec3_t getheadorig = entity->get_eye_pos();

	dlight_t* elight = interfaces::effects->cl_alloc_elight(entity->index());
	elight->color.r = float(c::visuals::players::dlight::color[0] * 255.f);
	elight->color.g = float(c::visuals::players::dlight::color[1] * 255.f);
	elight->color.b = float(c::visuals::players::dlight::color[2] * 255.f);
	elight->color.exponent = 8.f;
	elight->direction = getheadorig;
	elight->origin = getheadorig;
	elight->radius = c::visuals::players::dlight::radius;
	elight->die_time = interfaces::globals->cur_time + 0.1f;
	elight->decay = 50.0f;		
	elight->key = entity->index();

	dlight_t* dlight = interfaces::effects->cl_alloc_dlight(entity->index());
	dlight->color.r = float(c::visuals::players::dlight::color[0] * 255.f);
	dlight->color.g = float(c::visuals::players::dlight::color[1] * 255.f);
	dlight->color.b = float(c::visuals::players::dlight::color[2] * 255.f);
	dlight->color.exponent = 8.f;
	dlight->direction = getorig;
	dlight->origin = getorig;
	dlight->radius = c::visuals::players::dlight::radius / 2;
	dlight->die_time = interfaces::globals->cur_time + 0.1f;
	dlight->decay = dlight->radius / 2.f;		
	dlight->key = entity->index();
}

static int buttons = 0;

void features::visuals::run_freecam(c_usercmd* cmd, vec3_t angles) {
	static vec3_t currentviewangles = vec3_t{};
	static vec3_t realviewangles = vec3_t{};
	static bool wascrouching = false;
	static bool washoldingattack = false;
	static bool washoldinguse = false;
	static bool hassetangles = false;
	buttons = cmd->buttons;

	if (!c::misc::freecam || !menu::checkkey(c::misc::freecam_key, c::misc::freecam_key_s)) {
		if (hassetangles) {
			interfaces::engine->set_view_angles(realviewangles);
			cmd->view_angles = currentviewangles;
			if (wascrouching)
				cmd->buttons |= in_duck;
			if (washoldingattack)
				cmd->buttons |= in_attack;
			if (washoldinguse)
				cmd->buttons |= in_use;
			wascrouching = false;
			washoldingattack = false;
			washoldinguse = false;
			hassetangles = false;
		}
		currentviewangles = vec3_t{};
		return;
	}

	if (!g::local || !g::local->is_alive())
		return;

	if (currentviewangles.null()) {
		currentviewangles = cmd->view_angles;
		realviewangles = angles;
		wascrouching = cmd->buttons & in_duck;
	}

	cmd->forward_move = 0;
	cmd->side_move = 0;
	cmd->buttons = 0;

	if (wascrouching)
		cmd->buttons |= in_duck;

	if (washoldingattack)
		cmd->buttons |= in_attack;

	if (washoldinguse)
		cmd->buttons |= in_use;

	cmd->view_angles = currentviewangles;
	hassetangles = true;
}

void features::visuals::freecam(view_setup_t* setup) {
	static vec3_t origin = vec3_t{ };

	if (!c::misc::freecam || !GetAsyncKeyState(c::misc::freecam_key)) {
		origin = vec3_t{ };
		return;
	}

	if (!g::local || !g::local->is_alive())
		return;

	float cam_speed = fabsf(static_cast<float>(2)); // cfg later 

	if (origin.null())
		origin = setup->origin;

	vec3_t forward{ }, right{ }, up{ };

	math::angle_vectors_alternative(setup->view, &forward, &right, &up);

	const bool inback = buttons & in_back;
	const bool inforward = buttons & in_forward;
	const bool rightBtn = buttons & in_moveright;
	const bool inleft = buttons & in_moveleft;
	const bool inshift = buttons & in_speed;
	const bool induck = buttons & in_duck;
	const bool injump = buttons & in_jump;

	if (induck)
		cam_speed *= 0.45f;

	if (inshift)
		cam_speed *= 1.65f;

	if (inforward)
		origin += forward * cam_speed;

	if (rightBtn)
		origin += right * cam_speed;

	if (inleft)
		origin -= right * cam_speed;

	if (inback)
		origin -= forward * cam_speed;

	if (injump)
		origin += up * cam_speed;

	setup->origin = origin;
}

void draw_screen_effect(i_material* material) {
	static auto fn = find_pattern("client.dll", "55 8B EC 83 E4 ? 83 EC ? 53 56 57 8D 44 24 ? 89 4C 24 ?");
	int w, h;
	interfaces::engine->get_screen_size(w, h);
	__asm {
		push h
		push w
		push 0
		xor edx, edx
		mov ecx, material
		call fn
		add esp, 12
	}
}

struct motion_blur_history {
	motion_blur_history() {
		last_time_update = 0.0f;
		previous_pitch = 0.0f;
		previous_yaw = 0.0f;
		previous_pos = vec3_t{ 0.0f, 0.0f, 0.0f };
		no_rotational_mb_until = 0.0f;
	}

	float last_time_update;
	float previous_pitch;
	float previous_yaw;
	vec3_t previous_pos;
	float no_rotational_mb_until;
};

void features::visuals::motion_blur(view_setup_t* setup)
{
	if (!c::visuals::world::motionblur::enable)
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

	static motion_blur_history history;
	static float motion_blur_values[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (setup)
	{
		const float time_elapsed = interfaces::globals->realtime - history.last_time_update;

		const auto view_angles = setup->view;

		float current_pitch = view_angles.x;

		while (current_pitch > 180.0f)
			current_pitch -= 360.0f;
		while (current_pitch < -180.0f)
			current_pitch += 360.0f;

		float current_yaw = view_angles.y;

		while (current_yaw > 180.0f)
			current_yaw -= 360.0f;
		while (current_yaw < -180.0f)
			current_yaw += 360.0f;

		vec3_t current_side_vector;
		vec3_t current_forward_vector;
		vec3_t current_up_vector;
		math::angle_vectors(setup->view, &current_forward_vector, &current_side_vector, &current_up_vector);

		vec3_t current_position = setup->origin;
		vec3_t position_change = history.previous_pos - current_position;

		if ((position_change.length() > 30.0f) && (time_elapsed >= 0.5f))
		{
			motion_blur_values[0] = 0.0f;
			motion_blur_values[1] = 0.0f;
			motion_blur_values[2] = 0.0f;
			motion_blur_values[3] = 0.0f;
		}
		else if (time_elapsed > (1.0f / 15.0f))
		{
			motion_blur_values[0] = 0.0f;
			motion_blur_values[1] = 0.0f;
			motion_blur_values[2] = 0.0f;
			motion_blur_values[3] = 0.0f;
		}
		else if (position_change.length() > 50.0f)
		{
			history.no_rotational_mb_until = interfaces::globals->realtime + 1.0f;
		}
		else
		{
			const float horizontal_fov = setup->fov;
			const float vertical_fov = (setup->aspect_ratio <= 0.0f) ? (setup->fov) : (setup->fov / setup->aspect_ratio);
			const float viewdot_motion = current_forward_vector.dot_product(position_change);

			if (c::visuals::world::motionblur::forward_move_blur)
			{
				motion_blur_values[2] = viewdot_motion;
			}

			const float sidedot_motion = current_side_vector.dot_product(position_change);
			float yawdiff_original = history.previous_yaw - current_yaw;

			if (((history.previous_yaw - current_yaw > 180.0f) || (history.previous_yaw - current_yaw < -180.0f)) && ((history.previous_yaw + current_yaw > -180.0f) && (history.previous_yaw + current_yaw < 180.0f)))
			{
				yawdiff_original = history.previous_yaw + current_yaw;
			}

			float yawdiff_adjusted = yawdiff_original + (sidedot_motion / 3.0f);

			if (yawdiff_original < 0.0f)
			{
				yawdiff_adjusted = std::clamp(yawdiff_adjusted, yawdiff_original, 0.0f);
			}
			else
			{
				yawdiff_adjusted = std::clamp(yawdiff_adjusted, 0.0f, yawdiff_original);
			}

			const float undampened_yaw = yawdiff_adjusted / horizontal_fov;
			motion_blur_values[0] = undampened_yaw * (1.0f - (fabsf(current_pitch) / 90.0f));

			const float pitch_compensate_mask = 1.0f - ((1.0f - fabsf(current_forward_vector[2])) * (1.0f - fabsf(current_forward_vector[2])));
			const float pitchdiff_original = history.previous_pitch - current_pitch;
			float pitchdiff_adjusted = pitchdiff_original;

			if (current_pitch > 0.0f)
			{
				pitchdiff_adjusted = pitchdiff_original - ((viewdot_motion / 2.0f) * pitch_compensate_mask);
			}
			else
			{
				pitchdiff_adjusted = pitchdiff_original + ((viewdot_motion / 2.0f) * pitch_compensate_mask);
			}


			if (pitchdiff_original < 0.0f)
			{
				pitchdiff_adjusted = std::clamp(pitchdiff_adjusted, pitchdiff_original, 0.0f);
			}
			else
			{
				pitchdiff_adjusted = std::clamp(pitchdiff_adjusted, 0.0f, pitchdiff_original);
			}

			motion_blur_values[1] = pitchdiff_adjusted / vertical_fov;
			motion_blur_values[3] = undampened_yaw;
			motion_blur_values[3] *= (fabs(current_pitch) / 90.0f) * (fabs(current_pitch) / 90.0f) * (fabs(current_pitch) / 90.0f);

			if (time_elapsed > 0.0f)
			{
				motion_blur_values[2] /= time_elapsed * 30.0f;
			}
			else
			{
				motion_blur_values[2] = 0.0f;
			}

			motion_blur_values[2] = std::clamp((fabsf(motion_blur_values[2]) - c::visuals::world::motionblur::falling_minimum) / (c::visuals::world::motionblur::falling_maximum - c::visuals::world::motionblur::falling_minimum), 0.0f, 1.0f) * (motion_blur_values[2] >= 0.0f ? 1.0f : -1.0f);
			motion_blur_values[2] /= 30.0f;
			motion_blur_values[0] *= c::visuals::world::motionblur::rotate_intensity * .15f * c::visuals::world::motionblur::strength;
			motion_blur_values[1] *= c::visuals::world::motionblur::rotate_intensity * .15f * c::visuals::world::motionblur::strength;
			motion_blur_values[2] *= c::visuals::world::motionblur::rotate_intensity * .15f * c::visuals::world::motionblur::strength;
			motion_blur_values[3] *= c::visuals::world::motionblur::falling_intensity * .15f * c::visuals::world::motionblur::strength;
		}

		if (interfaces::globals->realtime < history.no_rotational_mb_until)
		{
			motion_blur_values[0] = 0.0f;
			motion_blur_values[1] = 0.0f;
			motion_blur_values[3] = 0.0f;
		}
		else 
		{
			history.no_rotational_mb_until = 0.0f;
		}

		history.previous_pos = current_position;
		history.previous_pitch = current_pitch;
		history.previous_yaw = current_yaw;
		history.last_time_update = interfaces::globals->realtime;

		return;
	}

	i_material* material = interfaces::material_system->find_material("dev/motion_blur", "RenderTargets", false);

	if (material->is_error_material())
	{
		return;
	}

	const auto motion_blur_internal = material->find_var("$MotionBlurInternal", nullptr, false);

	motion_blur_internal->set_vec_component_value(motion_blur_values[0], 0);
	motion_blur_internal->set_vec_component_value(motion_blur_values[1], 1);
	motion_blur_internal->set_vec_component_value(motion_blur_values[2], 2);
	motion_blur_internal->set_vec_component_value(motion_blur_values[3], 3);

	const auto motion_blur_view_port_internal = material->find_var("$MotionBlurViewportInternal", nullptr, false);

	motion_blur_view_port_internal->set_vec_component_value(0.0f, 0);
	motion_blur_view_port_internal->set_vec_component_value(0.0f, 1);
	motion_blur_view_port_internal->set_vec_component_value(1.0f, 2);
	motion_blur_view_port_internal->set_vec_component_value(1.0f, 3);

	if (c::visuals::world::motionblur::video_adapter == 0 || c::visuals::world::motionblur::video_adapter == 3)
	{
		static convar* mat_res = interfaces::console->get_convar("mat_resolveFullFrameDepth");
		mat_res->set_value(0);
	}

	draw_screen_effect(material);
}

template <typename t>
static t interpolate(const t& t1, const t& t2, float progress)
{
	if (t1 == t2)
		return t1;

	return t2 * progress + t1 * (1.0f - progress);
}

void thirdperson_init(bool fakeducking, float progress) {
	/* our current fraction. */
	static float current_fraction = 0.0f;

	float distance = c::visuals::world::thirdperson::distance * progress;
	vec3_t angles, trace_angles;

	// get camera angles.
	interfaces::engine->get_view_angles(angles);
	interfaces::engine->get_view_angles(trace_angles);

	// cam_idealdist convar.
	trace_angles.z = distance;

	// set camera direction.
	vec3_t forward, right, up;
	math::angle_vectors(trace_angles, &forward, &right, &up);

	// various fixes to camera when fakeducking.
	auto eye_position = fakeducking ? g::local->get_absolute_origin() + interfaces::game_movement->GetPlayerViewOffset(false) : g::local->get_absolute_origin() + g::local->view_offset();
	auto camera_position = eye_position + forward * -distance + right + up;

	// setup trace filter and trace.
	trace_filter filter;
	trace_t trace;
	ray_t ray;
	ray.initialize(eye_position, camera_position, vec3_t(-16.0f, -16.0f, -16.0f), vec3_t(16.0f, 16.0f, 16.0f));

	// tracing to camera angles.
	interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &trace);

	auto fraction = trace.flFraction;

	if (fraction < 1.0f)
	{
		auto current_eye_pos = eye_position + right * (float)c::visuals::world::thirdperson::distance * 0.5f;
		current_eye_pos.z = eye_position.z;

		ray.initialize(current_eye_pos, camera_position, vec3_t(-16.0f, -16.0f, -16.0f), vec3_t(16.0f, 16.0f, 4.0f));
		interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &trace);

		if (trace.flFraction >= 1.0f && !trace.startSolid)
		{
			ray.initialize(camera_position, camera_position, vec3_t(-16.0f, -16.0f, -16.0f), vec3_t(16.0f, 16.0f, 4.0f));
			interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &trace);

			if (!trace.startSolid)
				fraction = 1.0f;
		}
		else
		{
			current_eye_pos = eye_position - right * (float)c::visuals::world::thirdperson::distance * 0.5f;
			current_eye_pos.z = eye_position.z;

			ray.initialize(current_eye_pos, camera_position, vec3_t(-16.0f, -16.0f, -16.0f), vec3_t(16.0f, 16.0f, 4.0f));
			interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &trace);

			if (trace.flFraction >= 1.0f && !trace.startSolid)
			{
				ray.initialize(camera_position, camera_position, vec3_t(-16.0f, -16.0f, -16.0f), vec3_t(16.0f, 16.0f, 4.0f));
				interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &trace);

				if (!trace.startSolid)
					fraction = 1.0f;
			}
		}
	}

	// interpolate camera speed if something behind our camera.
	if (current_fraction > trace.flFraction)
		current_fraction = trace.flFraction;
	else if (current_fraction > 0.9999f)
		current_fraction = 1.0f;

	// adapt distance to travel time.
	current_fraction = interpolate(current_fraction, trace.flFraction, interfaces::globals->frame_time * 10.0f);
	angles.z = distance * current_fraction;

	// override camera angles.
	interfaces::input->camera_offset = angles;

	// https://www.unknowncheats.me/forum/counterstrike-global-offensive/320594-thirdperson-flickering.html
	// https://www.unknowncheats.me/forum/counterstrike-global-offensive/319202-rendering-attached-wecapons-thirdperson.html
	// shoot out eexomi source
	g::local->update_visibility_all_entities();
}

void features::visuals::thirdperson()
{
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
	{
		return;
	}

	if (!g::local)
	{
		return;
	}

	static float progress;
	static bool in_transition;
	static auto in_thirdperson = false;

	if (!in_thirdperson && (c::visuals::world::thirdperson::enable && menu::checkkey(c::visuals::world::thirdperson::key, c::visuals::world::thirdperson::key_s)))
		in_thirdperson = true;
	else if (in_thirdperson && !(c::visuals::world::thirdperson::enable && menu::checkkey(c::visuals::world::thirdperson::key, c::visuals::world::thirdperson::key_s)))
		in_thirdperson = false;

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

	if (c::visuals::world::thirdperson::turn_off_while[0] && g::local->is_scoped())
	{
		in_thirdperson = false;
		interfaces::input->camera_in_third_person = false;

		return;
	}

	if (c::visuals::world::thirdperson::turn_off_while[1] && data->m_iWeaponType == WEAPONTYPE_GRENADE)
	{
		in_thirdperson = false;
		interfaces::input->camera_in_third_person = false;
		return;
	}

	if (c::visuals::world::thirdperson::turn_off_while[2] && data->m_iWeaponType == WEAPONTYPE_KNIFE)
	{
		in_thirdperson = false;
		interfaces::input->camera_in_third_person = false;
		return;
	}

	if (c::visuals::world::thirdperson::turn_off_while[3] && data->m_iWeaponType == WEAPON_TASER)
	{
		in_thirdperson = false;
		interfaces::input->camera_in_third_person = false;
		return;
	}

	if (in_thirdperson)
	{
		in_transition = false;

		if (!interfaces::input->camera_in_third_person)
			interfaces::input->camera_in_third_person = true;
	}
	else
	{
		progress -= interfaces::globals->frame_time * 8.f + (progress / 100);
		progress = std::clamp(progress, 0.f, 1.f);

		if (!progress)
			interfaces::input->camera_in_third_person = false;
		else
		{
			in_transition = true;
			interfaces::input->camera_in_third_person = true;
		}
	}

	if (interfaces::input->camera_in_third_person && !in_transition)
	{
		progress += interfaces::globals->frame_time * 8.f + (progress / 100);
		progress = std::clamp(progress, 0.f, 1.f);
	}

	thirdperson_init(false, progress);

	bool require_reset = false;

	if (g::local->is_alive())
	{
		require_reset = false;
		return;
	}

	if (c::visuals::world::thirdperson::when_spectating)
	{
		if (require_reset)
			g::local->observer_mode() = obs_mode_chase;

		if (g::local->observer_mode() == obs_mode_in_eye)
			require_reset = true;
	}
}