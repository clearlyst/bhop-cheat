#pragma once
#include "../../../sdk/sdk.hpp"

using md5_pseudo_random_fn = uint32_t(__thiscall*)(uint32_t);

namespace prediction
{
	void restore_ent_to_predicted_frame(int predicted_frame);
	void start(c_usercmd* cmd);
	void stop();
	void updatepacket();

	inline int flags;
	inline int movetype;
	inline vec3_t velocity;
	inline vec3_t origin;
	inline float curtime;
	inline float frametime;
	inline int tickcount;
	inline bool in_prediction = false;
	inline bool first_time_predicted = false;
	inline md5_pseudo_random_fn md5_pseudo_random;
	inline int* prediction_random_seed;
	inline player_t** prediction_player;
	inline CMoveData move_data;
	inline bool using_prediction;
}