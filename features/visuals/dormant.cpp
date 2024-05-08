#include "visuals.hpp"

void features::visuals::dormant_system::start() {
	CUtlVector<SndInfo_t> m_utlCurSoundList;
	interfaces::engine_sound->GetActiveSounds(m_utlCurSoundList);

	if (!m_utlCurSoundList.Count())
		return;

	static auto flSpawnTime = 0.f;

	if (!g::local)
		return;

	if (flSpawnTime != g::local->spawn_time()) {
		flSpawnTime = g::local->spawn_time();
		return;
	}

	for (int iter = 0; iter < m_utlCurSoundList.Count(); iter++) {
		SndInfo_t& sound = m_utlCurSoundList[iter];
		if (sound.m_nSoundSource == 0 || sound.m_nSoundSource > 64)   
			continue;

		auto player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(sound.m_nSoundSource));

		if (!player || player == g::local || sound.m_pOrigin->is_zero())
			continue;

		if (!is_valid_sound(sound))
			continue;

		restore_sound.emplace_back(*sound.m_pOrigin, iter);

		setup_adjust_player(player, sound);

		m_cSoundPlayers[sound.m_nSoundSource].override(sound);
	}

	for (int iter = 1; iter < interfaces::globals->max_clients; ++iter) {
		auto player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(iter));
		if (!player || !player->dormant() || !player->is_alive())
			continue;

		adjust_player_begin(player, iter);
	}

	m_utlvecSoundBuffer = m_utlCurSoundList;
}

void features::visuals::dormant_system::finish() {
	adjust_player_finish();
}

void features::visuals::dormant_system::adjust_player_begin(player_t* player, int i) {
	constexpr int EXPIRE_DURATION = 450; 
	auto& sound_player = m_cSoundPlayers[player->index()];
	bool sound_expired = GetTickCount() - sound_player.m_iReceiveTime > EXPIRE_DURATION;
	if (sound_expired)
		return;

	SoundPlayer current_player;
	current_player.playerindex = i;
	current_player.m_bDormant = true;
	current_player.m_nFlags = player->flags();
	current_player.m_vecOrigin = player->origin();
	current_player.m_vecAbsOrigin = player->get_absolute_origin();
	m_arRestorePlayers.emplace_back(current_player);

	if (!sound_expired)
		*(bool*)((uintptr_t)player + 0xED) = false; 

	player->flags() = sound_player.m_nFlags;
	player->origin() = sound_player.m_vecOrigin;
	player->set_abs_origin(sound_player.m_vecOrigin);
}

void features::visuals::dormant_system::adjust_player_finish() {
	for (auto& RestorePlayer : m_arRestorePlayers) {
		auto player = player_t::get_player_by_index(RestorePlayer.playerindex);

		if (!player)
			continue;

		player->flags() = RestorePlayer.m_nFlags;
		player->origin() = RestorePlayer.m_vecOrigin;
		player->set_abs_origin(RestorePlayer.m_vecAbsOrigin);
		*(bool*)((uintptr_t)player + 0xED) = RestorePlayer.m_bDormant; 
	}
	m_arRestorePlayers.clear();

	CUtlVector<SndInfo_t> m_utlCurSoundList;
	interfaces::engine_sound->GetActiveSounds(m_utlCurSoundList);

	if (!m_utlCurSoundList.Count())
		return;

	for (int iter = 0; iter < m_utlCurSoundList.Count(); iter++) {
		SndInfo_t& sound = m_utlCurSoundList[iter];

		if (sound.m_nSoundSource == 0 || sound.m_nSoundSource > 64) 
			continue;

		for (int i = 0; i < restore_sound.size(); i++) {
			auto sound_ = restore_sound.at(i);
			if (sound_.second == iter)
				*sound.m_pOrigin = sound_.first;
		}
	}
}

void features::visuals::dormant_system::setup_adjust_player(player_t* player, SndInfo_t& sound) {
	vec3_t src3D, dst3D;
	trace_t tr;
	ray_t ray;
	trace_filter filter;

	filter.skip = player;
	src3D = (*sound.m_pOrigin) + vec3_t(0, 0, 1); 
	dst3D = src3D - vec3_t(0, 0, 100);
	ray.initialize(src3D, dst3D);

	interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &filter, &tr);

	if (tr.allsolid) {
		m_cSoundPlayers[sound.m_nSoundSource].m_iReceiveTime = -1;
	}

	*sound.m_pOrigin = (tr.flFraction < 0.97 ? tr.end : *sound.m_pOrigin);
	m_cSoundPlayers[sound.m_nSoundSource].m_nFlags = player->flags();
	m_cSoundPlayers[sound.m_nSoundSource].m_nFlags |= (tr.flFraction < 0.50f ? fl_ducking : 0) | (tr.flFraction != 1 ? fl_onground : 0);
	m_cSoundPlayers[sound.m_nSoundSource].m_nFlags &= (tr.flFraction > 0.50f ? ~fl_ducking : 0) | (tr.flFraction == 1 ? ~fl_onground : 0);
}

bool features::visuals::dormant_system::is_valid_sound(SndInfo_t& sound) {
	for (int iter = 0; iter < m_utlvecSoundBuffer.Count(); iter++) {
		const SndInfo_t& cached_sound = m_utlvecSoundBuffer[iter];
		if (cached_sound.m_nGuid == sound.m_nGuid) {
			return false;
		}
	}

	return true;
}