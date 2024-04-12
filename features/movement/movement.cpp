#define NOMINMAX
#include "movement.hpp"
#include "../../menu/menu.hpp"
#include "../../menu/config/config.hpp"
#include "../movement/prediction/prediction.hpp"
#include "../notifications/notifications.hpp"

void features::movement::bhop(c_usercmd* cmd) 
{
	if (!c::movement::bhop)
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	if (menu::checkkey(c::movement::jump_bug_key, c::movement::jump_bug_key_s))
	{
		return;
	}

	if (!(g::local->flags() & fl_onground)) 
	{
		cmd->buttons &= ~in_jump;
	}
}

void features::movement::delay_hop(c_usercmd* cmd) 
{
	if (!c::movement::delay_hop || !menu::checkkey(c::movement::delay_hop_key, c::movement::delay_hop_key_s))
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	static int ticks = 0;
	const int ticks_to_wait = c::movement::dh_tick;

	if (g::local->flags() & fl_onground) 
	{
		if (cmd->tick_count > ticks + ticks_to_wait) 
		{
			cmd->buttons |= in_jump;

			should[5] = true;

			ticks = cmd->tick_count;
		}
	}
	else 
	{
		should[5] = false;

		ticks = cmd->tick_count;
	}
}

void features::movement::edge_jump(c_usercmd* cmd) 
{
	if (!c::movement::edge_jump || !menu::checkkey(c::movement::edge_jump_key, c::movement::edge_jump_key_s))
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	if ((prediction::flags & fl_onground) && (!(g::local->flags() & fl_onground)))
	{
		if (c::movement::adaptive_key_cancelling && c::movement::adaptive_key_for[0])
		{
			if (c::movement::lj_null[0]) 
			{
				cmd->buttons &= ~in_forward;
				interfaces::engine->execute_cmd(xs("-forward"));
			}

			if (c::movement::lj_null[1]) 
			{
				cmd->buttons &= ~in_back;
				interfaces::engine->execute_cmd(xs("-back"));
			}

			if (c::movement::lj_null[2]) 
			{
				cmd->buttons &= ~in_moveleft;
				interfaces::engine->execute_cmd(xs("-moveleft"));
			}

			if (c::movement::lj_null[3]) 
			{
				cmd->buttons &= ~in_moveright;
				interfaces::engine->execute_cmd(xs("-moveright"));

			}
		}

		cmd->buttons |= in_jump;
		should[1] = true;
	}
	else 
	{
		should[1] = false;
	}
}

void features::movement::long_jump(c_usercmd* cmd)
{
	if (!c::movement::long_jump || !menu::checkkey(c::movement::long_jump_key, c::movement::long_jump_key_s))
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	static int tick;
	static bool should_duck;

	if ((prediction::flags & fl_onground) && (!(g::local->flags() & fl_onground)))
	{
		tick = cmd->tick_count + 2;
		should_duck = true;
		cmd->buttons |= in_duck;
		should[2] = true;

		if (c::movement::adaptive_edge_jump && c::movement::adaptive_edge_jump_for[0])
		{
			cmd->buttons |= in_jump;
		}

		if (c::movement::adaptive_key_cancelling && c::movement::adaptive_key_for[3])
		{
			if (c::movement::lj_null[0])
			{
				cmd->buttons &= ~in_forward;
				interfaces::engine->execute_cmd(xs("-forward"));
			}

			if (c::movement::lj_null[1])
			{
				cmd->buttons &= ~in_back;
				interfaces::engine->execute_cmd(xs("-back"));
			}

			if (c::movement::lj_null[2])
			{
				cmd->buttons &= ~in_moveleft;
				interfaces::engine->execute_cmd(xs("-moveleft"));
			}

			if (c::movement::lj_null[3])
			{
				cmd->buttons &= ~in_moveright;
				interfaces::engine->execute_cmd(xs("-moveright"));
			}
		}
	}
	else
	{
		should[2] = false;
	}

	if (should_duck && (cmd->tick_count < tick))
	{
		cmd->buttons |= in_duck;
	}
	else
	{
		should_duck = false;
	}
}

void features::movement::mini_jump(c_usercmd* cmd) 
{
	if (!c::movement::mini_jump || !menu::checkkey(c::movement::mini_jump_key, c::movement::mini_jump_key_s))
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	if ((prediction::flags & fl_onground) && (!(g::local->flags() & fl_onground)))
	{
		if (c::movement::adaptive_key_cancelling && c::movement::adaptive_key_for[1]) 
		{
			if (c::movement::lj_null[0])
			{
				cmd->buttons &= ~in_forward;
				interfaces::engine->execute_cmd(xs("-forward"));
			}

		    if (c::movement::lj_null[1])
		    {
		    	cmd->buttons &= ~in_back;
		    	interfaces::engine->execute_cmd(xs("-back"));
		    }
		    
		    if (c::movement::lj_null[2]) 
		    {
		    	cmd->buttons &= ~in_moveleft;
		    	interfaces::engine->execute_cmd(xs("-moveleft"));
		    }
		    
		    if (c::movement::lj_null[3]) 
		    {
		    	cmd->buttons &= ~in_moveright;
		    	interfaces::engine->execute_cmd(xs("-moveright"));
		    }
		}

		if (c::movement::adaptive_edge_jump && c::movement::adaptive_edge_jump_for[1])
		{
			cmd->buttons |= in_jump;
		}

		should[4] = true;

		cmd->buttons |= in_duck;
	}
	else 
	{
		should[4] = false;
	}
}

void features::movement::ladder_jump(c_usercmd* cmd) 
{
	if (!c::movement::ladder_edge_jump || !menu::checkkey(c::movement::ladder_edge_jump_key, c::movement::ladder_edge_jump_key_s))
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_observer)
	{
		return;
	}

	if (g::local->flags() & fl_onground)
	{
		return;
	}

	static int tick;
	const int pre = g::local->move_type();

	prediction::start(cmd);
	prediction::stop();

	const int post = g::local->move_type();

	if (pre == movetype_ladder)
	{
		if (post != movetype_ladder)
		{
			tick = interfaces::globals->tick_count;

			if (c::movement::adaptive_edge_jump && c::movement::adaptive_edge_jump_for[2])
			{
				cmd->buttons |= in_jump;
			}

			if (c::movement::adaptive_key_cancelling && c::movement::adaptive_key_for[4])
			{
				cmd->forward_move = 0.0f;
				cmd->side_move = 0.0f;
				cmd->buttons = cmd->buttons & ~(in_forward | in_back | in_moveright | in_moveleft);
			}

			should[9] = true;
		}
	}

	if (interfaces::globals->tick_count - tick > 1 && interfaces::globals->tick_count - tick < 15)
	{
		if (c::movement::adaptive_key_cancelling && c::movement::adaptive_key_for[4])
		{
			cmd->forward_move = 0.0f;
			cmd->side_move = 0.0f;
			cmd->buttons = cmd->buttons & ~(in_forward | in_back | in_moveright | in_moveleft);
		}

		cmd->buttons |= in_duck;

		should[9] = false;
	}
}

void features::movement::null_strafing(c_usercmd* cmd) 
{
	if (!c::movement::null_strafing)
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	if (g::local->flags() & fl_onground)
	{
		return;
	}

	if (cmd->mouse_dx > 0 && cmd->buttons & in_moveright && cmd->buttons & in_moveleft)
	{
		cmd->side_move = -450.f;
	}
	else if (cmd->mouse_dx < 0 && cmd->buttons & in_moveleft && cmd->buttons & in_moveright)
	{
		cmd->side_move = 450.f;
	}
}

void features::movement::block_bot(c_usercmd* cmd)
{
	if (!c::movement::block_bot || !menu::checkkey(c::movement::block_bot_key, c::movement::block_bot_key_s))
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	float best_dist = std::numeric_limits<float>::max();
	int best_target = -1;

	for (int i = 1; i < interfaces::globals->max_clients; i++)
	{
		auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(i));

		if (!entity || entity->dormant() || !entity->is_alive() || entity == g::local)
		{
			continue;
		}

		float dist = g::local->origin().distance_to(entity->origin());

		if (dist < best_dist)
		{
			best_dist = dist;
			best_target = i;
		}
	}

	auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(best_target));

	if (!entity)
	{
		return;
	}

	const float speed = entity->velocity().length();
	const auto forward = entity->origin() - g::local->origin();

	if (entity->get_bone_position(6).z < g::local->origin().z && g::local->origin().distance_to(entity->origin()) < 100.f)
	{
		cmd->forward_move = ((sin(DirectX::XMConvertToRadians((cmd->view_angles.y)) * forward.y) + (cos(DirectX::XMConvertToRadians(cmd->view_angles.y)) * forward.x)) * speed);
		cmd->side_move = ((cos(DirectX::XMConvertToRadians((cmd->view_angles.y)) * -forward.y) + (sin(DirectX::XMConvertToRadians(cmd->view_angles.y)) * forward.x)) * speed);
	}
	else
	{
		auto yaw_delta = (atan2(forward.y, forward.x) * 180.0f / 3.14159265359f) - cmd->view_angles.y;

		if (yaw_delta > 180)
		{
			yaw_delta -= 360;
		}
		else if (yaw_delta < -180)
		{
			yaw_delta += 360;
		}

		if (yaw_delta > 0.25)
		{
			cmd->side_move = -speed;
		}
		else if (yaw_delta < -0.25)
		{
			cmd->side_move = speed;
		}
	}
}

void features::movement::auto_strafe(c_usercmd* cmd, vec3_t& current_angle)
{
	if (!c::movement::auto_strafe || !menu::checkkey(c::movement::auto_strafe_key, c::movement::auto_strafe_key_s))
	{
		return;
	}

	if (c::movement::strafe_optimizer || menu::checkkey(c::movement::strafe_optimizer_key, c::movement::strafe_optimizer_key_s))
	{
		return;
	}

	if (c::movement::mouse_strafe_limiter || menu::checkkey(c::movement::mouse_strafe_limiter_key, c::movement::mouse_strafe_limiter_key_s))
	{
		return;
	}

	if (menu::checkkey(c::misc::freecam_key, c::misc::freecam_key_s) || menu::checkkey(c::movement::edge_bug_key, c::movement::edge_bug_key_s) || menu::checkkey(c::movement::pixel_surf_key, c::movement::pixel_surf_key_s))
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	if (g::local->flags() & fl_onground)
	{
		return;
	}

	if (g::local->get_velocity().length_2d() <= 10.0f)
	{
		return;
	}

	static float angle = 0.f;

	if (cmd->buttons & in_back) 
	{
		angle = -180.f;

		if (cmd->buttons & in_moveleft)
		{
			angle -= 45.f;
		}
		else if (cmd->buttons & in_moveright)
		{
			angle += 45.f;
		}
	}
	else if (cmd->buttons & in_moveleft)
	{
		angle = 90.f;

		if (cmd->buttons & in_back)
		{
			angle += 45.f;
		}
		else if (cmd->buttons & in_forward)
		{
			angle -= 45.f;
		}
	}
	else if (cmd->buttons & in_moveright)
	{
		angle = -90.f;

		if (cmd->buttons & in_back)
		{
			angle -= 45.f;
		}
		else if (cmd->buttons & in_forward)
		{
			angle += 45.f;
		}
	}
	else 
	{
		angle = 0.f;
	}

	current_angle.y += angle;

	cmd->forward_move = 0.f;
	cmd->side_move = 0.f;

	auto velocity = g::local->get_velocity();
	const auto delta = math::normalize_yaw(current_angle.y - math::rad2deg(std::atan2(velocity.y, velocity.x)));

	cmd->side_move = delta > 0.f ? -450.f : 450.f;

	current_angle.y = math::normalize_yaw(current_angle.y - delta);
}

void features::movement::strafe_optimizer(c_usercmd* cmd) 
{
	if (!c::movement::strafe_optimizer || !menu::checkkey(c::movement::strafe_optimizer_key, c::movement::strafe_optimizer_key_s))
	{
		return;
	}

	if (c::movement::auto_strafe || menu::checkkey(c::movement::auto_strafe_key, c::movement::auto_strafe_key_s))
	{
		return;
	}

	if (c::movement::mouse_strafe_limiter || menu::checkkey(c::movement::mouse_strafe_limiter_key, c::movement::mouse_strafe_limiter_key_s))
	{
		return;
	}

	if (menu::checkkey(c::misc::freecam_key, c::misc::freecam_key_s) || menu::checkkey(c::movement::edge_bug_key, c::movement::edge_bug_key_s) || menu::checkkey(c::movement::pixel_surf_key, c::movement::pixel_surf_key_s))
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	if (g::local->flags() & fl_onground)
	{
		return;
	}

	static auto m_yaw = interfaces::console->get_convar("m_yaw");
	static auto sensitivity = interfaces::console->get_convar("sensitivity");
	static float previous_view_angles_yaw = cmd->view_angles.y;
	vec3_t velocity = g::local->velocity();

	if ((velocity.length_2d() > c::movement::strafe_optimizer_minimum_speed) || (velocity.length_2d() < c::movement::strafe_optimizer_maximum_speed))
	{

		float mouse_yaw_factor = m_yaw->get_float();
		float mouse_sensitivity = sensitivity->get_float();
		float mouse_yaw_step = mouse_sensitivity * mouse_yaw_factor;

		if (cmd->side_move < 0)
		{
			if (previous_view_angles_yaw - cmd->view_angles[1] < 0)
			{
				float strafe_angle = remainderf(cmd->view_angles[1] - atan2f(velocity[1], velocity[0]) * 180 / m_pi, 360) * c::movement::strafe_optimizer_gain / 100;

				if (strafe_angle < -mouse_yaw_step)
				{
					if (strafe_angle < -180)
					{
						strafe_angle = -180;
					}

					cmd->view_angles[1] = remainderf(cmd->view_angles[1] - mouse_yaw_step * roundf(strafe_angle / mouse_yaw_step), 360);
					cmd->mouse_dx = (__int16)(mouse_sensitivity * ceilf(remainderf(previous_view_angles_yaw - cmd->view_angles[1], 360) / sqrtf(mouse_yaw_step)));

					interfaces::engine->set_view_angles(cmd->view_angles);
				}
			}
		}
		else if (cmd->side_move > 0)
		{
			if (previous_view_angles_yaw - cmd->view_angles[1] > 0)
			{
				float strafe_angle = remainderf(cmd->view_angles[1] - atan2f(velocity[1], velocity[0]) * 180 / m_pi, 360) * c::movement::strafe_optimizer_gain / 100;

				if (strafe_angle > mouse_yaw_step)
				{
					if (strafe_angle > 180)
					{
						strafe_angle = 180;
					}

					cmd->view_angles[1] = remainderf(cmd->view_angles[1] - mouse_yaw_step * roundf(strafe_angle / mouse_yaw_step), 360);
					cmd->mouse_dx = (__int16)(mouse_sensitivity * ceilf(remainderf(previous_view_angles_yaw - cmd->view_angles[1], 360) / sqrtf(mouse_yaw_step)));
					interfaces::engine->set_view_angles(cmd->view_angles);
				}
			}
		}
	}

	previous_view_angles_yaw = cmd->view_angles[1];
}

void features::movement::mouse_strafe_limiter(float* x, float* y) 
{
	if (!c::movement::mouse_strafe_limiter || !menu::checkkey(c::movement::mouse_strafe_limiter_key, c::movement::mouse_strafe_limiter_key_s))
	{
		return;
	}

	if (c::movement::auto_strafe || menu::checkkey(c::movement::auto_strafe_key, c::movement::auto_strafe_key_s))
	{
		return;
	}

	if (c::movement::strafe_optimizer || menu::checkkey(c::movement::strafe_optimizer_key, c::movement::strafe_optimizer_key_s))
	{
		return;
	}

	if (menu::checkkey(c::misc::freecam_key, c::misc::freecam_key_s) || menu::checkkey(c::movement::edge_bug_key, c::movement::edge_bug_key_s) || menu::checkkey(c::movement::pixel_surf_key, c::movement::pixel_surf_key_s))
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	if (g::local->flags() & fl_onground)
	{
		return;
	}

	*x = std::clamp<float>(*x, -c::movement::mouse_strafe_limiter_value, c::movement::mouse_strafe_limiter_value);

	if (c::movement::mouse_strafe_limiter_affect_pitch)
	{
		*y = std::clamp<float>(*y, -c::movement::mouse_strafe_limiter_value, c::movement::mouse_strafe_limiter_value);
	}
}

void features::movement::jump_bug(c_usercmd* cmd) 
{
	if (!c::movement::jump_bug || !menu::checkkey(c::movement::jump_bug_key, c::movement::jump_bug_key_s))
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	if (!(prediction::flags & fl_onground) && g::local->flags() & fl_onground)
	{
		cmd->buttons |= in_duck;
		cmd->buttons &= ~in_jump;		
	}

	should[0] = (!(prediction::flags & fl_onground) && !(g::local->flags() & fl_onground) && !(should[3]) && !(should[6]) && (g::local->velocity().z > prediction::velocity.z) && g::local->move_type() != movetype_ladder && g::local->move_type() != movetype_noclip && g::local->move_type() != movetype_observer ? true : false);

	if (c::movement::detection_printf[0])
	{
		if (should[0])
		{
			features::notification::run("done jumpbug", "#_print_jumpbug", true, true, true);
		}
	}
}

bool features::movement::advanced_detection_for_edgebug(c_usercmd* cmd)
{
	if (prediction::velocity.z < -5.62895 && g::local->velocity().z > prediction::velocity.z && g::local->velocity().z < -8.293333)
	{
		float velocty_before_engineprediction = g::local->velocity().z;

		prediction::start(cmd);
		prediction::stop();

		static float get_gravity_float = interfaces::console->get_convar("sv_gravity")->get_float();
		float gravity_velocity_constant = roundf(-get_gravity_float * interfaces::globals->interval_per_tick + velocty_before_engineprediction);

		if (gravity_velocity_constant == round(g::local->velocity().z))
		{
			return true;
		}
	}

	static convar* sv_gravity = interfaces::console->get_convar("sv_gravity");

	float gravity_vel = (sv_gravity->get_float() * 0.5f * interfaces::globals->interval_per_tick);

	if (prediction::velocity.z < -gravity_vel && round(g::local->velocity().z) == -round(gravity_vel))
	{
		return true;
	}

	return false;
}

void features::movement::edge_bug(c_usercmd* cmd)
{
	bool crouching;
	bool strafing;
	int original_buttons = cmd->buttons;
	float original_forwardmove = cmd->forward_move;
	float original_sidemove = cmd->side_move;
	float forwardmove, sidemove;

	if (!should_edgebug)
	{
		forwardmove = original_forwardmove;
		sidemove = original_sidemove;
	}

	if (!c::movement::edge_bug || !menu::checkkey(c::movement::edge_bug_key, c::movement::edge_bug_key_s))
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	for (int type = 0; type < 2; type++)
	{
		prediction::restore_ent_to_predicted_frame(interfaces::prediction->m_nCommandsPredicted() - 1);

		if (type == 0)
		{
			crouching = false;
			strafing = false;
		}
		else if (type == 1)
		{
			crouching = true;
			strafing = false;
		}

		for (int ticks = 0; ticks <= 64; ticks++)
		{
			if (strafing)
			{
				cmd->forward_move = forwardmove;
				cmd->side_move = sidemove;
			}
			else
			{
				cmd->forward_move = 0.0f;
				cmd->side_move = 0.0f;
			}

			if (crouching)
			{
				cmd->buttons |= in_duck;
			}
			else
			{
				cmd->buttons &= ~in_duck;
			}

			prediction::start(cmd);
			should_edgebug = advanced_detection_for_edgebug(cmd);
			prediction::stop();

			if (g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_noclip || g::local->flags() & fl_onground)
			{
				break;
			}

			if (should_edgebug)
			{
				if (strafing)
				{
					cmd->forward_move = forwardmove;
					cmd->side_move = sidemove;
				}

				should_duck = crouching;

				should[3] = should_edgebug;

				return;
			}
		}
	}

	cmd->forward_move = original_forwardmove;
	cmd->side_move = original_sidemove;
	cmd->buttons = original_buttons;
}

void features::movement::auto_align(c_usercmd* cmd)
{
	if (!c::movement::auto_align)
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

	if (g::local->move_type() == movetype_noclip || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_observer)
	{
		return;
	}

	if (g::local->flags() & fl_onground)
	{
		return;
	}

	trace_t trace;
	ray_t ray;

	trace_world_only flt;

	const auto mins = g::local->collideable()->mins();
	const auto maxs = g::local->collideable()->maxs();

	for (float a = 0.0f; a < (m_pi * 2.0f); a += (m_pi * 2.0f) / 32.0f)
	{
	
	}
}

color_t interpolate(const color_t& first_color, const color_t& second_color, const float time)
{
	return color_t(
		first_color.r() + time * (second_color.r() - first_color.r()), 
		first_color.g() + time * (second_color.g() - first_color.g()),
		first_color.b() + time * (second_color.b() - first_color.b()),
		first_color.a() + time * (second_color.a() - first_color.a())
	);
}

void features::movement::velocity_indicator( )
{
	if (!c::movement::indicators::velocity::enable)
	{
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	static bool didjumplast = false;
	static int last_delta_velocity = 0;
	static int last_velocity = 0;
	static int pre_velocity = 0;
	static int lasttick = 0;

	int velocity = round(g::local->velocity().length_2d());

	auto velocity_color = [](int velocity) -> color_t
	{
		if (velocity > 0)
		{
			return color_t(c::movement::indicators::velocity::color_3[0], c::movement::indicators::velocity::color_3[1], c::movement::indicators::velocity::color_3[2], c::movement::indicators::velocity::color_3[3]);
		}
		else if (velocity < 0)
		{
			return color_t(c::movement::indicators::velocity::color_4[0], c::movement::indicators::velocity::color_4[1], c::movement::indicators::velocity::color_4[2], c::movement::indicators::velocity::color_4[3]);
		}
		else if (velocity > -1 && velocity < 1)
		{
			return color_t(c::movement::indicators::velocity::color_5[0], c::movement::indicators::velocity::color_5[1], c::movement::indicators::velocity::color_5[2], c::movement::indicators::velocity::color_5[3]);
		}
	};

	static float velocity_alpha = 0.0f;
	velocity_alpha = 1.f * (float(velocity) / 255.f);

	color_t color_1 = color_t(c::movement::indicators::velocity::color_1[0], c::movement::indicators::velocity::color_1[1], c::movement::indicators::velocity::color_1[2], c::movement::indicators::velocity::color_1[3]);
	color_t color_2 = color_t(c::movement::indicators::velocity::color_2[0], c::movement::indicators::velocity::color_2[1], c::movement::indicators::velocity::color_2[2], c::movement::indicators::velocity::color_2[3]);

	std::string velocity_str = std::to_string(velocity);
	std::string place_holder_velocity = "";

	last_delta_velocity = velocity - last_velocity;

	if (g::local->flags() & fl_onground)
	{
		if (didjumplast)
		{
			didjumplast = false;
		}

		place_holder_velocity = velocity_str;
	}
	else
	{
		if (!didjumplast)
		{
			pre_velocity = velocity;
			didjumplast = true;
		}

		if (c::movement::indicators::velocity::takeoff)
		{
			place_holder_velocity = velocity_str + (" (") + std::to_string(pre_velocity) + (")");
		}
		else
		{
			place_holder_velocity = velocity_str;
		}
	}

	im_render.text(g::width / 2, (g::height / 2) + c::movement::indicators::position, c::fonts::indi_size, fonts::indicator_font, place_holder_velocity.c_str(), true, c::movement::indicators::velocity::custom_color ? interpolate(color_1, color_2, velocity_alpha) : velocity_color(last_delta_velocity), c::fonts::indi_font_flag[9], c::fonts::indi_font_flag[10]);

	if (fabs(interfaces::globals->tick_count - lasttick) > 5)
	{
		lasttick = interfaces::globals->tick_count;
		last_velocity = velocity;
	}
}

void features::movement::stamina_indicator( )
{
	if (!c::movement::indicators::stamina::enable)
	{
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}

	static float last_speed = 0, last_jump_speed = 0;
	static float last_vel_update = 0.0f;
	float current_speed = g::local->stamina();
	static bool last_onground = false;
	bool current_onground = g::local->flags() & fl_onground;

	if (last_onground && !current_onground)
	{
		last_jump_speed = current_speed;
	}

	std::string value_str;

	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << current_speed;
	value_str = ss.str();

	std::string str = value_str;

	std::stringstream ss1;
	ss1 << std::fixed << std::setprecision(1) << last_jump_speed;
	std::string value_str2 = ss1.str();

	if (current_onground || g::local->move_type() == movetype_ladder)
	{
		str = value_str;
	}
	else if (c::movement::indicators::stamina::takeoff && !(current_onground))
	{
		str += " (" + value_str2 + ")";
	}

	static float time = 0.f;
	time = 1.f * (float(g::local->stamina()) / 35.f);

	color_t color = color_t(c::movement::indicators::stamina::color[0], c::movement::indicators::stamina::color[1], c::movement::indicators::stamina::color[2], c::movement::indicators::stamina::color[3]);
	color_t color_1 = color_t(c::movement::indicators::stamina::color_1[0], c::movement::indicators::stamina::color_1[1], c::movement::indicators::stamina::color_1[2], c::movement::indicators::stamina::color_1[3]);
	color_t color_2 = color_t(c::movement::indicators::stamina::color_2[0], c::movement::indicators::stamina::color_2[1], c::movement::indicators::stamina::color_2[2], c::movement::indicators::stamina::color_2[3]);

	im_render.text(g::width / 2, c::movement::indicators::velocity::enable ? (g::height / 2) + c::movement::indicators::position + c::fonts::indi_size : (g::height / 2) + c::movement::indicators::position, c::fonts::indi_size, fonts::indicator_font, str.c_str(), true, c::movement::indicators::stamina::custom_color ? interpolate(color_1, color_2, time) : color, c::fonts::indi_font_flag[9], c::fonts::indi_font_flag[10]);

	if (interfaces::globals->tick_count > last_vel_update)
	{
		last_speed = current_speed;
		last_vel_update = interfaces::globals->tick_count + 0.05f;
	}

	last_onground = current_onground;
}

void features::movement::keys_indicator()
{
	if (!c::movement::indicators::keys::enable || c::movement::indicators::keys::background)
	{
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		return;
	}
	
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
	{
		return;
	}

	c_usercmd* cmd = g::cmd;

	if (c::movement::indicators::keys::under_line)
	{
		im_render.text(g::width / 2 + c::fonts::sub_indi_size, c::movement::indicators::keys::position - 1, c::fonts::sub_indi_size, fonts::sub_indicator_font, "_", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
		im_render.text(g::width / 2 - c::fonts::sub_indi_size, c::movement::indicators::keys::position - 1, c::fonts::sub_indi_size, fonts::sub_indicator_font, "_", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
		im_render.text(g::width / 2, c::movement::indicators::keys::position - 1, c::fonts::sub_indi_size, fonts::sub_indicator_font, "_", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
		im_render.text(g::width / 2 - c::fonts::sub_indi_size, (c::movement::indicators::keys::position - 1) + c::fonts::sub_indi_size, c::fonts::sub_indi_size, fonts::sub_indicator_font, "_", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
		im_render.text(g::width / 2, (c::movement::indicators::keys::position - 1) + c::fonts::sub_indi_size, c::fonts::sub_indi_size, fonts::sub_indicator_font, "_", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
		im_render.text(g::width / 2 + c::fonts::sub_indi_size, (c::movement::indicators::keys::position - 1) + c::fonts::sub_indi_size, c::fonts::sub_indi_size, fonts::sub_indicator_font, "_", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
	}

	if (cmd->buttons & in_jump)
	{
		im_render.text(g::width / 2 + c::fonts::sub_indi_size, c::movement::indicators::keys::position - 1, c::fonts::sub_indi_size, fonts::sub_indicator_font, "J", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
	}

	if (cmd->buttons & in_duck)
	{
		im_render.text(g::width / 2 - c::fonts::sub_indi_size, c::movement::indicators::keys::position - 1, c::fonts::sub_indi_size, fonts::sub_indicator_font, "C", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
	}

	if (cmd->buttons & in_forward)
	{
		im_render.text(g::width / 2, c::movement::indicators::keys::position - 1, c::fonts::sub_indi_size, fonts::sub_indicator_font, "W", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
	}

	if (cmd->buttons & in_back)
	{
		im_render.text(g::width / 2, (c::movement::indicators::keys::position - 1) + c::fonts::sub_indi_size, c::fonts::sub_indi_size, fonts::sub_indicator_font, "S", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
	}

	if (c::movement::indicators::keys::nulling)
	{
		if ((cmd->buttons & in_moveleft) && (cmd->buttons & in_moveright))
		{
			im_render.text(g::width / 2 - c::fonts::sub_indi_size, (c::movement::indicators::keys::position - 1) + c::fonts::sub_indi_size, c::fonts::sub_indi_size, fonts::sub_indicator_font, "A", true, color_t(c::movement::indicators::keys::color[0], c::movement::indicators::keys::color[1], c::movement::indicators::keys::color[2], 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
			im_render.text(g::width / 2 + c::fonts::sub_indi_size, (c::movement::indicators::keys::position - 1) + c::fonts::sub_indi_size, c::fonts::sub_indi_size, fonts::sub_indicator_font, "D", true, color_t(c::movement::indicators::keys::color[0], c::movement::indicators::keys::color[1], c::movement::indicators::keys::color[2], 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
		}
		else
		{
			if (cmd->buttons & in_moveleft)
			{
				im_render.text(g::width / 2 - c::fonts::sub_indi_size, (c::movement::indicators::keys::position - 1) + c::fonts::sub_indi_size, c::fonts::sub_indi_size, fonts::sub_indicator_font, "A", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
			}

			if (cmd->buttons & in_moveright)
			{
				im_render.text(g::width / 2 + c::fonts::sub_indi_size, (c::movement::indicators::keys::position - 1) + c::fonts::sub_indi_size, c::fonts::sub_indi_size, fonts::sub_indicator_font, "D", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
			}
		}
	}
	else
	{
		if (cmd->buttons & in_moveleft)
		{
			im_render.text(g::width / 2 - c::fonts::sub_indi_size, (c::movement::indicators::keys::position - 1) + c::fonts::sub_indi_size, c::fonts::sub_indi_size, fonts::sub_indicator_font, "A", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
		}

		if (cmd->buttons & in_moveright)
		{
			im_render.text(g::width / 2 + c::fonts::sub_indi_size, (c::movement::indicators::keys::position - 1) + c::fonts::sub_indi_size, c::fonts::sub_indi_size, fonts::sub_indicator_font, "D", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
		}
	}

	if (c::movement::indicators::keys::mouse_direction)
	{
		if (cmd->mouse_dx < 5.f)
		{
			im_render.text(g::width / 2 - c::fonts::sub_indi_size, (c::movement::indicators::keys::position - 1) + (c::fonts::sub_indi_size * 2), c::fonts::sub_indi_size, fonts::sub_indicator_font, "<", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
		}

		if (cmd->mouse_dx > 5.f)
		{
			im_render.text(g::width / 2 + c::fonts::sub_indi_size, (c::movement::indicators::keys::position - 1) + (c::fonts::sub_indi_size * 2), c::fonts::sub_indi_size, fonts::sub_indicator_font, ">", true, color_t(1.f, 1.f, 1.f, 1.f), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
		}
	}
}

void features::movement::add(std::string name, bool enabled, color_t clr)
{
	if (!m_indicators.count(name))
	{
		m_indicators.insert({ name,  { color_t(c::movement::indicators::binds::color[0], c::movement::indicators::binds::color[1], c::movement::indicators::binds::color[2]), 0}});
	}

	m_indicators[name].clr[0] = clr.r();
	m_indicators[name].clr[1] = clr.g();
	m_indicators[name].clr[2] = clr.b();

	if (enabled)
	{
		if (m_indicators[name].alpha < 1.f)
		{
			m_indicators[name].alpha += interfaces::globals->frame_time * c::movement::indicators::binds::speed;
		}

		if (m_indicators[name].alpha > 1.f)
		{
			m_indicators[name].alpha = 1.f;
		}
	}
	else
	{
		if (m_indicators[name].alpha > 0.f)
		{
			m_indicators[name].alpha -= interfaces::globals->frame_time * c::movement::indicators::binds::speed;
		}

		if (m_indicators[name].alpha < 0.f)
		{
			m_indicators[name].alpha = 0.f;
		}
	}

	m_indicators[name].clr.setAlpha(m_indicators[name].alpha);
}

void features::movement::indicators() 
{
	if (!c::movement::indicators::binds::enable)
	{
		saved_tick[0] = 0;
		saved_tick[1] = 0;
		saved_tick[2] = 0;
		saved_tick[3] = 0;
		saved_tick[4] = 0;
		saved_tick[5] = 0;
		saved_tick[6] = 0;
		saved_tick[7] = 0;
		saved_tick[8] = 0;
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		saved_tick[0] = 0;
		saved_tick[1] = 0;
		saved_tick[2] = 0;
		saved_tick[3] = 0;
		saved_tick[4] = 0;
		saved_tick[5] = 0;
		saved_tick[6] = 0;
		saved_tick[7] = 0;
		saved_tick[8] = 0;
		return;
	}

	if (!g::local || !g::local->is_alive())
	{
		saved_tick[0] = 0;
		saved_tick[1] = 0;
		saved_tick[2] = 0;
		saved_tick[3] = 0;
		saved_tick[4] = 0;
		saved_tick[5] = 0;
		saved_tick[6] = 0;
		saved_tick[7] = 0;
		saved_tick[8] = 0;
		return;
	}

	/* jb */
	if (should[0])
	{
		saved_tick[0] = interfaces::globals->tick_count;
	}
	/* ej */
	if (should[1])
	{
		saved_tick[1] = interfaces::globals->tick_count;
	}
	/* lj */
	if (should[2])
	{
		saved_tick[2] = interfaces::globals->tick_count;
	}
	/* eb */
	if (should[3])
	{
		saved_tick[3] = interfaces::globals->tick_count;
	}
	/* mj */
	if (should[4])
	{
		saved_tick[4] = interfaces::globals->tick_count;
	}
	/* sh */
	if (should[5])
	{
		saved_tick[5] = interfaces::globals->tick_count;
	}
	/* ps */
	if (should[6])
	{
		saved_tick[6] = interfaces::globals->tick_count;
	}
	/* al */
	if (should[7])
	{
		saved_tick[7] = interfaces::globals->tick_count;
	}
	/* ad */
	if (should[8])
	{
		saved_tick[8] = interfaces::globals->tick_count;
	}

	/* jb 1  */
	if (c::movement::indicators::binds::list[0])
	{
		add("jb", c::movement::jump_bug && menu::checkkey(c::movement::jump_bug_key, c::movement::jump_bug_key_s), interfaces::globals->tick_count - saved_tick[0] < 25 ? color_t(c::movement::indicators::binds::color_1[0], c::movement::indicators::binds::color_1[1], c::movement::indicators::binds::color_1[2]) : color_t(255, 255, 255));
	}
	/* ej 2  */
	if (c::movement::indicators::binds::list[1])
	{
		add("ej", c::movement::edge_jump && menu::checkkey(c::movement::edge_jump_key, c::movement::edge_jump_key_s), interfaces::globals->tick_count - saved_tick[1] < 25 ? color_t(c::movement::indicators::binds::color_1[0], c::movement::indicators::binds::color_1[1], c::movement::indicators::binds::color_1[2]) : color_t(255, 255, 255));
	}
	/* lj 3  */
	if (c::movement::indicators::binds::list[2])
	{
		add("lj", c::movement::long_jump && menu::checkkey(c::movement::long_jump_key, c::movement::long_jump_key_s), interfaces::globals->tick_count - saved_tick[2] < 25 ? color_t(c::movement::indicators::binds::color_2[0], c::movement::indicators::binds::color_2[1], c::movement::indicators::binds::color_2[2]) : color_t(255, 255, 255));
	}
	/* eb 4  */
	if (c::movement::indicators::binds::list[3])
	{
		add("eb", c::movement::edge_bug && menu::checkkey(c::movement::edge_bug_key, c::movement::edge_bug_key_s), interfaces::globals->tick_count - saved_tick[3] < 25 ? color_t(c::movement::indicators::binds::color_4[0], c::movement::indicators::binds::color_4[1], c::movement::indicators::binds::color_4[2]) : color_t(255, 255, 255));
	}
	/* mj 5 */
	if (c::movement::indicators::binds::list[4])
	{
		add("mj", c::movement::mini_jump && menu::checkkey(c::movement::mini_jump_key, c::movement::mini_jump_key_s), interfaces::globals->tick_count - saved_tick[4] < 25 ? color_t(c::movement::indicators::binds::color_2[0], c::movement::indicators::binds::color_2[1], c::movement::indicators::binds::color_2[2]) : color_t(255, 255, 255));
	}
	/* sh 6 */
	if (c::movement::indicators::binds::list[5])
	{
		add("sh", c::movement::delay_hop && menu::checkkey(c::movement::delay_hop_key, c::movement::delay_hop_key_s), interfaces::globals->tick_count - saved_tick[5] < 25 ? color_t(c::movement::indicators::binds::color_4[0], c::movement::indicators::binds::color_4[1], c::movement::indicators::binds::color_4[2]) : color_t(255, 255, 255));
	}
	/* ps 7 */
	if (c::movement::indicators::binds::list[6])
	{
		add("ps", c::movement::pixel_surf && menu::checkkey(c::movement::pixel_surf_key, c::movement::pixel_surf_key_s), interfaces::globals->tick_count - saved_tick[6] < 25 ? color_t(c::movement::indicators::binds::color_4[0], c::movement::indicators::binds::color_4[1], c::movement::indicators::binds::color_4[2]) : color_t(255, 255, 255));
	}
	/* al 8 */
	if (c::movement::indicators::binds::list[7])
	{
		add("al", c::movement::auto_align, interfaces::globals->tick_count - saved_tick[7] < 25 ? color_t(c::movement::indicators::binds::color_4[0], c::movement::indicators::binds::color_4[1], c::movement::indicators::binds::color_4[2]) : color_t(255, 255, 255));
	}
	/* ad 9 */
	if (c::movement::indicators::binds::list[8])
	{
		//add("ad", c::movement::auto_duck && menu::checkkey(c::movement::auto_duck_key, c::movement::auto_duck_key_s), interfaces::globals->tick_count - saved_tick[8] < 25 ? color_t(c::movement::indicators::binds::color_4[0], c::movement::indicators::binds::color_4[1], c::movement::indicators::binds::color_4[2]) : color_t(255, 255, 255));
	}
	/* as 10 */
	if (c::movement::indicators::binds::list[9])
	{
		add("as", c::movement::auto_strafe && menu::checkkey(c::movement::auto_strafe_key, c::movement::auto_strafe_key_s), color_t(255, 255, 255));
	}
	/* bb 11 */
	if (c::movement::indicators::binds::list[10])
	{
		add("bb", c::movement::block_bot && menu::checkkey(c::movement::block_bot_key, c::movement::block_bot_key_s), color_t(255, 255, 255));
	}
	/* msl 12 */
	if (c::movement::indicators::binds::list[11])
	{
		add("msl", c::movement::mouse_strafe_limiter && menu::checkkey(c::movement::mouse_strafe_limiter_key, c::movement::mouse_strafe_limiter_key_s), color_t(255, 255, 255));
	}
	/* so 13 */
	if (c::movement::indicators::binds::list[12])
	{
		add("so", c::movement::strafe_optimizer && menu::checkkey(c::movement::strafe_optimizer_key, c::movement::strafe_optimizer_key_s), color_t(255, 255, 255));
	}
	/* null 14 */
	if (c::movement::indicators::binds::list[13])
	{
		add("null", c::movement::null_strafing, color_t(255, 255, 255));
	}

	int position = 0;

	for (const auto& [name, data] : m_indicators)
	{
		if (data.alpha > 0.f)
		{
			if (c::movement::indicators::velocity::enable || c::movement::indicators::stamina::enable)
			{
				if (c::movement::indicators::velocity::enable && c::movement::indicators::stamina::enable)
				{
					im_render.text(g::width / 2, (g::height / 2) + c::movement::indicators::position + (c::fonts::indi_size * 2) + position, c::fonts::indi_size, fonts::indicator_font, name, true, data.clr, c::fonts::indi_font_flag[9], c::fonts::indi_font_flag[10]);
				}
				else
				{
					im_render.text(g::width / 2, (g::height / 2) + c::movement::indicators::position + c::fonts::indi_size + position, c::fonts::indi_size, fonts::indicator_font, name, true, data.clr, c::fonts::indi_font_flag[9], c::fonts::indi_font_flag[10]);
				}
			}
			else
			{
				im_render.text(g::width / 2, (g::height / 2) + c::movement::indicators::position + position, c::fonts::indi_size, fonts::indicator_font, name, true, data.clr, c::fonts::indi_font_flag[9], c::fonts::indi_font_flag[10]);
			}

			position += c::fonts::indi_size + c::movement::indicators::binds::sameline;
		}
	}
}