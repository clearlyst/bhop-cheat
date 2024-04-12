#include "../prediction/prediction.hpp"
#include "../movement.hpp"

void prediction::updatepacket()
{
	if (!g::local)
	{
		return;
	}

	if (interfaces::client_state->m_delta_tick > 0)
	{
		interfaces::prediction->Update(
			interfaces::client_state->m_delta_tick,
			interfaces::client_state->m_delta_tick > 0,
			interfaces::client_state->m_last_command_ack,
			interfaces::client_state->m_last_outgoing_command + interfaces::client_state->m_choked_commands);
	}
}


void prediction::restore_ent_to_predicted_frame(int predicted_frame)
{
	static auto fn = reinterpret_cast<DWORD(__stdcall*)(int, int)>(find_pattern("client.dll", "55 8B EC 8B 4D ? 56 E8 ? ? ? ? 8B 75"));
	fn(0, predicted_frame);
}

void post_think()
{
	interfaces::mdl_cache->begin_lock();

	if (g::local->is_alive())
	{
		g::local->UpdateCollisionBounds();

		if (g::local->flags() & fl_onground)
		{
			g::local->fall_velocity() = 0.f;
		}

		if (g::local->get_sequence() == -1)
		{
			g::local->SetSequence(0);
		}

		g::local->StudioFrameAdvance();

		g::local->post_think_v_physics();
	}
	g::local->simulate_player_simulated_entities();

	interfaces::mdl_cache->end_lock();
}

void prediction::start(c_usercmd* cmd) {
	if (!g::local || !cmd)
	{
		return;
	}

	using_prediction = true;

	*reinterpret_cast<c_usercmd**>(uint32_t(g::local) + 0x3348) = cmd;
	*reinterpret_cast<c_usercmd**>(uint32_t(g::local) + 0x3298) = cmd;

	if (!prediction_player)
	{
		prediction_player = *reinterpret_cast<player_t***>(find_pattern_2("client.dll", "89 35 ? ? ? ? F3 0F 10 48") + 0x2);
	}

	if (!prediction_random_seed)
	{
		prediction_random_seed = *reinterpret_cast<int**>(find_pattern_2("client.dll", "A3 ? ? ? ? 66 0F 6E 86") + 0x1);
	}

	if (!md5_pseudo_random)
	{
		md5_pseudo_random = reinterpret_cast<md5_pseudo_random_fn>(find_pattern_2("client.dll", "55 8B EC 83 E4 F8 83 EC 70 6A"));
	}

	*prediction_player = g::local;
	*prediction_random_seed = md5_pseudo_random(cmd->command_number) & 0x7FFFFFFF;

	flags = g::local->flags();
	velocity = g::local->velocity();
	origin = g::local->abs_origin();
	movetype = g::local->move_type();

	curtime = interfaces::globals->cur_time;
	frametime = interfaces::globals->frame_time;
	tickcount = interfaces::globals->tick_count;

	in_prediction = interfaces::prediction->m_bInPrediction();
	first_time_predicted = interfaces::prediction->m_bIsFirstTimePredicted();

	updatepacket();

	interfaces::prediction->m_bInPrediction() = true;
	interfaces::prediction->m_bIsFirstTimePredicted() = false;

	interfaces::globals->cur_time = interfaces::globals->interval_per_tick * g::local->get_tick_base();
	interfaces::globals->frame_time = interfaces::prediction->m_bEnginePaused() ? 0 : interfaces::globals->interval_per_tick;

	cmd->buttons |= g::local->ButtonForced();
	cmd->buttons &= ~(g::local->ButtonDisabled());

	interfaces::move_helper->SetHost(g::local);
	interfaces::game_movement->StartTrackPredictionErrors(g::local);

	if (cmd->impulse)
	{
		*reinterpret_cast<std::uint32_t*>(uintptr_t(g::local) + 0x320C) = cmd->impulse;
	}

	const int iButtons = cmd->buttons;
	const int nButtonsChanged = iButtons ^ *reinterpret_cast<int*>(uintptr_t(g::local) + 0x3208);
	
	*reinterpret_cast<int*>(uintptr_t(g::local) + 0x31FC) = (uintptr_t(g::local) + 0x3208);
	*reinterpret_cast<int*>(uintptr_t(g::local) + 0x3208) = iButtons;
	*reinterpret_cast<int*>(uintptr_t(g::local) + 0x3200) = iButtons & nButtonsChanged;
	*reinterpret_cast<int*>(uintptr_t(g::local) + 0x3204) = nButtonsChanged & ~iButtons;

	interfaces::prediction->CheckMovingGround(g::local, interfaces::globals->frame_time);
	interfaces::prediction->SetLocalViewAngle(cmd->view_angles);

	if (g::local->physics_run_think(0))
	{
		g::local->PreThink();
	}

	if (g::local->next_think_tick() != -1 && g::local->next_think_tick() > 0 && g::local->next_think_tick() <= g::local->get_tick_base())
	{
		g::local->next_think_tick() = -1;
		g::local->Think();
	}

	interfaces::prediction->SetupMove(g::local, cmd, interfaces::move_helper, &move_data);
	interfaces::game_movement->SetupMovementBounds(&move_data);
	interfaces::game_movement->ProcessMovement(g::local, &move_data);
	interfaces::prediction->FinishMove(g::local, cmd, &move_data);
	interfaces::move_helper->ProcessImpacts();

	post_think();

	interfaces::game_movement->FinishTrackPredictionErrors(g::local);
	interfaces::move_helper->SetHost(nullptr);
}

void prediction::stop() {
	interfaces::prediction->m_bInPrediction() = in_prediction;
	interfaces::prediction->m_bIsFirstTimePredicted() = first_time_predicted;

	interfaces::globals->cur_time = curtime;
	interfaces::globals->frame_time = frametime;
	interfaces::globals->tick_count = tickcount;

	*prediction_player = nullptr;
	*prediction_random_seed = -1;

	interfaces::game_movement->Reset();

	*reinterpret_cast<c_usercmd**>(uint32_t(g::local) + 0x3348) = NULL;

	using_prediction = false;
}
