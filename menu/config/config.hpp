#pragma once
#include <string_view>
#include <vector>
#include "../../includes/nlohmann/json.hpp"

namespace c {
	void create_directory();
	void update_configs();
	void create_file(const std::string_view name);
	void delete_file(const std::size_t index);
	void save(const std::size_t index);
	void load(const std::size_t index);
	inline std::string directory_path_fonts;
	inline std::string directory_path;
	inline std::string directory;

	namespace backtrack 
	{
		inline bool enable = false;
		inline int key;
		inline int key_s = 1;
		inline int time = 0;
		inline float fake = 200;
		inline bool fake_latency = false;
	}

	namespace triggerbot 
	{
		inline bool enable = false;
		inline int key;
		inline int key_s = 1;
	}

	namespace aimbot
	{
		inline bool enable = false;
		inline bool shoot_at_aim_key = false;
		inline bool aim_at_backtrack = false;
		inline int key;
		inline int key_s = 1;

		struct aimbot_value_t
		{
			bool override_general = false;
			bool autopistol = false;
			bool silent = false;
			bool recoil_compensate = false;
			bool autowall = false;
			int autowall_damage = 0;
			int fov = 0;
			bool head = true;
			bool neck = false;
			bool chest = false;
			bool stomach = false;
			bool pelvis = false;
			bool foot = false;

			aimbot_value_t() = default;
		};

		inline aimbot_value_t general = aimbot_value_t();
		inline aimbot_value_t pistols = aimbot_value_t();
		inline aimbot_value_t heavy_pistols = aimbot_value_t();
		inline aimbot_value_t smg = aimbot_value_t();
		inline aimbot_value_t shotguns = aimbot_value_t();
		inline aimbot_value_t rifles = aimbot_value_t();
		inline aimbot_value_t snipers = aimbot_value_t();
	}

	namespace movement {
		inline int  dh_tick = 1;
		inline bool auto_strafe = false;
		inline int auto_strafe_key = 0;
		inline int auto_strafe_key_s = 1;
		inline float auto_strafe_minimum_speed = 0.f;
		inline float auto_strafe_maximum_speed = 0.f;
		inline bool bhop = false;
		inline bool null_strafing = false;
		inline bool delay_hop = false;
		inline int  delay_hop_key = 0;
		inline int  delay_hop_key_s = 1;
		inline bool edge_bug = false;
		inline bool edge_bug_asisst = false;
		inline int edge_bug_key = 0;
		inline int edge_bug_key_s = 1;
		inline float edge_bug_time = 0.75f;
		inline bool edge_bug_mouselock = 0;
		inline float edge_bug_mouselock_strenght = 0.0f;
		inline bool edge_jump = false;
		inline int edge_jump_key = 0;
		inline int edge_jump_key_s = 1;
		inline bool ladder_edge_jump = false;
		inline int ladder_edge_jump_key = 0;
		inline int ladder_edge_jump_key_s = 1;
		inline bool adaptive_edge_jump = false;
		inline bool adaptive_edge_jump_for[3] = { false, false, false };
		inline bool long_jump = false;
		inline int long_jump_key = 0;
		inline int long_jump_key_s = 1;
		inline bool lj_null[4] = { false, false, false, false };
		inline bool mini_jump = false;
		inline int mini_jump_key = 0;
		inline int mini_jump_key_s = 1;
		inline bool adaptive_key_cancelling = false;
		inline bool adaptive_key_for[4] = { false, false, false, false };
		inline bool block_bot = false;
		inline int block_bot_key = 0;
		inline int block_bot_key_s = 1;
		inline bool strafe_optimizer = false;
		inline int strafe_optimizer_key = 0;
		inline int strafe_optimizer_key_s = 1;
		inline float strafe_optimizer_gain = 0.f;
		inline float strafe_optimizer_minimum_speed = 0.f;
		inline float strafe_optimizer_maximum_speed = 0.f;
		inline bool mouse_strafe_limiter = false;
		inline bool mouse_strafe_limiter_affect_pitch = false;
		inline float mouse_strafe_limiter_value = 20.f;
		inline int mouse_strafe_limiter_key = 0;
		inline int mouse_strafe_limiter_key_s = 1;
		inline bool jump_bug = false;
		inline int jump_bug_key = 0;
		inline int jump_bug_key_s = 1;
		inline bool fastduck = false;
		inline bool auto_align = false;
		inline bool freelook_surf = true;
		inline bool pixel_surf_fix = false;
		inline bool pixel_surf = false;
		inline int  pixel_surf_key = 0;
		inline int  pixel_surf_key_s = 1;
		inline int pixel_surf_ticks = 7;

		inline bool detection_printf[3] = { false, false, false };

		namespace indicators
		{
			inline int position = 200;

			namespace graphs
			{
				inline bool fade = true;
				inline float size = 1.00f;

				namespace velocity
				{
					inline bool enable = false;
					inline bool draw_velocity = false;
					inline bool draw_jumpbug = false;
					inline bool draw_edgebug = false;
					inline bool draw_pixelsurf = false;
					inline float color[3]{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f };
				};

				namespace stamina
				{
					inline bool enable = false;
					inline float color[3]{ 166 / 255.0f, 143 / 255.0f, 242 / 255.0f };
				};
			};

			namespace binds
			{
				inline bool enable = false;
				inline bool custom_color = false;
				inline float speed = 1.25f;
				inline int sameline = 2;
				inline float color[3]{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f };
				inline float color_1[3]{ 90 / 255.0f, 160 / 255.0f, 255 / 255.0f };
				inline float color_2[3]{ 170 / 255.0f, 255 / 255.0f, 140 / 255.0f };
				inline float color_3[3]{ 255 / 255.0f, 190 / 255.0f, 155 / 255.0f };
				inline float color_4[3]{ 190 / 255.0f, 155 / 255.0f, 255 / 255.0f };

				inline bool list[14] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false };
			};

			namespace velocity
			{
				inline bool enable = true;
				inline bool disable_takeoff_on_ground = true;
				inline bool takeoff = true;
				inline bool custom_color = true;

				inline float color_1[4]{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 255 / 255.0f };
				inline float color_2[4]{255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 255 / 255.0f };

				inline float color_3[4]{ 23 / 255.0f, 238 / 255.0f, 103 / 255.0f, 255 / 255.0f };
				inline float color_4[4]{ 225 / 255.0f, 100 / 255.0f, 100 / 255.0f, 255 / 255.0f };
				inline float color_5[4]{ 255 / 255.0f, 200 / 255.0f, 100 / 255.0f, 255 / 255.0f };
			};

			namespace stamina
			{
				inline bool enable = true;
				inline bool takeoff = true;
				inline bool disable_takeoff_on_ground = true;
				inline bool custom_color = true;
				inline float color_1[4]{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 255 / 255.0f };
				inline float color_2[4]{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 255 / 255.0f };
				inline float color[4]{ 200 / 255.0f, 200 / 255.0f, 200 / 255.0f, 255 / 255.0f };
			};

			namespace keys
			{
				inline bool enable = false;
				inline bool background = false;
				inline bool mouse_direction = false;
				inline bool under_line = false;
				inline bool nulling = false;
				inline float color[3]{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f };
				inline int position = 200;
			};
		};
	}

	namespace misc {
		inline bool force_crosshair = false;
		inline bool sniper_crosshair = false;
		inline bool recoil_crosshair = false;

		namespace bullet_impacts
		{
			inline bool enable = false;
			inline float time = 3.00f;

			namespace server
			{
				inline bool enable = false;
				inline float color[4]{ 1.f, 1.f, 1.f, 127 / 255.0f };
			};

			namespace client
			{
				inline bool enable = false;
				inline float color[4]{ 1.f, 1.f, 1.f, 127 / 255.0f };
			};
		};

		namespace bullet_tracers
		{
			inline bool enable = false;
			inline float time = 3.0f;

			namespace local
			{
				inline bool enable = false;
				inline float color[4]{ 1.f, 1.f, 1.f, 1.f };
			};

			namespace entity
			{
				inline bool enable = false;
				inline float color[4]{ 1.f, 1.f, 1.f, 1.f };
			};

			namespace hit_entity
			{
				inline bool enable = false;
				inline float color[4]{ 1.f, 1.f, 1.f, 1.f };
			};
		};

		namespace penetration_reticle
		{
			inline bool enable = false;
		};

		namespace spread_circle
		{
			inline bool enable = false;
			inline float color[3]{ 1.f, 1.f, 1.f };
		};

		namespace debug_information
		{
			inline bool enable = false;

			namespace autowall
			{
				inline bool enable = false;
				inline bool rainbow_color = false;
				inline float inactive_color[3]{ 1.f, 1.f, 1.f };
				inline float active_color[3]{ 1.f, 1.f, 1.f };
			};

			namespace can_fire
			{
				inline bool enable = false;
				inline int style = 0;
				inline float inactive_color[3]{ 1.f, 0.f, 0.f };
				inline float active_color[3]{ 0.f, 1.f, 0.f };
			};

			namespace check_point_system
			{
				inline bool enable = false;
				inline float inactive_color[3]{ 1.f, 1.f, 1.f };
				inline float active_color[3]{ 1.f, 1.f, 1.f };
			};
		};

		namespace preverse_killfeed
		{
			inline bool enable = false;
			inline float time = 2.0f;
		};

		namespace sv_cheats
		{
			inline bool enable = false;
		};

		inline bool spectators_list = false;
		inline bool spectators_list_mode = false;
		inline bool spectators_list_avatars = false;
		inline bool practice = false;
		inline int savepos = 0;
		inline int savepos_s = 1;
		inline int loadpos = 0;
		inline int loadpos_s = 1;
		inline bool enable_fov = false;
		inline float field_of_view = 68.0f;
		inline bool view_model = false;
		inline float view_model_x = 2.0f;
		inline float view_model_y = 0.0f;
		inline float view_model_z = -1.5f;
		inline bool radaringame = false;
		inline bool swayscale = false;
		inline float swayscale_value = 0.f;
		inline char misc_clantag_text[256] = " ";
		inline bool misc_clantag_spammer = false;
		inline bool misc_clantag_reverse_rolling = false;
		inline int misc_clantag_type = 1;
		inline float misc_clantag_speed = 1.0f;
		inline bool misc_hitmarker = false;
		inline bool misc_hitmarker_sound = false;
		inline bool misc_hit_info[3] = { false, false, false };
		inline bool misc_hitchams = false;
		inline float misc_hitchams_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline int misc_hitmarker_sound_type = 0;
		inline bool nadepred = false;
		inline float nadepred_clr[3]{ 1.f, 1.f, 1.f };
		inline bool freecam = false;
		inline int freecam_key = 0;
		inline int freecam_key_s = 1;
		inline bool mousefix = false;
		inline int menu_key = 0x2D; //ins
	}

	namespace skins {
		inline bool agent_changer = false;
		inline int agent_t = 0;
		inline int agent_ct = 0;

		inline bool knife_changer_enable = false;
		inline int knife_changer_wear = 0;
		inline int knife_changer_model = 0;
		inline int knife_changer_skin_id = 0;
		inline int knife_changer_paint_kit = 0;
		inline int knife_changer_vector_paint_kit = 0;
		inline int knife_skin = 0;

		inline bool gloves_endable = false;
		inline int gloves_model = 0;
		inline int gloves_skin = 0;
		inline int gloves_skin_id = 0;
		inline int gloves_wear = 0;
		inline int agent_model = 0;

		inline bool skin_custom_clr = false;
		inline float skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float skin_modulation4[3]{ 1.f, 1.f, 1.f };

		inline bool weapon_endable = false;
		inline int weapons_page = 0;
		inline int weapons_model = 0;

	}

	namespace visuals 
	{
		inline bool enable = false;
		inline int key = 0;
		inline int key_s = 3;

		namespace glow
		{
			inline bool enable = false;

			namespace players
			{
				inline bool visible = false;
				inline bool invisible = false;
				inline int type = 0;

				namespace color
				{
					inline float visible[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
					inline float invisible[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				};
			};

			namespace bomb_and_defuse_kit
			{
				inline bool enable = false;
				inline float color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
			};

			namespace weapon
			{
				inline bool enable = false;
				inline float color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
			};

			namespace planted_bomb
			{
				inline bool enable = false;
				inline float color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
			};

			namespace grenade
			{
				inline bool enable = false;
				inline float color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
			};

			namespace hostage
			{
				inline bool enable = false;
				inline float color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
			};
		};

		namespace players
		{
			inline bool engine_radar = false;

			namespace colors
			{
				inline bool custom = false;
				inline float visible[4]{ 0.0f, 1.0f, 0.0f, 1.0f };
				inline float invisible[4]{ 1.0f, 0.0f, 0.0f, 1.0f };
				inline float box[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float box_outline[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
				inline float name[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float health_bar[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float health_bar_upper[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float health_bar_lower[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float health_bar_outline[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
				inline float health_text[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float ammo_text[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float ammo_bar[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float ammo_bar_outline[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
				inline float weapon_text[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float weapon_icon[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float view_angle[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float skeleton[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float backtrack_skeleton[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float backtrack_dot[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float snapline[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float distance[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float dlight[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float out_of_view[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
				inline float sounds[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
			};

			namespace fade
			{
				inline bool enable = false;
				inline float time = 0.0f;
			};

			namespace box
			{
				inline bool enable = false;
				inline int type = 0;
				inline bool outline[2] = { false, false };
				inline float lenght = 5.f;
			};

			namespace name
			{
				inline bool enable = false;
				inline bool bot_check = false;
			};

			namespace health_bar
			{
				inline bool enable = false;
				inline bool baseonhealth = false;
				inline bool gradient = false;
				inline bool background = false;
				inline bool text = false;
				inline int size = 1;
			};

			namespace health_text
			{
				inline bool enable = false;
				inline bool baseonhealth = false;
				inline bool hide_suffix = false;
				inline bool hide_at_max = false;
			};

			namespace ammo_text
			{
				inline bool enable = false;
			};

			namespace ammo_bar
			{
				inline bool enable = false;
				inline bool background = false;
				inline int size = 1;
			};

			namespace weapon_text
			{
				inline bool enable = false;
			};

			namespace weapon_icon
			{
				inline bool enable = false;
			};

			namespace view_angle
			{
				inline bool enable = false;
				inline float thickness = 0.5f;
				inline float distance = 50.0f;
			};

			namespace skeleton
			{
				inline bool enable = false;
				inline float thickness = 0.5f;
			};

			namespace backtrack_skeleton
			{
				inline bool enable = false;
				inline float thickness = 0.5f;
			};

			namespace backtrack_dot
			{
				inline bool enable = false;
			};

			namespace out_of_view
			{
				inline bool enable = false;
				inline float size = 7.5f;
				inline float distance = 30.0f;
			};

			namespace flags
			{
				inline bool enable = false;
				inline bool flags[7] = { false, false, false, false, false, false, false };
			};

			namespace distance
			{
				inline bool enable = false;
			};

			namespace snapline
			{
				inline bool enable = false;
				inline bool type[3] = { false, false, false };
				inline float thickness = 0.5f;
			};

			namespace emitted_sound
			{
				inline bool enable = true;
			};

			namespace dlight
			{
				inline bool enable = false;
				inline float radius = 50.0f;
				inline float color[4]{ 1.f, 1.f, 1.f, 1.f };
			};

			namespace dropped_weapon
			{
				namespace box
				{
					inline bool enable = false;
					inline int type = 0;
					inline bool outline[2] = { false, false };
					inline float lenght = 5.f;
					inline float color[4]{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 200 / 255.0f };
				};

				namespace text
				{
					inline bool enable = false;
					inline float color[4]{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 200 / 255.0f };
				};

				namespace icon
				{
					inline bool enable = false;
					inline float color[4]{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 200 / 255.0f };
				};

				namespace ammo_text
				{
					inline bool enable = false;
					inline float color[4]{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 200 / 255.0f };
				};

				namespace ammo_bar
				{
					inline bool enable = false;
					inline float color[4]{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 200 / 255.0f };
				};
			};

			namespace thrown_grenade
			{
				namespace text
				{
					inline bool enable = false;
					inline float color[4]{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 200 / 255.0f };
				};

				namespace icon
				{
					inline bool enable = false;
					inline float color[4]{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 200 / 255.0f };
				};
			};
		};

		namespace world
		{
			namespace fog
			{
				inline bool enable = false;
				inline float start = 0.0f;
				inline float end = 3000.0f;
				inline float density = 1.0f;
				inline float color[4]{ 1.f, 1.f, 1.f, 1.f };
			};

			namespace shadow
			{
				inline bool enable = false;
				inline float x = 200.0f;
				inline float y = 300.0f;
				inline float z = -600.0f;
			};

			namespace bloom
			{
				inline bool enable = false;
				inline float scale = 0.0f;
				inline float exposure_min = 0.0f;
				inline float exposure_max = 0.0f;
			};

			namespace brightness
			{
				inline bool enable = false;
				inline float amount = 1.0f;
			};

			namespace fullbright
			{
				inline bool enable = false;
			};

			namespace particles
			{
				inline bool enable = false;
				inline float color[4]{ 1.f, 1.f, 1.f, 1.f };
			};

			namespace skybox
			{
				inline bool enable = false;
				inline int style = 0;
			};

			namespace world_modulation
			{
				inline bool enable = false;
				inline float color[4]{ 1.f, 1.f, 1.f, 1.f };
			};

			namespace props_modulation
			{
				inline bool enable = false;
				inline float color[4]{ 1.f, 1.f, 1.f, 1.f };
			};

			namespace skybox_modulation
			{
				inline bool enable = false;
				inline float color[4]{ 1.f, 1.f, 1.f, 1.f };
			};

			namespace ragdoll
			{
				inline bool enable = false;
				inline int style = 0;
			};

			namespace aspect_ratio_modulation
			{
				inline bool enable = false;
				inline float amount = 1.78f;
			};

			namespace motionblur
			{
				inline bool enable = false;
				inline int video_adapter = 1;
				inline bool forward_move_blur = false;
				inline float falling_minimum = 10.0f;
				inline float falling_maximum = 20.0f;
				inline float falling_intensity = 1.0f;
				inline float rotate_intensity = 1.0f;
				inline float strength = 1.0f;
			};

			namespace thirdperson
			{
				inline bool enable = false;
				inline bool when_spectating = false;
				inline int key = 0;
				inline int key_s = 0;
				inline bool turn_off_while[4] = { false, false, false, false };
				inline int distance = 150;
			};

			namespace weather
			{
				inline bool enable = false;
				inline int type = 1;
				inline bool custom_density = false;
				inline float density = 100.0f;
				inline bool custom_radius = false;
				inline float radius = 100.0f;
			};
		};

		namespace post_processing
		{
			inline bool enable = false;
		};

		namespace player_model
		{
			inline bool enable = false;
		};

		namespace player_sleeves
		{
			inline bool enable = false;
		};

		namespace scope
		{
			inline bool enable = false;
			inline float color[4]{ 1.f, 1.f, 1.f, 1.f };
		};

		namespace flash
		{
			inline bool enable = false;
			inline bool full = false;
			inline int alpha = 100;
		};

		namespace smoke
		{
			inline bool enable = false;
		};

		namespace panorama_blur
		{
			inline bool enable = false;
		};
	}

	namespace chams {
		inline bool backtrack_chams = false;
		inline bool backtrack_chams_gradient = false;
		inline bool backtrack_chams_invisible = false;
		inline bool backtrack_chams_draw_all_ticks = false;
		inline float backtrack_chams_clr1[4]{ 1.f, 1.f, 1.f, 1.f };
		inline float backtrack_chams_clr2[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool visible_chams = false;
		inline bool visible_wireframe = false; //
		inline float visible_chams_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool visible_chams_ov = false; //
		inline bool visible_wireframe_ov = false; //
		inline float visible_chams_clr_ov[4]{ 1.f, 1.f, 1.f, 1.f }; //
		inline bool invisible_chams = false;
		inline float invisible_chams_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool invisible_wireframe = false; //
		inline int cham_type_bt = 0;
		inline int cham_type = 0;
		inline int cham_type_overlay = 0; //
		inline int chams_backrack_ticks = 0;
		inline int type_p = 0;
	}

	namespace fonts {

		inline int indi_size = 28;
		inline int indi_font = 0;
		inline bool indi_font_flag[11] = { false, false, false, false, false, false, false, false, false, true, false };

		inline int sub_indi_size = 9;
		inline int sub_indi_font = 0;
		inline bool sub_indi_font_flag[11] = { false, false, false, true, false, false, true, false, true, true, false };

		inline int scene_font = 0;
		inline int scene_size = 12;
		inline bool scene_flag[11] = { false, false, false, true, false, false, true, false, false, true, false };

		inline int scene_sub_font = 0;
		inline int scene_sub_size = 12;
		inline bool scene_sub_flag[11] = { false, false, false, true, false, false, true, false, false, true, false };

		inline int esp_font = 0;
		inline int esp_size = 9;
		inline bool esp_flag[11] = { false, false, false, true, false, false, true, false, true, true, false };

		inline int esp_sub_font = 0;
		inline int esp_sub_size = 9;
		inline bool esp_sub_flag[11] = { false, false, false, true, false, false, true, false, true, true, false };

		inline int debug_information_font = 0;
		inline int debug_information_size = 32;
		inline bool debug_information_flag[11] = { false, false, false, true, false, false, true, false, true, true, false };
	}

	inline std::vector<std::string> configs;
}

struct cvar_t;
struct cvars_t;