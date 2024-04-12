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

	void velocity_indicator();
	void stamina_indicator();
	void keys_indicator();
	void add(std::string name, bool enabled, color_t clr);
	void indicators();

	struct indicator
	{
		color_t clr;
		float alpha;
	};

	inline std::unordered_map<std::string, indicator> m_indicators;
}