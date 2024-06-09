#pragma once
#include "../../sdk/sdk.hpp"
#include "../../hooks/hooks.hpp"

namespace features::chams {
	void run(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix);
	void run_bt(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix);
	void initialize();
}

struct bbox_t {
	int x, y, w, h;
	bbox_t() = default;
	bbox_t(const int x, const int y, const int w, const int h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};

namespace features::visuals {
	bool get_player_box(player_t* entity, bbox_t& in);
	bool get_entity_box(entity_t* entity, bbox_t& in);

	void get_update_sounds();

	void throwed_grenade(entity_t* entity);
	void dropped_weapon(entity_t* entity, bbox_t bbox);

	namespace dormant_system
	{
		void start();
		void finish();

		void adjust_player_begin(player_t* player, int i);
		void adjust_player_finish();
		void setup_adjust_player(player_t* player, SndInfo_t& sound);
		
		bool is_valid_sound(SndInfo_t& sound);
		
		struct SoundPlayer {
			void override(SndInfo_t& sound) {
				m_iIndex = sound.m_nSoundSource;
				m_vecOrigin = *sound.m_pOrigin;
				m_iReceiveTime = GetTickCount();
			}
		
			int m_iIndex = 0;
			int m_iReceiveTime = 0;
			vec3_t m_vecOrigin = vec3_t(0, 0, 0);
			vec3_t m_vecLastOrigin = vec3_t(0, 0, 0);
		
			/* Restore data */
			int m_nFlags = 0;
			int playerindex = 0;
			vec3_t m_vecAbsOrigin = vec3_t(0, 0, 0);
			bool m_bDormant = false;
		} inline m_cSoundPlayers[64];
		
		inline CUtlVector<SndInfo_t> m_utlvecSoundBuffer;
		inline std::vector<SoundPlayer> m_arRestorePlayers;
		inline std::vector< std::pair<vec3_t, int> > restore_sound;
	}

	struct SoundInfo_t {
		int guid;
		float soundTime;
		float alpha;
		vec3_t soundPos;
	};

	inline std::map<int, std::vector<SoundInfo_t>> m_sound_list;

	namespace player 
	{
        inline float player_alpha[65] = { };

		void run();
		void draw_name(player_t* entity, bbox_t bbox, color_t color);
		void draw_box(player_t* entity, bbox_t bbox, color_t color, color_t color_second);
		void draw_health_bar(player_t* entity, bbox_t bbox, color_t color, color_t upper, color_t lower, color_t outline);
		void draw_backtrack_dot(player_t* entity, color_t color);
		void draw_backtrack_skeleton(player_t* entity, color_t color);
		void draw_skeleton(player_t* entity, color_t color);
		void draw_snapline(player_t* entity, bbox_t bbox, color_t color);
		void draw_viewangle(player_t* entity, color_t color);
		void draw_flags(player_t* entity, bbox_t bbox, color_t color);
		void draw_bottom_bar(player_t* entity, bbox_t bbox, color_t ammo_bar, color_t health_text, color_t weapon_icon, color_t weapon_text, color_t ammo_text, color_t distance, color_t ammo_bar_outline);
		void draw_outoffov(player_t* entity, color_t color);
		void draw_sounds();
	}

	namespace glow {
		void run();
	}

	namespace weather
	{
		struct precip_entity_t {
			entity_t* m_entity;
			i_client_networkable* m_networkable;
			vec3_t m_mins;
			vec3_t m_maxs;
			vcollide_t m_collide;
			bool m_collide_init;
			int m_model_index;
			int m_abs_model_index;
			precipitation_type_t m_type;
		};

		void precipitation_update();
		void precipitation_main(client_frame_stage_t stage, bool original_called);
		void clean_up_precipitation();
		void unload_precip_entity(precip_entity_t& precip);
		void remove_unused_entities(precipitation_type_t type);
		bool is_type_active(precipitation_type_t type);
		void apply_weather_effect(precipitation_type_t type);
		void create_precip_entity(precip_entity_t* ent_info, precipitation_type_t precip_mode);

		inline std::list< precip_entity_t > m_precip_list;
	}

	void particles();
	void entities_ragdoll();
	void skybox_changer(client_frame_stage_t stage);
	void flashalpha();
	void nosmoke();
	void draw_scope();
	void dlights(player_t* entity);
	void run_freecam(c_usercmd* cmd, vec3_t angles);
	void freecam(view_setup_t* setup);
	void motion_blur(view_setup_t* setup);
	void thirdperson();
}

namespace features::grenade_prediction {

	class c_nadepoint {
	public:
		c_nadepoint() {
			m_valid = false;
		}

		c_nadepoint(vec3_t start, vec3_t end, bool plane, bool valid, vec3_t normal, bool detonate) {
			m_start = start;
			m_end = end;
			m_plane = plane;
			m_valid = valid;
			m_normal = normal;
			m_detonate = detonate;
		}

		vec3_t m_start, m_end, m_normal;
		bool m_valid, m_plane, m_detonate;
	};

	inline std::array< c_nadepoint, 500 > _points{ };
	inline bool _predicted = false;
	void predict(c_usercmd* ucmd);
	bool detonated(weapon_t* weapon, float time, trace_t& trace);
	void run(c_usercmd* ucmd);
	void draw();
}
