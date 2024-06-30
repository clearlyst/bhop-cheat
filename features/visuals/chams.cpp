#include "../visuals/visuals.hpp"
#include "../../menu/config/config.hpp"
#include "../../menu/menu.hpp"
#include "../../sdk/math/math.hpp"
#include "../../features/aimbot/aimbot.hpp"
#include "../../hooks/hooks.hpp"

i_material* material_textured;
i_material* material_flat;
i_material* material_metallic;
i_material* material_plastic;
i_material* material_elixir;
i_material* material_glow;
i_material* material_crystal;
i_material* material_space;
i_material* material_glass;
i_material* material_oldshader;

i_material* create_material(std::string name, std::string type, std::string material) {
	auto kv = static_cast<c_key_values*>(malloc(36));
	kv->init(type.c_str());
	kv->load_from_buffer(name.c_str(), material.c_str());
	return interfaces::material_system->create_material(name.c_str(), kv);
}

void override_material(bool ignorez, int type, color_t clr) {
	i_material* material = nullptr;

	switch (type) {
	case 0: material = material_textured; break;
	case 1: material = material_flat; break;
	case 2: material = material_metallic; break;
	case 3: material = material_plastic; break;
	case 4: material = material_elixir; break;
	case 5: material = material_glow; break;
	case 6: material = material_textured; break;
	case 7: material = material_crystal; break;
	case 8: material = material_space; break;
	case 9: material = material_glass; break;
	}

	material->set_material_var_flag(material_var_ignorez, ignorez);
	material->set_material_var_flag(material_var_wireframe, type == 6);

	material->color_modulate(clr.r(), clr.g(), clr.b());
	material->alpha_modulate(clr.a());
	interfaces::model_render->override_material(material);
}

void override_overlay_material(bool ignorez, int type, color_t clr) {
	i_material* material = nullptr;

	switch (type) {
	case 0: material = material_textured; break;
	case 1: material = material_flat; break;
	case 2: material = material_metallic; break;
	case 3: material = material_plastic; break;
	case 4: material = material_elixir; break;
	case 5: material = material_glow; break;
	case 6: material = material_textured; break;
	case 7: material = material_crystal; break;
	case 8: material = material_space; break;
	case 9: material = material_glass; break;
	}

	material->set_material_var_flag(material_var_ignorez, ignorez);
	material->set_material_var_flag(material_var_wireframe, type == 6);

	// overlay color
	auto overlay = material->find_var("$envmaptint", nullptr);
	overlay->set_vec_value(clr.r(), clr.g(), clr.b());

	// color modulate
	material->color_modulate(clr.r(), clr.g(), clr.b());
	material->alpha_modulate(clr.a());
	interfaces::model_render->override_material(material);
}

void features::chams::initialize() {
	material_textured = interfaces::material_system->find_material("debug/debugambientcube", TEXTURE_GROUP_MODEL);
	material_textured->increment_reference_count();

	material_flat = interfaces::material_system->find_material("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
	material_flat->increment_reference_count();

	material_plastic = interfaces::material_system->find_material("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_MODEL);
	material_plastic->increment_reference_count();

	material_glow = interfaces::material_system->find_material("dev/glow_armsrace", TEXTURE_GROUP_MODEL);
	material_glow->increment_reference_count();

	material_glass = interfaces::material_system->find_material("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_MODEL);
	material_glass->increment_reference_count();

	material_metallic = create_material("metallic", "VertexLitGeneric", R"#("VertexLitGeneric" {
      "$basetexture" "vgui/white_additive"
      "$ignorez" "0"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "1"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
}
)#");

	material_elixir = create_material("elixir", "VertexLitGeneric", R"#("VertexLitGeneric" {
        "$basetexture" "VGUI/white_additive"
		"$bumpmap" "de_nuke/hr_nuke/pool_water_normals_002"
		"$bumptransform" "center 0.5 0.5 scale 0.25 0.25 rotate 0.0 translate 0.0 0.0"
		"$ignorez" "0"
		"$nofog" "1"
		"$model" "1"
		"$color2" "[1.0, 1.0, 1.0]"
		"$halflambert" "1"
		"$envmap" "env_cubemap"
		"$envmaptint" "[0.1 0.1 0.1]"
		"$envmapfresnel" "1.0"
		"$envmapfresnelminmaxexp" "[1.0, 1.0, 1.0]"
		"$phong" "1"
		"$phongexponent" "1024"
		"$phongboost" "4.0"
		"$phongfresnelranges" "[1.0, 1.0, 1.0]"
		"$rimlight" "1"
		"$rimlightexponent" "4.0"
		"$rimlightboost" "2.0"
    
		"Proxies"
		{
			"TextureScroll"
			{
				"textureScrollVar" "$bumptransform"
				"textureScrollRate" "0.25"
				"textureScrollAngle" "0.0"
			}
		}
   }  )#");

	material_crystal = create_material("crystal", "VertexLitGeneric", R"#("VertexLitGeneric"
        {
            "$basetexture" "vgui/white"
            "$envmap" "models/effects/crystal_cube_vertigo_hdr"
            "$envmaptint" "[1 1 1]"
            "$envmapfresnel" "1"
            "$envmapfresnelminmaxexp" "[0 1 2]"
            "$phong" "1"
            "$reflectivity" "[2 2 2]"
            "$bumpmap" "models/weapons/customization/materials/origamil_camo"
            "$bumptransform" "center 0.5 0.5 scale 0.25 0.25 rotate 0.0 translate 0.0 0.0"
            "Proxies"
            {
                "TextureScroll"
                {
                    "textureScrollVar" "$bumptransform"
                    "textureScrollRate" "0.2"
                    "textureScrollAngle" "0.0"
                }
            }
        })#");

	material_space = create_material("space", "VertexLitGeneric", R"#("VertexLitGeneric"
        {
		   "$basetexture"                "dev/snowfield"
		   "$ignorez" "0"
		   "$nofog" "1"
		   "$model" "1"
		   "$nocull" "0"
		   "$selfillum" "1"
		   "$halflambert" "1"
		   "$znearer" "0"
		   "$flat" "1" 
		   "$additive"                    "1"
		   "$envmap"                    "editor/cube_vertigo"
		   "$envmaptint"                "[0 0.5 0.55]"
		   "$envmapfresnel"            "1"
		   "$envmapfresnelminmaxexp"   "[0.00005 0.6 6]"
		   "$alpha"                    "1"
 		   
		    Proxies
		    {
		        TextureScroll
		        {
		            "texturescrollvar"            "$baseTextureTransform"
		            "texturescrollrate"            "0.10"
		            "texturescrollangle"        "270"
		        }
		    }
        })#");

	material_oldshader = create_material("shaders", "VertexLitGeneric", R"#("Character"
    {
		"$basetexture"            "models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle"
		"$bumpmap"                "models/weapons/v_models/arms/bare/bare_arm_133_normal"
		"$masks1"                 "models/weapons/v_models/arms/bare/bare_arm_masks1"
		"$phongwarptexture"       "models/weapons/customization/materials/material_specwarp"
		"$fresnelrangestexture"   "models/weapons/v_models/arms/bare/bare_arm_fresnelrange"
		"$basealphaenvmask"       "1"
		"$rimlightboost"          "1"
		"$rimlightexponent"       "8"
		"$ambientreflectionboost" "1.0"
		"$shadowsaturation"       "0.015"
		"$shadowsaturationbounds" "[0.25 0.5 0.5 0.7]"
		"$phongboost"             "1"
		"$envmap"                 "env_cubemap"
		"$envmaplightscale"      "0.5"
		"$shadowrimboost"			"1"
		
		"GPU<2"
		{
			"$phong" "0"
			"$envmap" ""
			"$envmaptint" "[0 0 0]"
		}
    })#");
}

void features::chams::run(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix)
{
	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	bool is_player = strstr(info.model->name, "models/player") != nullptr;
	bool is_hand = strstr(info.model->name, "arms") != nullptr;
	bool is_sleeve = strstr(info.model->name, "sleeve") != nullptr;
	bool is_weapon = strstr(info.model->name, "weapons/v_") != nullptr;
	bool is_glove = strstr(info.model->name, "models/weapons/v_models/arms/bare/v_bare_hands.mdl")
		&& strstr(info.model->name, "models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl")
		&& strstr(info.model->name, "models/weapons/v_models/arms/glove_fullfinger/v_glove_fullfinger.mdl")
		&& strstr(info.model->name, "models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl") != nullptr;
	bool is_weapon_on_back = strstr(info.model->name, "_dropped.mdl") && strstr(info.model->name, "models/weapons/w") && !strstr(info.model->name, "arms") && !strstr(info.model->name, "ied_dropped");
	bool is_weapon_enemy_hands = strstr(info.model->name, "models/weapons/w") && !strstr(info.model->name, "arms") && !strstr(info.model->name, "ied_dropped");
	bool is_defuse_kit = strstr(info.model->name, "defuser") && !strstr(info.model->name, "arms") && !strstr(info.model->name, "ied_dropped");

	if (is_player) 
	{
		player_t* player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(info.entity_index));

		if (!player || !player->is_alive() || player->dormant() || player == g::local || player->team() == g::local->team())
		{
			return;
		}

		auto material = interfaces::material_system->find_material(info.model->name, TEXTURE_GROUP_MODEL);

		if (!material)
		{
			return;
		}


		if (c::chams::invisible::enable)
		{
			if (c::chams::invisible::hide_original_model)
			{
				material->set_material_var_flag(material_var_no_draw, true);
				interfaces::model_render->override_material(material);
			}

			override_material(true, c::chams::invisible::type, color_t(c::chams::invisible::color[0], c::chams::invisible::color[1], c::chams::invisible::color[2], c::chams::invisible::color[3]));
			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
		}

		if (c::chams::invisible::overlay::enable)
		{
			override_material(true, c::chams::invisible::overlay::type, color_t(c::chams::invisible::overlay::color[0], c::chams::invisible::overlay::color[1], c::chams::invisible::overlay::color[2], c::chams::invisible::overlay::color[3]));
			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
		}

		interfaces::model_render->override_material(nullptr);

		if (c::chams::visible::enable)
		{
			if (c::chams::visible::hide_original_model)
			{
				material->set_material_var_flag(material_var_no_draw, true);
				interfaces::model_render->override_material(material);
			}

			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, c::chams::visible::type, color_t(c::chams::visible::color[0], c::chams::visible::color[1], c::chams::visible::color[2], c::chams::visible::color[3]));
		}

		if (c::chams::visible::overlay::enable)
		{
			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_overlay_material(false, c::chams::visible::overlay::type, color_t(c::chams::visible::overlay::color[0], c::chams::visible::overlay::color[1], c::chams::visible::overlay::color[2], c::chams::visible::overlay::color[3]));
		}
	}

	if (is_sleeve)
	{
		auto material = interfaces::material_system->find_material(info.model->name, TEXTURE_GROUP_MODEL);

		if (!material)
		{
			return;
		}

		interfaces::model_render->override_material(nullptr);

		if (c::chams::sleeves::enable)
		{
			if (c::chams::sleeves::hide_original_model)
			{
				material->set_material_var_flag(material_var_no_draw, true);
				interfaces::model_render->override_material(material);
			}

			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, c::chams::sleeves::type, color_t(c::chams::sleeves::color[0], c::chams::sleeves::color[1], c::chams::sleeves::color[2], c::chams::sleeves::color[3]));
		}

		if (c::chams::sleeves::overlay::enable)
		{
			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_overlay_material(false, c::chams::sleeves::overlay::type, color_t(c::chams::sleeves::overlay::color[0], c::chams::sleeves::overlay::color[1], c::chams::sleeves::overlay::color[2], c::chams::sleeves::overlay::color[3]));
		}
	}

	if (is_hand && !is_sleeve)
	{
		auto material = interfaces::material_system->find_material(info.model->name, TEXTURE_GROUP_MODEL);

		if (!material)
		{
			return;
		}

		interfaces::model_render->override_material(nullptr);

		if (c::chams::hands::enable)
		{
			if (c::chams::hands::hide_original_model)
			{
				material->set_material_var_flag(material_var_no_draw, true);
				interfaces::model_render->override_material(material);
			}

			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, c::chams::hands::type, color_t(c::chams::hands::color[0], c::chams::hands::color[1], c::chams::hands::color[2], c::chams::hands::color[3]));
		}

		if (c::chams::hands::overlay::enable)
		{
			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_overlay_material(false, c::chams::hands::overlay::type, color_t(c::chams::hands::overlay::color[0], c::chams::hands::overlay::color[1], c::chams::hands::overlay::color[2], c::chams::hands::overlay::color[3]));
		}
	}

	if (is_weapon && !is_hand && !is_sleeve)
	{
		auto material = interfaces::material_system->find_material(info.model->name, TEXTURE_GROUP_MODEL);

		if (!material)
		{
			return;
		}

		interfaces::model_render->override_material(nullptr);

		if (c::chams::weapon::enable)
		{
			if (c::chams::weapon::hide_original_model)
			{
				material->set_material_var_flag(material_var_no_draw, true);
				interfaces::model_render->override_material(material);
			}

			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, c::chams::weapon::type, color_t(c::chams::weapon::color[0], c::chams::weapon::color[1], c::chams::weapon::color[2], c::chams::weapon::color[3]));
		}

		if (c::chams::weapon::overlay::enable)
		{
			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_overlay_material(false, c::chams::weapon::overlay::type, color_t(c::chams::weapon::overlay::color[0], c::chams::weapon::overlay::color[1], c::chams::weapon::overlay::color[2], c::chams::hands::overlay::color[3]));
		}
	}

	if (is_weapon_on_back || is_weapon_enemy_hands || is_defuse_kit)
	{
		auto material = interfaces::material_system->find_material(info.model->name, TEXTURE_GROUP_MODEL);

		if (!material)
		{
			return;
		}

		if (c::chams::invisible_attachment::enable)
		{
			if (c::chams::invisible_attachment::hide_original_model)
			{
				material->set_material_var_flag(material_var_no_draw, true);
				interfaces::model_render->override_material(material);
			}

			override_material(true, c::chams::invisible_attachment::type, color_t(c::chams::invisible_attachment::color[0], c::chams::invisible_attachment::color[1], c::chams::invisible_attachment::color[2], c::chams::invisible_attachment::color[3]));
			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
		}

		if (c::chams::invisible_attachment::overlay::enable)
		{
			override_material(true, c::chams::invisible_attachment::overlay::type, color_t(c::chams::invisible_attachment::overlay::color[0], c::chams::invisible_attachment::overlay::color[1], c::chams::invisible_attachment::overlay::color[2], c::chams::invisible_attachment::overlay::color[3]));
			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
		}

		interfaces::model_render->override_material(nullptr);

		if (c::chams::visible_attachment::enable)
		{
			if (c::chams::visible_attachment::hide_original_model)
			{
				material->set_material_var_flag(material_var_no_draw, true);
				interfaces::model_render->override_material(material);
			}

			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, c::chams::visible_attachment::type, color_t(c::chams::visible_attachment::color[0], c::chams::visible_attachment::color[1], c::chams::visible_attachment::color[2], c::chams::visible_attachment::color[3]));
		}

		if (c::chams::visible_attachment::overlay::enable)
		{
			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_overlay_material(false, c::chams::visible_attachment::overlay::type, color_t(c::chams::visible_attachment::overlay::color[0], c::chams::visible_attachment::overlay::color[1], c::chams::visible::overlay::color[2], c::chams::visible::overlay::color[3]));
		}
	}
}

void override_material_bt(bool ignorez, int type, color_t clr) {
	i_material* material = nullptr;

	switch (type) {
	case 0: material = material_textured; break;
	case 1: material = material_flat; break;
	case 2: material = material_metallic; break;
	case 3: material = material_plastic; break;
	case 4: material = material_elixir; break;
	case 5: material = material_glow; break;
	case 6: material = material_textured; break;
	case 7: material = material_crystal; break;
	case 8: material = material_space; break;
	case 9: material = material_glass; break;
	}

	material->set_material_var_flag(material_var_ignorez, ignorez);
	material->set_material_var_flag(material_var_wireframe, type == 6);

	material->color_modulate(clr.r(), clr.g(), clr.b());
	material->alpha_modulate(clr.a());
	interfaces::model_render->override_material(material);
}

void features::chams::run_bt(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix)
{
	if (!g::local || !g::local->is_alive()  || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	bool is_player = strstr(info.model->name, "models/player") != nullptr;

	if (is_player) 
	{
		player_t* player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(info.entity_index));

		if (!player || !player->is_alive() || player->dormant() || player == g::local || player->team() == g::local->team() || !player->is_moving())
		{
			return;
		}

		if (c::chams::backtrack::enable)
		{
			auto record = &backtrack.data[player->index()];

			if (record->empty())
			{
				return;
			}

			if (c::chams::backtrack::draw_all_ticks)
			{
				for (int i = 0; i < record->size(); i++)
				{
					if (record->at(i).sim_time)
					{
						float h, s, v, a;
						h = (math::rgbtoh(c::chams::backtrack::color_2[0], c::chams::backtrack::color_2[1], c::chams::backtrack::color_2[2]) - math::rgbtoh(c::chams::backtrack::color_1[0], c::chams::backtrack::color_1[1], c::chams::backtrack::color_1[2])) / backtrack.data[player->index()].size();
						s = (math::rgbtos(c::chams::backtrack::color_2[0], c::chams::backtrack::color_2[1], c::chams::backtrack::color_2[2]) - math::rgbtos(c::chams::backtrack::color_1[0], c::chams::backtrack::color_1[1], c::chams::backtrack::color_1[2])) / backtrack.data[player->index()].size();
						v = (math::rgbtov(c::chams::backtrack::color_2[0], c::chams::backtrack::color_2[1], c::chams::backtrack::color_2[2]) - math::rgbtov(c::chams::backtrack::color_1[0], c::chams::backtrack::color_1[1], c::chams::backtrack::color_1[2])) / backtrack.data[player->index()].size();
						a = (c::chams::backtrack::color_2[3] - c::chams::backtrack::color_1[3]) / backtrack.data[player->index()].size();
																		
						float hh, ss, vv;								
						hh = math::rgbtoh(c::chams::backtrack::color_1[0], c::chams::backtrack::color_1[1], c::chams::backtrack::color_1[2]) + (h * i);
						ss = math::rgbtos(c::chams::backtrack::color_1[0], c::chams::backtrack::color_1[1], c::chams::backtrack::color_1[2]) + (s * i);
						vv = math::rgbtov(c::chams::backtrack::color_1[0], c::chams::backtrack::color_1[1], c::chams::backtrack::color_1[2]) + (v * i);
						float fade_color[3] = { math::fromhsvtor(hh, ss, vv), math::fromhsvtog(hh, ss, vv), math::fromhsvtob(hh, ss, vv) };

						color_t gradient = color_t(fade_color[0], fade_color[1], fade_color[2], c::chams::backtrack::color_1[3] + (a * i));

						override_material_bt(c::chams::backtrack::invisible, c::chams::backtrack::type, c::chams::backtrack::gradient ? gradient : color_t(c::chams::backtrack::color_2[0], c::chams::backtrack::color_2[1], c::chams::backtrack::color_2[2], c::chams::backtrack::color_2[3]));

						sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, backtrack.data[player->index()][i].m_matrix);
						interfaces::model_render->override_material(nullptr);
					}
				}
			}
			else 
			{
				if (record->front().sim_time)
				{
					override_material_bt(c::chams::backtrack::invisible, c::chams::backtrack::type, color_t(c::chams::backtrack::color_2[0], c::chams::backtrack::color_2[1], c::chams::backtrack::color_2[2], c::chams::backtrack::color_2[3]));
					sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, record->back().m_matrix);
					interfaces::model_render->override_material(nullptr);
				}
			}
		}
	}
}