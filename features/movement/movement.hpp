#pragma once
#include "../../sdk/sdk.hpp"
#include <filesystem>
#include <cstddef>
#include <cstdio>

namespace features::movement
{
	inline bool should[9] = { false, false, false, false, false, false, false, false, false };
	inline int saved_tick[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	inline bool should_duck;
	inline bool should_edgebug;
	inline bool should_pixelsurf;

	struct ps_data_t {
		bool predicted_ps = false;
		bool pixelsurfing = false;
		bool pixelducking = false;
		int counter = 0;
		int pixeltick = 0;
		float last_set = 0.f;
		c_usercmd* pixelsurf_cmd = nullptr;
		vec3_t view_point = vec3_t(0, 0, 0);
		float forward_move = 0.f;
		float side_move = 0.f;
	};

	inline ps_data_t ps_data;

	void bhop(c_usercmd* cmd);
	void delay_hop(c_usercmd* cmd);
	void jump_bug(c_usercmd* cmd);
	bool advanced_detection_for_edgebug(c_usercmd* cmd);
	void edge_bug(c_usercmd* cmd);
	void edge_jump(c_usercmd* cmd);
	void long_jump(c_usercmd* cmd );
	void ladder_jump(c_usercmd* cmd);
	void null_strafing(c_usercmd* cmd);
	void mini_jump(c_usercmd* cmd);
	void auto_strafe(c_usercmd* cmd, vec3_t& current_angle);
	void block_bot(c_usercmd* cmd);
	void strafe_optimizer(c_usercmd* user_cmd);
	void mouse_strafe_limiter(float* x, float* y);
	void auto_align(c_usercmd* cmd);
	void auto_pixelsurf(c_usercmd* cmd);

	void velocity_indicator();
	void stamina_indicator();
	void keys_indicator();
	void add(std::string name, bool enabled, color_t color);
	void indicators();
	void graphs_data();
	void velocity_graph_indicator();
	void stamina_graph_indicator();

	inline ImVec2 graph_position;

	struct pixelsurf_data_t
	{
		bool pixelsurf_ducked = false;
		int pixelsurf_ticks = 0;
	};

	struct velocity_data_t
	{
		int speed;
		bool on_ground;
		bool pixelsurfed;
		bool jumpbugged;
		bool edgebugged;
		bool was_in_prediction;
	};

	struct stamina_data_t
	{
		float stamina;
		bool on_ground;
	};

	inline std::vector<velocity_data_t> velocity_data;
	inline std::vector<stamina_data_t> stamina_data;
	inline std::unordered_map<std::string, color_t> indicators_data;
}