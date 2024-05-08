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
	bool get_playerbox(player_t* entity, bbox_t& in);

	void get_update_sounds();

	void throwed_grenade(entity_t* entity);
	void dropped_weapon(entity_t* entity);

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

	void particles();
	void entities_ragdoll();
	void skybox_changer(client_frame_stage_t stage);
	void flashalpha();
	void nosmoke();
	void draw_scope();
	void dlights(player_t* entity);
	void run_freecam(c_usercmd* cmd, vec3_t angles);
	void freecam(view_setup_t* setup);
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
