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
			if (client_class->class_id != cfogcontroller)
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

			if (client_class->class_id != class_ids::ccascadelight)	
				return;

			entity->shadow_direction() = vec3_t(c::visuals::world::shadow::x, c::visuals::world::shadow::y, c::visuals::world::shadow::z);
			entity->light_shadow_direction() = vec3_t(c::visuals::world::shadow::x, c::visuals::world::shadow::y, c::visuals::world::shadow::z);
		}();

		[&]() {
			if (client_class->class_id != class_ids::cenvtonemapcontroller)
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

	static bool* disable_postprocessing = *reinterpret_cast<bool**>(find_pattern("client.dll", "83 EC 4C 80 3D") + 0x5);
	*disable_postprocessing = c::visuals::post_processing::enable;

	interfaces::console->get_convar("mat_force_tonemap_scale")->callbacks.clear();
	interfaces::console->get_convar("mat_fullbright")->callbacks.clear();
	interfaces::console->get_convar("r_3dsky")->callbacks.clear();

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

		if (client_class->class_id != ccsragdoll)
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