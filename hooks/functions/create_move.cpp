#include "../../features/movement/prediction/prediction.hpp"
#include "../../features/movement/movement.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/aimbot/aimbot.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"
#include "../../sdk/math/math.hpp"
#include "../../sdk/sdk.hpp"
#include "../hooks.hpp"

bool __stdcall sdk::hooks::create_move::create_move(float sampletime, c_usercmd* cmd )
{
	sdk::hooks::create_move::ofunc(sampletime, cmd);

	if (!cmd || !cmd->command_number)
		return true;

	g::local = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	g::cmd = cmd;
	interfaces::engine->get_screen_size(g::width, g::height);

	i_net_channel* net_channel = interfaces::client_state->m_net_channel;

	features::grenade_prediction::run(cmd);
	features::misc::fix_mouse_delta(cmd);
	features::misc::clantag_spammer();
	features::misc::checkpoint_system();
	features::movement::graphs_data();

	if (features::movement::should_edgebug && features::movement::should_duck)
	{
		cmd->buttons |= in_duck;
	}

	if (c::movement::fastduck)
	{
		cmd->buttons |= in_bullrush;
	}

	features::movement::bhop(cmd);
	features::movement::delay_hop(cmd);

	prediction::start(cmd);
	{
		aimbot::run(cmd);
		triggerbot::run(cmd);
		backtrack.on_move(cmd);
	}
	prediction::stop();

	features::movement::edge_jump(cmd);
	features::movement::jump_bug(cmd);
	features::movement::edge_bug(cmd);
	features::movement::long_jump(cmd);
	features::movement::mini_jump(cmd);
	features::movement::ladder_jump(cmd);
	features::movement::auto_strafe(cmd, cmd->view_angles);
	features::movement::strafe_optimizer(cmd);
	features::visuals::run_freecam(cmd, cmd->view_angles);

	if (net_channel != nullptr) 
	{
		if (c::backtrack::fake_latency)
		{
			fakeping.update_sequence(net_channel);
		}
		else
		{
			fakeping.clear_sequence();
		}
	}

	return false;
}