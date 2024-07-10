#include "config.hpp"
#include <urlmon.h>    
#include <filesystem>
#include <fstream>
#include "../menu.hpp"
#include "../../utils/xor.hpp"
#pragma comment(lib, "Urlmon")

static std::string path = "C:/bhopcheat/";
static const std::filesystem::path path_fs = "C:/bhopcheat/";
template <typename T>
static void read_value(const nlohmann::json& src, T& dest) {
	if (src.is_null()) return;

	dest = src.get<T>();
}

template <typename T>
static T read_value(const nlohmann::json& src) {
	if (src.is_null()) return T{};

	return src.get<T>();
}

template <typename T>
static void write_value(nlohmann::json& dest, const T& src) {
	dest = src;
}

namespace c {
	void create_directory() {
		if (!std::filesystem::exists(path_fs))
			std::filesystem::create_directory(path_fs);
		else
			update_configs();	
	}

	void open_directory() {
		ShellExecuteA(nullptr, "open", path.c_str(), nullptr, nullptr, SW_NORMAL);
	}

	void update_configs() {
		configs.clear();

		for (const auto& entry : std::filesystem::directory_iterator{ path_fs }) {
			if (!entry.is_regular_file() || entry.path().extension() != (".json")) continue;

			const auto& file_name = entry.path().stem().string();
			configs.emplace_back(file_name);
		}
	}

	void create_file(const std::string_view name) {
		if (name.empty() || name.find_first_not_of(' ') == std::string_view::npos) return;
		if (std::find(configs.cbegin(), configs.cend(), name) != configs.cend()) return;

		configs.emplace_back(name);
		save(configs.size() - 1);
	}

	void delete_file(const std::size_t index) {
		if (configs.size() <= index) return;

		path.append(configs.at(index));
		path.append(".json");
		std::filesystem::remove(path);
		path.erase(path.size() - configs.at(index).size() - 5);

		configs.erase(configs.cbegin() + index);
		configs.shrink_to_fit();
		update_configs;
	}

	void save(const std::size_t index) {
		nlohmann::json json;

		// backtrack namespace
		write_value(json[("backtrack")][("enable")], backtrack::enable);
		write_value(json[("backtrack")][("key")], backtrack::key);
		write_value(json[("backtrack")][("key_s")], backtrack::key_s);
		write_value(json[("backtrack")][("time")], backtrack::time);
		write_value(json[("backtrack")][("fake")], backtrack::fake);
		write_value(json[("backtrack")][("fake_latency")], backtrack::fake_latency);

		// triggerbot namespace
		write_value(json[("triggerbot")][("enable")], triggerbot::enable);
		write_value(json[("triggerbot")][("key")], triggerbot::key);
		write_value(json[("triggerbot")][("key_s")], triggerbot::key_s);

		// aimbot namespace
		write_value(json[("aimbot")][("enable")], aimbot::enable);
		write_value(json[("aimbot")][("shoot_at_aim_key")], aimbot::shoot_at_aim_key);
		write_value(json[("aimbot")][("aim_at_backtrack")], aimbot::aim_at_backtrack);
		write_value(json[("aimbot")][("key")], aimbot::key);
		write_value(json[("aimbot")][("key_s")], aimbot::key_s);

		// aimbot general values
		write_value(json[("aimbot")][("general_override_general")], aimbot::general.override_general);
		write_value(json[("aimbot")][("general_autopistol")], aimbot::general.autopistol);
		write_value(json[("aimbot")][("general_silent")], aimbot::general.silent);
		write_value(json[("aimbot")][("general_recoil_compensate")], aimbot::general.recoil_compensate);
		write_value(json[("aimbot")][("general_autowall")], aimbot::general.autowall);
		write_value(json[("aimbot")][("general_autowall_damage")], aimbot::general.autowall_damage);
		write_value(json[("aimbot")][("general_fov")], aimbot::general.fov);
		write_value(json[("aimbot")][("general_head")], aimbot::general.head);
		write_value(json[("aimbot")][("general_neck")], aimbot::general.neck);
		write_value(json[("aimbot")][("general_chest")], aimbot::general.chest);
		write_value(json[("aimbot")][("general_stomach")], aimbot::general.stomach);
		write_value(json[("aimbot")][("general_pelvis")], aimbot::general.pelvis);
		write_value(json[("aimbot")][("general_foot")], aimbot::general.foot);

		// aimbot pistols values
		write_value(json[("aimbot")][("pistols_override_pistols")], aimbot::pistols.override_general);
		write_value(json[("aimbot")][("pistols_autopistol")], aimbot::pistols.autopistol);
		write_value(json[("aimbot")][("pistols_silent")], aimbot::pistols.silent);
		write_value(json[("aimbot")][("pistols_recoil_compensate")], aimbot::pistols.recoil_compensate);
		write_value(json[("aimbot")][("pistols_autowall")], aimbot::pistols.autowall);
		write_value(json[("aimbot")][("pistols_autowall_damage")], aimbot::pistols.autowall_damage);
		write_value(json[("aimbot")][("pistols_fov")], aimbot::pistols.fov);
		write_value(json[("aimbot")][("pistols_head")], aimbot::pistols.head);
		write_value(json[("aimbot")][("pistols_neck")], aimbot::pistols.neck);
		write_value(json[("aimbot")][("pistols_chest")], aimbot::pistols.chest);
		write_value(json[("aimbot")][("pistols_stomach")], aimbot::pistols.stomach);
		write_value(json[("aimbot")][("pistols_pelvis")], aimbot::pistols.pelvis);
		write_value(json[("aimbot")][("pistols_foot")], aimbot::pistols.foot);

		// aimbot heavy_pistols values
		write_value(json[("aimbot")][("heavy_pistols_override_general")], aimbot::heavy_pistols.override_general);
		write_value(json[("aimbot")][("heavy_pistols_autopistol")], aimbot::heavy_pistols.autopistol);
		write_value(json[("aimbot")][("heavy_pistols_silent")], aimbot::heavy_pistols.silent);
		write_value(json[("aimbot")][("heavy_pistols_recoil_compensate")], aimbot::heavy_pistols.recoil_compensate);
		write_value(json[("aimbot")][("heavy_pistols_autowall")], aimbot::heavy_pistols.autowall);
		write_value(json[("aimbot")][("heavy_pistols_autowall_damage")], aimbot::heavy_pistols.autowall_damage);
		write_value(json[("aimbot")][("heavy_pistols_fov")], aimbot::heavy_pistols.fov);
		write_value(json[("aimbot")][("heavy_pistols_head")], aimbot::heavy_pistols.head);
		write_value(json[("aimbot")][("heavy_pistols_neck")], aimbot::heavy_pistols.neck);
		write_value(json[("aimbot")][("heavy_pistols_chest")], aimbot::heavy_pistols.chest);
		write_value(json[("aimbot")][("heavy_pistols_stomach")], aimbot::heavy_pistols.stomach);
		write_value(json[("aimbot")][("heavy_pistols_pelvis")], aimbot::heavy_pistols.pelvis);
		write_value(json[("aimbot")][("heavy_pistols_foot")], aimbot::heavy_pistols.foot);

		// aimbot smg values
		write_value(json[("aimbot")][("smg_override_general")], aimbot::smg.override_general);
		write_value(json[("aimbot")][("smg_autopistol")], aimbot::smg.autopistol);
		write_value(json[("aimbot")][("smg_silent")], aimbot::smg.silent);
		write_value(json[("aimbot")][("smg_recoil_compensate")], aimbot::smg.recoil_compensate);
		write_value(json[("aimbot")][("smg_autowall")], aimbot::smg.autowall);
		write_value(json[("aimbot")][("smg_autowall_damage")], aimbot::smg.autowall_damage);
		write_value(json[("aimbot")][("smg_fov")], aimbot::smg.fov);
		write_value(json[("aimbot")][("smg_head")], aimbot::smg.head);
		write_value(json[("aimbot")][("smg_neck")], aimbot::smg.neck);
		write_value(json[("aimbot")][("smg_chest")], aimbot::smg.chest);
		write_value(json[("aimbot")][("smg_stomach")], aimbot::smg.stomach);
		write_value(json[("aimbot")][("smg_pelvis")], aimbot::smg.pelvis);
		write_value(json[("aimbot")][("smg_foot")], aimbot::smg.foot);

		// aimbot smg values
		write_value(json[("aimbot")][("shotguns_override_general")], aimbot::shotguns.override_general);
		write_value(json[("aimbot")][("shotguns_autopistol")], aimbot::shotguns.autopistol);
		write_value(json[("aimbot")][("shotguns_silent")], aimbot::shotguns.silent);
		write_value(json[("aimbot")][("shotguns_recoil_compensate")], aimbot::shotguns.recoil_compensate);
		write_value(json[("aimbot")][("shotguns_autowall")], aimbot::shotguns.autowall);
		write_value(json[("aimbot")][("shotguns_autowall_damage")], aimbot::shotguns.autowall_damage);
		write_value(json[("aimbot")][("shotguns_fov")], aimbot::shotguns.fov);
		write_value(json[("aimbot")][("shotguns_head")], aimbot::shotguns.head);
		write_value(json[("aimbot")][("shotguns_neck")], aimbot::shotguns.neck);
		write_value(json[("aimbot")][("shotguns_chest")], aimbot::shotguns.chest);
		write_value(json[("aimbot")][("shotguns_stomach")], aimbot::shotguns.stomach);
		write_value(json[("aimbot")][("shotguns_pelvis")], aimbot::shotguns.pelvis);
		write_value(json[("aimbot")][("shotguns_foot")], aimbot::shotguns.foot);

		// aimbot rifles values
		write_value(json[("aimbot")][("rifles_override_general")], aimbot::rifles.override_general);
		write_value(json[("aimbot")][("rifles_autopistol")], aimbot::rifles.autopistol);
		write_value(json[("aimbot")][("rifles_silent")], aimbot::rifles.silent);
		write_value(json[("aimbot")][("rifles_recoil_compensate")], aimbot::rifles.recoil_compensate);
		write_value(json[("aimbot")][("rifles_autowall")], aimbot::rifles.autowall);
		write_value(json[("aimbot")][("rifles_autowall_damage")], aimbot::rifles.autowall_damage);
		write_value(json[("aimbot")][("rifles_fov")], aimbot::rifles.fov);
		write_value(json[("aimbot")][("rifles_head")], aimbot::rifles.head);
		write_value(json[("aimbot")][("rifles_neck")], aimbot::rifles.neck);
		write_value(json[("aimbot")][("rifles_chest")], aimbot::rifles.chest);
		write_value(json[("aimbot")][("rifles_stomach")], aimbot::rifles.stomach);
		write_value(json[("aimbot")][("rifles_pelvis")], aimbot::rifles.pelvis);
		write_value(json[("aimbot")][("rifles_foot")], aimbot::rifles.foot);

		// aimbot snipers values
		write_value(json[("aimbot")][("snipers_override_general")], aimbot::snipers.override_general);
		write_value(json[("aimbot")][("snipers_autopistol")], aimbot::snipers.autopistol);
		write_value(json[("aimbot")][("snipers_silent")], aimbot::snipers.silent);
		write_value(json[("aimbot")][("snipers_recoil_compensate")], aimbot::snipers.recoil_compensate);
		write_value(json[("aimbot")][("snipers_autowall")], aimbot::snipers.autowall);
		write_value(json[("aimbot")][("snipers_autowall_damage")], aimbot::snipers.autowall_damage);
		write_value(json[("aimbot")][("snipers_fov")], aimbot::snipers.fov);
		write_value(json[("aimbot")][("snipers_head")], aimbot::snipers.head);
		write_value(json[("aimbot")][("snipers_neck")], aimbot::snipers.neck);
		write_value(json[("aimbot")][("snipers_chest")], aimbot::snipers.chest);
		write_value(json[("aimbot")][("snipers_stomach")], aimbot::snipers.stomach);
		write_value(json[("aimbot")][("snipers_pelvis")], aimbot::snipers.pelvis);
		write_value(json[("aimbot")][("snipers_foot")], aimbot::snipers.foot);

		// chams namespace
		write_value(json["chams"]["visible"], chams::visible::enable);
		write_value(json["chams"]["visible_hide_original_model"], chams::visible::hide_original_model);
		write_value(json["chams"]["visible_type"], chams::visible::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("visible_color[") + std::to_string(i) + ("]")], chams::visible::color[i]);
		}
		write_value(json["chams"]["visible_overlay"], chams::visible::overlay::enable);
		write_value(json["chams"]["visible_overlay_hide_original_model"], chams::visible::overlay::index);
		write_value(json["chams"]["visible_overlay_type"], chams::visible::overlay::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("visible_overlay_color[") + std::to_string(i) + ("]")], chams::visible::overlay::color[i]);
		}
		write_value(json["chams"]["invisible"], chams::invisible::enable);
		write_value(json["chams"]["invisible_hide_original_model"], chams::invisible::hide_original_model);
		write_value(json["chams"]["invisible_type"], chams::invisible::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("invisible_color[") + std::to_string(i) + ("]")], chams::invisible::color[i]);
		}
		write_value(json["chams"]["invisible_overlay"], chams::invisible::overlay::enable);
		write_value(json["chams"]["invisible_overlay_hide_original_model"], chams::invisible::overlay::index);
		write_value(json["chams"]["invisible_overlay_type"], chams::invisible::overlay::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("invisible_overlay_color[") + std::to_string(i) + ("]")], chams::invisible::overlay::color[i]);
		}
		write_value(json["chams"]["visible_attachment"], chams::visible_attachment::enable);
		write_value(json["chams"]["visible_attachment_hide_original_model"], chams::visible_attachment::hide_original_model);
		write_value(json["chams"]["visible_attachment_type"], chams::visible_attachment::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("visible_attachment_color[") + std::to_string(i) + ("]")], chams::visible_attachment::color[i]);
		}
		write_value(json["chams"]["visible_attachment_overlay"], chams::visible_attachment::overlay::enable);
		write_value(json["chams"]["visible_attachment_overlay_hide_original_model"], chams::visible_attachment::overlay::index);
		write_value(json["chams"]["visible_attachment_overlay_type"], chams::visible_attachment::overlay::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("visible_attachment_overlay_color[") + std::to_string(i) + ("]")], chams::visible_attachment::overlay::color[i]);
		}
		write_value(json["chams"]["invisible_attachment"], chams::invisible_attachment::enable);
		write_value(json["chams"]["invisible_attachment_hide_original_model"], chams::invisible_attachment::hide_original_model);
		write_value(json["chams"]["invisible_attachment_type"], chams::invisible_attachment::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("invisible_attachment_color[") + std::to_string(i) + ("]")], chams::invisible_attachment::color[i]);
		}
		write_value(json["chams"]["invisible_attachment_overlay"], chams::invisible_attachment::overlay::enable);
		write_value(json["chams"]["invisible_attachment_overlay_hide_original_model"], chams::invisible_attachment::overlay::index);
		write_value(json["chams"]["invisible_attachment_overlay_type"], chams::invisible_attachment::overlay::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("invisible_attachment_overlay_color[") + std::to_string(i) + ("]")], chams::invisible_attachment::overlay::color[i]);
		}
		write_value(json["chams"]["sleeves"], chams::sleeves::enable);
		write_value(json["chams"]["sleeves_hide_original_model"], chams::sleeves::hide_original_model);
		write_value(json["chams"]["sleeves_type"], chams::sleeves::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("sleeves_color[") + std::to_string(i) + ("]")], chams::sleeves::color[i]);
		}
		write_value(json["chams"]["sleeves_overlay"], chams::sleeves::overlay::enable);
		write_value(json["chams"]["sleeves_overlay_hide_original_model"], chams::sleeves::overlay::index);
		write_value(json["chams"]["sleeves_overlay_type"], chams::sleeves::overlay::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("sleeves_overlay_color[") + std::to_string(i) + ("]")], chams::sleeves::overlay::color[i]);
		}
		write_value(json["chams"]["hands"], chams::hands::enable);
		write_value(json["chams"]["hands_old_shaders"], chams::hands::old_shaders);
		write_value(json["chams"]["hands_hide_original_model"], chams::hands::hide_original_model);
		write_value(json["chams"]["hands_type"], chams::hands::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("hands_color[") + std::to_string(i) + ("]")], chams::hands::color[i]);
		}
		write_value(json["chams"]["hands_overlay"], chams::hands::overlay::enable);
		write_value(json["chams"]["hands_overlay_hide_original_model"], chams::hands::overlay::index);
		write_value(json["chams"]["hands_overlay_type"], chams::hands::overlay::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("hands_overlay_color[") + std::to_string(i) + ("]")], chams::hands::overlay::color[i]);
		}
		write_value(json["chams"]["weapon"], chams::weapon::enable);
		write_value(json["chams"]["weapon_hide_original_model"], chams::weapon::hide_original_model);
		write_value(json["chams"]["weapon_type"], chams::weapon::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("weapon_color[") + std::to_string(i) + ("]")], chams::sleeves::color[i]);
		}
		write_value(json["chams"]["weapon_overlay"], chams::weapon::overlay::enable);
		write_value(json["chams"]["weapon_overlay_hide_original_model"], chams::weapon::overlay::index);
		write_value(json["chams"]["weapon_overlay_type"], chams::weapon::overlay::type);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("chams")][("weapon_overlay_color[") + std::to_string(i) + ("]")], chams::weapon::overlay::color[i]);
		}

		// misc namespace
		write_value(json[("misc")][("force_crosshair")], misc::force_crosshair);
		write_value(json[("misc")][("sniper_crosshair")], misc::sniper_crosshair);
		write_value(json[("misc")][("recoil_crosshair")], misc::recoil_crosshair);
		write_value(json[("misc")][("bullet_impacts")], misc::bullet_impacts::enable);
		write_value(json[("misc")][("bullet_impacts_time")], misc::bullet_impacts::time);
		write_value(json[("misc")][("bullet_impacts_server")], misc::bullet_impacts::server::enable);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("misc")][("bullet_impacts_server_color[" + std::to_string(i) + ("]"))], misc::bullet_impacts::server::color[i]);
		}
		write_value(json[("misc")][("bullet_impacts_client")], misc::bullet_impacts::client::enable);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("misc")][("bullet_impacts_client_color[" + std::to_string(i) + ("]"))], misc::bullet_impacts::client::color[i]);
		}
		write_value(json[("misc")][("bullet_tracers")], misc::bullet_tracers::enable);
		write_value(json[("misc")][("bullet_tracers_time")], misc::bullet_tracers::time);
		write_value(json[("misc")][("bullet_tracers_local")], misc::bullet_tracers::local::enable);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("misc")][("bullet_tracers_local_color[" + std::to_string(i) + ("]"))], misc::bullet_tracers::local::color[i]);
		}
		write_value(json[("misc")][("bullet_tracers_entity")], misc::bullet_tracers::entity::enable);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("misc")][("bullet_tracers_entity_color[" + std::to_string(i) + ("]"))], misc::bullet_tracers::entity::color[i]);
		}
		write_value(json[("misc")][("bullet_tracers_hit_entity")], misc::bullet_tracers::hit_entity::enable);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("misc")][("bullet_tracers_hit_entity_color[" + std::to_string(i) + ("]"))], misc::bullet_tracers::entity::color[i]);
		}
		write_value(json[("misc")][("penetration_reticle")], misc::penetration_reticle::enable);
		write_value(json[("misc")][("spread_circle")], misc::spread_circle::enable);
		for (int i = 0; i < 3; ++i) {
			write_value(json[("misc")][("spread_circle_color[" + std::to_string(i) + ("]"))], misc::spread_circle::color[i]);
		}
		write_value(json[("misc")][("debug_information")], misc::debug_information::enable);
		write_value(json[("misc")][("debug_information_autowall")], misc::debug_information::autowall::enable);
		write_value(json[("misc")][("debug_information_autowall_rainbow_color")], misc::debug_information::autowall::rainbow_color);
		write_value(json[("misc")][("debug_information_can_fire")], misc::debug_information::can_fire::enable);
		write_value(json[("misc")][("debug_information_can_fire_style")], misc::debug_information::can_fire::style);
		for (int i = 0; i < 3; ++i) {
			write_value(json[("misc")][("debug_information_can_fire_inactive_color[" + std::to_string(i) + ("]"))], misc::debug_information::can_fire::inactive_color[i]);
			write_value(json[("misc")][("debug_information_can_fire_active_color[" + std::to_string(i) + ("]"))], misc::debug_information::can_fire::active_color[i]);
		}
		write_value(json[("misc")][("debug_information_check_point_system")], misc::debug_information::check_point_system::enable);
		for (int i = 0; i < 3; ++i) {
			write_value(json[("misc")][("debug_information_check_point_system_inactive_color[" + std::to_string(i) + ("]"))], misc::debug_information::check_point_system::inactive_color[i]);
			write_value(json[("misc")][("debug_information_check_point_system_active_color[" + std::to_string(i) + ("]"))], misc::debug_information::check_point_system::active_color[i]);
		}
		write_value(json[("misc")][("preverse_killfeed")], misc::preverse_killfeed::enable);
		write_value(json[("misc")][("preverse_killfeed_time")], misc::preverse_killfeed::time);
		write_value(json[("misc")][("sv_cheats")], misc::sv_cheats::enable);
		write_value(json[("misc")][("spectators_list")], misc::spectators_list);
		write_value(json[("misc")][("spectators_list_mode")], misc::spectators_list_mode);
		write_value(json[("misc")][("spectators_list_avatars")], misc::spectators_list_avatars);
		write_value(json[("misc")][("practice")], misc::practice);
		write_value(json[("misc")][("savepos")], misc::savepos);
		write_value(json[("misc")][("savepos_s")], misc::savepos_s);
		write_value(json[("misc")][("loadpos")], misc::loadpos);
		write_value(json[("misc")][("loadpos_s")], misc::loadpos_s);
		write_value(json[("misc")][("enable_fov")], misc::enable_fov);
		write_value(json[("misc")][("field_of_view")], misc::field_of_view);
		write_value(json[("misc")][("view_model")], misc::view_model);
		write_value(json[("misc")][("view_model_x")], misc::view_model_x);
		write_value(json[("misc")][("view_model_y")], misc::view_model_y);
		write_value(json[("misc")][("view_model_z")], misc::view_model_z);
		write_value(json[("misc")][("radaringame")], misc::radaringame);
		write_value(json[("misc")][("swayscale")], misc::swayscale);
		write_value(json[("misc")][("swayscale_value")], misc::swayscale_value);
		write_value(json[("misc")][("misc_clantag_spammer")], misc::misc_clantag_spammer);
		write_value(json[("misc")][("misc_clantag_reverse_rolling")], misc::misc_clantag_reverse_rolling);
		write_value(json[("misc")][("misc_clantag_type")], misc::misc_clantag_type);
		write_value(json[("misc")][("misc_clantag_speed")], misc::misc_clantag_speed);
		write_value(json[("misc")][("misc_hitmarker")], misc::misc_hitmarker);
		write_value(json[("misc")][("misc_hitmarker_sound")], misc::misc_hitmarker_sound);
		for (int i = 0; i < 3; ++i) {
			write_value(json[("misc")][("misc_hit_info[") + std::to_string(i) + ("]")], misc::misc_hit_info[i]);
		}
		write_value(json[("misc")][("misc_hitchams")], misc::misc_hitchams);
		for (int i = 0; i < 4; ++i) {
			write_value(json[("misc")][("misc_hitchams_color[") + std::to_string(i) + ("]")], misc::misc_hitchams_clr[i]);
		}
		write_value(json[("misc")][("nadepred")], misc::nadepred);
		for (int i = 0; i < 3; ++i) {
			write_value(json[("misc")][("nadepred_color[") + std::to_string(i) + ("]")], misc::nadepred_clr[i]);
		}
		write_value(json[("misc")][("freecam")], misc::freecam);
		write_value(json[("misc")][("freecam_key")], misc::freecam_key);
		write_value(json[("misc")][("freecam_key_s")], misc::freecam_key_s);
		write_value(json[("misc")][("mousefix")], misc::mousefix);
		write_value(json[("misc")][("menu_key")], misc::menu_key);

		// menu namespace
		for (int i = 0; i < 3; ++i) {
			write_value(json[("menu")][("menu_color[") + std::to_string(i) + ("]")], menu::menu_accent[i]);
		}

		// fonts namespace
		write_value(json[("fonts")][("indi_size")], fonts::indi_size);
		write_value(json[("fonts")][("indi_font")], fonts::indi_font);
		for (int i = 0; i < 11; ++i) {
			write_value(json[("fonts")][("indi_font_flag[") + std::to_string(i) + ("]")], fonts::indi_font_flag[i]);
		}
		write_value(json[("fonts")][("sub_indi_size")], fonts::sub_indi_size);
		write_value(json[("fonts")][("sub_indi_font")], fonts::sub_indi_font);
		for (int i = 0; i < 11; ++i) {
			write_value(json[("fonts")][("sub_indi_font_flag[") + std::to_string(i) + ("]")], fonts::sub_indi_font_flag[i]);
		}
		write_value(json[("fonts")][("scene_font")], fonts::scene_font);
		write_value(json[("fonts")][("scene_size")], fonts::scene_size);
		for (int i = 0; i < 11; ++i) {
			write_value(json[("fonts")][("scene_flag[") + std::to_string(i) + ("]")], fonts::scene_flag[i]);
		}
		write_value(json[("fonts")][("scene_sub_font")], fonts::scene_sub_font);
		write_value(json[("fonts")][("scene_sub_size")], fonts::scene_sub_size);
		for (int i = 0; i < 11; ++i) {
			write_value(json[("fonts")][("scene_sub_flag[") + std::to_string(i) + ("]")], fonts::scene_sub_flag[i]);
		}
		write_value(json[("fonts")][("esp_font")], fonts::esp_font);
		write_value(json[("fonts")][("esp_size")], fonts::esp_size);
		for (int i = 0; i < 11; ++i) {
			write_value(json[("fonts")][("esp_flag[") + std::to_string(i) + ("]")], fonts::esp_flag[i]);
		}
		write_value(json[("fonts")][("esp_sub_font")], fonts::esp_sub_font);
		write_value(json[("fonts")][("esp_sub_size")], fonts::esp_sub_size);
		for (int i = 0; i < 11; ++i) {
			write_value(json[("fonts")][("esp_sub_flag[") + std::to_string(i) + ("]")], fonts::esp_sub_flag[i]);
		}
		write_value(json[("fonts")][("debug_information_font")], fonts::debug_information_font);
		write_value(json[("fonts")][("debug_information_size")], fonts::debug_information_size);
		for (int i = 0; i < 11; ++i) {
			write_value(json[("fonts")][("debug_information_flag[") + std::to_string(i) + ("]")], fonts::debug_information_flag[i]);
		}

		// skins namespace
		write_value(json[("skins")][("agent_changer")], skins::agent_changer);
		write_value(json[("skins")][("agent_t")], skins::agent_t);
		write_value(json[("skins")][("agent_ct")], skins::agent_ct);
		write_value(json[("skins")][("knife_changer_enable")], skins::knife_changer_enable);
		write_value(json[("skins")][("knife_changer_wear")], skins::knife_changer_wear);
		write_value(json[("skins")][("knife_changer_model")], skins::knife_changer_model);
		write_value(json[("skins")][("knife_changer_skin_id")], skins::knife_changer_skin_id);
		write_value(json[("skins")][("knife_changer_paint_kit")], skins::knife_changer_paint_kit);
		write_value(json[("skins")][("knife_changer_vector_paint_kit")], skins::knife_changer_vector_paint_kit);
		write_value(json[("skins")][("knife_skin")], skins::knife_skin);
		write_value(json[("skins")][("gloves_endable")], skins::gloves_endable);
		write_value(json[("skins")][("gloves_model")], skins::gloves_model);
		write_value(json[("skins")][("gloves_skin")], skins::gloves_skin);
		write_value(json[("skins")][("gloves_skin_id")], skins::gloves_skin_id);
		write_value(json[("skins")][("gloves_wear")], skins::gloves_wear);
		write_value(json[("skins")][("agent_model")], skins::agent_model);
		write_value(json[("skins")][("skin_custom_clr")], skins::skin_custom_clr);
		for (int i = 0; i < 3; ++i) {
			write_value(json[("skins")][("skin_modulation1[") + std::to_string(i) + ("]")], skins::skin_modulation1[i]);
			write_value(json[("skins")][("skin_modulation2[") + std::to_string(i) + ("]")], skins::skin_modulation2[i]);
			write_value(json[("skins")][("skin_modulation3[") + std::to_string(i) + ("]")], skins::skin_modulation3[i]);
			write_value(json[("skins")][("skin_modulation4[") + std::to_string(i) + ("]")], skins::skin_modulation4[i]);
		}

		path.append(configs.at(index));

		const bool has_json_in_filename = path.rfind((".json")) != std::string::npos;
		if (!has_json_in_filename) {
			path.append((".json"));
		}

		if (std::ofstream output_file{ path }; output_file.good()) {
			output_file << std::setw(4) << json;
		}

		path.erase(path.size() - configs.at(index).size() - (!has_json_in_filename ? 5 : 0));
	}

	void load(const std::size_t index) {
		path.append(configs.at(index));
		path.append((".json"));

		if (std::ifstream input_file{ path }; input_file.good()) {
			nlohmann::json json;
			input_file >> json;

			// backtrack namespace
			read_value(json[("backtrack")][("enable")], backtrack::enable);
			read_value(json[("backtrack")][("key")], backtrack::key);
			read_value(json[("backtrack")][("key_s")], backtrack::key_s);
			read_value(json[("backtrack")][("time")], backtrack::time);
			read_value(json[("backtrack")][("fake")], backtrack::fake);
			read_value(json[("backtrack")][("fake_latency")], backtrack::fake_latency);

			// triggerbot namespace
			read_value(json[("triggerbot")][("enable")], triggerbot::enable);
			read_value(json[("triggerbot")][("key")], triggerbot::key);
			read_value(json[("triggerbot")][("key_s")], triggerbot::key_s);

			// aimbot namespace
			read_value(json[("aimbot")][("enable")], aimbot::enable);
			read_value(json[("aimbot")][("shoot_at_aim_key")], aimbot::shoot_at_aim_key);
			read_value(json[("aimbot")][("aim_at_backtrack")], aimbot::aim_at_backtrack);
			read_value(json[("aimbot")][("key")], aimbot::key);
			read_value(json[("aimbot")][("key_s")], aimbot::key_s);

			// aimbot general values
			read_value(json[("aimbot")][("general_override_general")], aimbot::general.override_general);
			read_value(json[("aimbot")][("general_autopistol")], aimbot::general.autopistol);
			read_value(json[("aimbot")][("general_silent")], aimbot::general.silent);
			read_value(json[("aimbot")][("general_recoil_compensate")], aimbot::general.recoil_compensate);
			read_value(json[("aimbot")][("general_autowall")], aimbot::general.autowall);
			read_value(json[("aimbot")][("general_autowall_damage")], aimbot::general.autowall_damage);
			read_value(json[("aimbot")][("general_fov")], aimbot::general.fov);
			read_value(json[("aimbot")][("general_head")], aimbot::general.head);
			read_value(json[("aimbot")][("general_neck")], aimbot::general.neck);
			read_value(json[("aimbot")][("general_chest")], aimbot::general.chest);
			read_value(json[("aimbot")][("general_stomach")], aimbot::general.stomach);
			read_value(json[("aimbot")][("general_pelvis")], aimbot::general.pelvis);
			read_value(json[("aimbot")][("general_foot")], aimbot::general.foot);

			// aimbot pistols values
			read_value(json[("aimbot")][("pistols_override_pistols")], aimbot::pistols.override_general);
			read_value(json[("aimbot")][("pistols_autopistol")], aimbot::pistols.autopistol);
			read_value(json[("aimbot")][("pistols_silent")], aimbot::pistols.silent);
			read_value(json[("aimbot")][("pistols_recoil_compensate")], aimbot::pistols.recoil_compensate);
			read_value(json[("aimbot")][("pistols_autowall")], aimbot::pistols.autowall);
			read_value(json[("aimbot")][("pistols_autowall_damage")], aimbot::pistols.autowall_damage);
			read_value(json[("aimbot")][("pistols_fov")], aimbot::pistols.fov);
			read_value(json[("aimbot")][("pistols_head")], aimbot::pistols.head);
			read_value(json[("aimbot")][("pistols_neck")], aimbot::pistols.neck);
			read_value(json[("aimbot")][("pistols_chest")], aimbot::pistols.chest);
			read_value(json[("aimbot")][("pistols_stomach")], aimbot::pistols.stomach);
			read_value(json[("aimbot")][("pistols_pelvis")], aimbot::pistols.pelvis);
			read_value(json[("aimbot")][("pistols_foot")], aimbot::pistols.foot);

			// aimbot heavy_pistols values
			read_value(json[("aimbot")][("heavy_pistols_override_general")], aimbot::heavy_pistols.override_general);
			read_value(json[("aimbot")][("heavy_pistols_autopistol")], aimbot::heavy_pistols.autopistol);
			read_value(json[("aimbot")][("heavy_pistols_silent")], aimbot::heavy_pistols.silent);
			read_value(json[("aimbot")][("heavy_pistols_recoil_compensate")], aimbot::heavy_pistols.recoil_compensate);
			read_value(json[("aimbot")][("heavy_pistols_autowall")], aimbot::heavy_pistols.autowall);
			read_value(json[("aimbot")][("heavy_pistols_autowall_damage")], aimbot::heavy_pistols.autowall_damage);
			read_value(json[("aimbot")][("heavy_pistols_fov")], aimbot::heavy_pistols.fov);
			read_value(json[("aimbot")][("heavy_pistols_head")], aimbot::heavy_pistols.head);
			read_value(json[("aimbot")][("heavy_pistols_neck")], aimbot::heavy_pistols.neck);
			read_value(json[("aimbot")][("heavy_pistols_chest")], aimbot::heavy_pistols.chest);
			read_value(json[("aimbot")][("heavy_pistols_stomach")], aimbot::heavy_pistols.stomach);
			read_value(json[("aimbot")][("heavy_pistols_pelvis")], aimbot::heavy_pistols.pelvis);
			read_value(json[("aimbot")][("heavy_pistols_foot")], aimbot::heavy_pistols.foot);

			// aimbot smg values
			read_value(json[("aimbot")][("smg_override_general")], aimbot::smg.override_general);
			read_value(json[("aimbot")][("smg_autopistol")], aimbot::smg.autopistol);
			read_value(json[("aimbot")][("smg_silent")], aimbot::smg.silent);
			read_value(json[("aimbot")][("smg_recoil_compensate")], aimbot::smg.recoil_compensate);
			read_value(json[("aimbot")][("smg_autowall")], aimbot::smg.autowall);
			read_value(json[("aimbot")][("smg_autowall_damage")], aimbot::smg.autowall_damage);
			read_value(json[("aimbot")][("smg_fov")], aimbot::smg.fov);
			read_value(json[("aimbot")][("smg_head")], aimbot::smg.head);
			read_value(json[("aimbot")][("smg_neck")], aimbot::smg.neck);
			read_value(json[("aimbot")][("smg_chest")], aimbot::smg.chest);
			read_value(json[("aimbot")][("smg_stomach")], aimbot::smg.stomach);
			read_value(json[("aimbot")][("smg_pelvis")], aimbot::smg.pelvis);
			read_value(json[("aimbot")][("smg_foot")], aimbot::smg.foot);

			// aimbot smg values
			read_value(json[("aimbot")][("shotguns_override_general")], aimbot::shotguns.override_general);
			read_value(json[("aimbot")][("shotguns_autopistol")], aimbot::shotguns.autopistol);
			read_value(json[("aimbot")][("shotguns_silent")], aimbot::shotguns.silent);
			read_value(json[("aimbot")][("shotguns_recoil_compensate")], aimbot::shotguns.recoil_compensate);
			read_value(json[("aimbot")][("shotguns_autowall")], aimbot::shotguns.autowall);
			read_value(json[("aimbot")][("shotguns_autowall_damage")], aimbot::shotguns.autowall_damage);
			read_value(json[("aimbot")][("shotguns_fov")], aimbot::shotguns.fov);
			read_value(json[("aimbot")][("shotguns_head")], aimbot::shotguns.head);
			read_value(json[("aimbot")][("shotguns_neck")], aimbot::shotguns.neck);
			read_value(json[("aimbot")][("shotguns_chest")], aimbot::shotguns.chest);
			read_value(json[("aimbot")][("shotguns_stomach")], aimbot::shotguns.stomach);
			read_value(json[("aimbot")][("shotguns_pelvis")], aimbot::shotguns.pelvis);
			read_value(json[("aimbot")][("shotguns_foot")], aimbot::shotguns.foot);

			// aimbot rifles values
			read_value(json[("aimbot")][("rifles_override_general")], aimbot::rifles.override_general);
			read_value(json[("aimbot")][("rifles_autopistol")], aimbot::rifles.autopistol);
			read_value(json[("aimbot")][("rifles_silent")], aimbot::rifles.silent);
			read_value(json[("aimbot")][("rifles_recoil_compensate")], aimbot::rifles.recoil_compensate);
			read_value(json[("aimbot")][("rifles_autowall")], aimbot::rifles.autowall);
			read_value(json[("aimbot")][("rifles_autowall_damage")], aimbot::rifles.autowall_damage);
			read_value(json[("aimbot")][("rifles_fov")], aimbot::rifles.fov);
			read_value(json[("aimbot")][("rifles_head")], aimbot::rifles.head);
			read_value(json[("aimbot")][("rifles_neck")], aimbot::rifles.neck);
			read_value(json[("aimbot")][("rifles_chest")], aimbot::rifles.chest);
			read_value(json[("aimbot")][("rifles_stomach")], aimbot::rifles.stomach);
			read_value(json[("aimbot")][("rifles_pelvis")], aimbot::rifles.pelvis);
			read_value(json[("aimbot")][("rifles_foot")], aimbot::rifles.foot);

			// aimbot snipers values
			read_value(json[("aimbot")][("snipers_override_general")], aimbot::snipers.override_general);
			read_value(json[("aimbot")][("snipers_autopistol")], aimbot::snipers.autopistol);
			read_value(json[("aimbot")][("snipers_silent")], aimbot::snipers.silent);
			read_value(json[("aimbot")][("snipers_recoil_compensate")], aimbot::snipers.recoil_compensate);
			read_value(json[("aimbot")][("snipers_autowall")], aimbot::snipers.autowall);
			read_value(json[("aimbot")][("snipers_autowall_damage")], aimbot::snipers.autowall_damage);
			read_value(json[("aimbot")][("snipers_fov")], aimbot::snipers.fov);
			read_value(json[("aimbot")][("snipers_head")], aimbot::snipers.head);
			read_value(json[("aimbot")][("snipers_neck")], aimbot::snipers.neck);
			read_value(json[("aimbot")][("snipers_chest")], aimbot::snipers.chest);
			read_value(json[("aimbot")][("snipers_stomach")], aimbot::snipers.stomach);
			read_value(json[("aimbot")][("snipers_pelvis")], aimbot::snipers.pelvis);
			read_value(json[("aimbot")][("snipers_foot")], aimbot::snipers.foot);

			// chams namespace
			read_value(json["chams"]["visible"], chams::visible::enable);
			read_value(json["chams"]["visible_hide_original_model"], chams::visible::hide_original_model);
			read_value(json["chams"]["visible_type"], chams::visible::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("visible_color[") + std::to_string(i) + ("]")], chams::visible::color[i]);
			}
			read_value(json["chams"]["visible_overlay"], chams::visible::overlay::enable);
			read_value(json["chams"]["visible_overlay_hide_original_model"], chams::visible::overlay::index);
			read_value(json["chams"]["visible_overlay_type"], chams::visible::overlay::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("visible_overlay_color[") + std::to_string(i) + ("]")], chams::visible::overlay::color[i]);
			}
			read_value(json["chams"]["invisible"], chams::invisible::enable);
			read_value(json["chams"]["invisible_hide_original_model"], chams::invisible::hide_original_model);
			read_value(json["chams"]["invisible_type"], chams::invisible::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("invisible_color[") + std::to_string(i) + ("]")], chams::invisible::color[i]);
			}
			read_value(json["chams"]["invisible_overlay"], chams::invisible::overlay::enable);
			read_value(json["chams"]["invisible_overlay_hide_original_model"], chams::invisible::overlay::index);
			read_value(json["chams"]["invisible_overlay_type"], chams::invisible::overlay::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("invisible_overlay_color[") + std::to_string(i) + ("]")], chams::invisible::overlay::color[i]);
			}
			read_value(json["chams"]["visible_attachment"], chams::visible_attachment::enable);
			read_value(json["chams"]["visible_attachment_hide_original_model"], chams::visible_attachment::hide_original_model);
			read_value(json["chams"]["visible_attachment_type"], chams::visible_attachment::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("visible_attachment_color[") + std::to_string(i) + ("]")], chams::visible_attachment::color[i]);
			}
			read_value(json["chams"]["visible_attachment_overlay"], chams::visible_attachment::overlay::enable);
			read_value(json["chams"]["visible_attachment_overlay_hide_original_model"], chams::visible_attachment::overlay::index);
			read_value(json["chams"]["visible_attachment_overlay_type"], chams::visible_attachment::overlay::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("visible_attachment_overlay_color[") + std::to_string(i) + ("]")], chams::visible_attachment::overlay::color[i]);
			}
			read_value(json["chams"]["invisible_attachment"], chams::invisible_attachment::enable);
			read_value(json["chams"]["invisible_attachment_hide_original_model"], chams::invisible_attachment::hide_original_model);
			read_value(json["chams"]["invisible_attachment_type"], chams::invisible_attachment::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("invisible_attachment_color[") + std::to_string(i) + ("]")], chams::invisible_attachment::color[i]);
			}
			read_value(json["chams"]["invisible_attachment_overlay"], chams::invisible_attachment::overlay::enable);
			read_value(json["chams"]["invisible_attachment_overlay_hide_original_model"], chams::invisible_attachment::overlay::index);
			read_value(json["chams"]["invisible_attachment_overlay_type"], chams::invisible_attachment::overlay::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("invisible_attachment_overlay_color[") + std::to_string(i) + ("]")], chams::invisible_attachment::overlay::color[i]);
			}
			read_value(json["chams"]["sleeves"], chams::sleeves::enable);
			read_value(json["chams"]["sleeves_hide_original_model"], chams::sleeves::hide_original_model);
			read_value(json["chams"]["sleeves_type"], chams::sleeves::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("sleeves_color[") + std::to_string(i) + ("]")], chams::sleeves::color[i]);
			}
			read_value(json["chams"]["sleeves_overlay"], chams::sleeves::overlay::enable);
			read_value(json["chams"]["sleeves_overlay_hide_original_model"], chams::sleeves::overlay::index);
			read_value(json["chams"]["sleeves_overlay_type"], chams::sleeves::overlay::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("sleeves_overlay_color[") + std::to_string(i) + ("]")], chams::sleeves::overlay::color[i]);
			}
			read_value(json["chams"]["hands"], chams::hands::enable);
			read_value(json["chams"]["hands_old_shaders"], chams::hands::old_shaders);
			read_value(json["chams"]["hands_hide_original_model"], chams::hands::hide_original_model);
			read_value(json["chams"]["hands_type"], chams::hands::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("hands_color[") + std::to_string(i) + ("]")], chams::hands::color[i]);
			}
			read_value(json["chams"]["hands_overlay"], chams::hands::overlay::enable);
			read_value(json["chams"]["hands_overlay_hide_original_model"], chams::hands::overlay::index);
			read_value(json["chams"]["hands_overlay_type"], chams::hands::overlay::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("hands_overlay_color[") + std::to_string(i) + ("]")], chams::hands::overlay::color[i]);
			}
			read_value(json["chams"]["weapon"], chams::weapon::enable);
			read_value(json["chams"]["weapon_hide_original_model"], chams::weapon::hide_original_model);
			read_value(json["chams"]["weapon_type"], chams::weapon::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("weapon_color[") + std::to_string(i) + ("]")], chams::sleeves::color[i]);
			}
			read_value(json["chams"]["weapon_overlay"], chams::weapon::overlay::enable);
			read_value(json["chams"]["weapon_overlay_hide_original_model"], chams::weapon::overlay::index);
			read_value(json["chams"]["weapon_overlay_type"], chams::weapon::overlay::type);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("chams")][("weapon_overlay_color[") + std::to_string(i) + ("]")], chams::weapon::overlay::color[i]);
			}

			// misc namespace
			read_value(json[("misc")][("force_crosshair")], misc::force_crosshair);
			read_value(json[("misc")][("sniper_crosshair")], misc::sniper_crosshair);
			read_value(json[("misc")][("recoil_crosshair")], misc::recoil_crosshair);
			read_value(json[("misc")][("bullet_impacts")], misc::bullet_impacts::enable);
			read_value(json[("misc")][("bullet_impacts_time")], misc::bullet_impacts::time);
			read_value(json[("misc")][("bullet_impacts_server")], misc::bullet_impacts::server::enable);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("misc")][("bullet_impacts_server_color[" + std::to_string(i) + ("]"))], misc::bullet_impacts::server::color[i]);
			}
			read_value(json[("misc")][("bullet_impacts_client")], misc::bullet_impacts::client::enable);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("misc")][("bullet_impacts_client_color[" + std::to_string(i) + ("]"))], misc::bullet_impacts::client::color[i]);
			}
			read_value(json[("misc")][("bullet_tracers")], misc::bullet_tracers::enable);
			read_value(json[("misc")][("bullet_tracers_time")], misc::bullet_tracers::time);
			read_value(json[("misc")][("bullet_tracers_local")], misc::bullet_tracers::local::enable);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("misc")][("bullet_tracers_local_color[" + std::to_string(i) + ("]"))], misc::bullet_tracers::local::color[i]);
			}
			read_value(json[("misc")][("bullet_tracers_entity")], misc::bullet_tracers::entity::enable);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("misc")][("bullet_tracers_entity_color[" + std::to_string(i) + ("]"))], misc::bullet_tracers::entity::color[i]);
			}
			read_value(json[("misc")][("bullet_tracers_hit_entity")], misc::bullet_tracers::hit_entity::enable);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("misc")][("bullet_tracers_hit_entity_color[" + std::to_string(i) + ("]"))], misc::bullet_tracers::entity::color[i]);
			}
			read_value(json[("misc")][("penetration_reticle")], misc::penetration_reticle::enable);
			read_value(json[("misc")][("spread_circle")], misc::spread_circle::enable);
			for (int i = 0; i < 3; ++i) {
				read_value(json[("misc")][("spread_circle_color[" + std::to_string(i) + ("]"))], misc::spread_circle::color[i]);
			}
			read_value(json[("misc")][("debug_information")], misc::debug_information::enable);
			read_value(json[("misc")][("debug_information_autowall")], misc::debug_information::autowall::enable);
			read_value(json[("misc")][("debug_information_autowall_rainbow_color")], misc::debug_information::autowall::rainbow_color);
			read_value(json[("misc")][("debug_information_can_fire")], misc::debug_information::can_fire::enable);
			read_value(json[("misc")][("debug_information_can_fire_style")], misc::debug_information::can_fire::style);
			for (int i = 0; i < 3; ++i) {
				read_value(json[("misc")][("debug_information_can_fire_inactive_color[" + std::to_string(i) + ("]"))], misc::debug_information::can_fire::inactive_color[i]);
				read_value(json[("misc")][("debug_information_can_fire_active_color[" + std::to_string(i) + ("]"))], misc::debug_information::can_fire::active_color[i]);
			}
			read_value(json[("misc")][("debug_information_check_point_system")], misc::debug_information::check_point_system::enable);
			for (int i = 0; i < 3; ++i) {
				read_value(json[("misc")][("debug_information_check_point_system_inactive_color[" + std::to_string(i) + ("]"))], misc::debug_information::check_point_system::inactive_color[i]);
				read_value(json[("misc")][("debug_information_check_point_system_active_color[" + std::to_string(i) + ("]"))], misc::debug_information::check_point_system::active_color[i]);
			}
			read_value(json[("misc")][("preverse_killfeed")], misc::preverse_killfeed::enable);
			read_value(json[("misc")][("preverse_killfeed_time")], misc::preverse_killfeed::time);
			read_value(json[("misc")][("sv_cheats")], misc::sv_cheats::enable);
			read_value(json[("misc")][("spectators_list")], misc::spectators_list);
			read_value(json[("misc")][("spectators_list_mode")], misc::spectators_list_mode);
			read_value(json[("misc")][("spectators_list_avatars")], misc::spectators_list_avatars);
			read_value(json[("misc")][("practice")], misc::practice);
			read_value(json[("misc")][("savepos")], misc::savepos);
			read_value(json[("misc")][("savepos_s")], misc::savepos_s);
			read_value(json[("misc")][("loadpos")], misc::loadpos);
			read_value(json[("misc")][("loadpos_s")], misc::loadpos_s);
			read_value(json[("misc")][("enable_fov")], misc::enable_fov);
			read_value(json[("misc")][("field_of_view")], misc::field_of_view);
			read_value(json[("misc")][("view_model")], misc::view_model);
			read_value(json[("misc")][("view_model_x")], misc::view_model_x);
			read_value(json[("misc")][("view_model_y")], misc::view_model_y);
			read_value(json[("misc")][("view_model_z")], misc::view_model_z);
			read_value(json[("misc")][("radaringame")], misc::radaringame);
			read_value(json[("misc")][("swayscale")], misc::swayscale);
			read_value(json[("misc")][("swayscale_value")], misc::swayscale_value);
			read_value(json[("misc")][("misc_clantag_spammer")], misc::misc_clantag_spammer);
			read_value(json[("misc")][("misc_clantag_reverse_rolling")], misc::misc_clantag_reverse_rolling);
			read_value(json[("misc")][("misc_clantag_type")], misc::misc_clantag_type);
			read_value(json[("misc")][("misc_clantag_speed")], misc::misc_clantag_speed);
			read_value(json[("misc")][("misc_hitmarker")], misc::misc_hitmarker);
			read_value(json[("misc")][("misc_hitmarker_sound")], misc::misc_hitmarker_sound);
			for (int i = 0; i < 3; ++i) {
				read_value(json[("misc")][("misc_hit_info[") + std::to_string(i) + ("]")], misc::misc_hit_info[i]);
			}
			read_value(json[("misc")][("misc_hitchams")], misc::misc_hitchams);
			for (int i = 0; i < 4; ++i) {
				read_value(json[("misc")][("misc_hitchams_color[") + std::to_string(i) + ("]")], misc::misc_hitchams_clr[i]);
			}
			read_value(json[("misc")][("nadepred")], misc::nadepred);
			for (int i = 0; i < 3; ++i) {
				read_value(json[("misc")][("nadepred_color[") + std::to_string(i) + ("]")], misc::nadepred_clr[i]);
			}
			read_value(json[("misc")][("freecam")], misc::freecam);
			read_value(json[("misc")][("freecam_key")], misc::freecam_key);
			read_value(json[("misc")][("freecam_key_s")], misc::freecam_key_s);
			read_value(json[("misc")][("mousefix")], misc::mousefix);
			read_value(json[("misc")][("menu_key")], misc::menu_key);

			// menu namespace
			for (int i = 0; i < 3; ++i) {
				read_value(json[("menu")][("menu_color[") + std::to_string(i) + ("]")], menu::menu_accent[i]);
			}

			// fonts namespace
			read_value(json[("fonts")][("indi_size")], fonts::indi_size);
			read_value(json[("fonts")][("indi_font")], fonts::indi_font);
			for (int i = 0; i < 11; ++i) {
				read_value(json[("fonts")][("indi_font_flag[") + std::to_string(i) + ("]")], fonts::indi_font_flag[i]);
			}
			read_value(json[("fonts")][("sub_indi_size")], fonts::sub_indi_size);
			read_value(json[("fonts")][("sub_indi_font")], fonts::sub_indi_font);
			for (int i = 0; i < 11; ++i) {
				read_value(json[("fonts")][("sub_indi_font_flag[") + std::to_string(i) + ("]")], fonts::sub_indi_font_flag[i]);
			}
			read_value(json[("fonts")][("scene_font")], fonts::scene_font);
			read_value(json[("fonts")][("scene_size")], fonts::scene_size);
			for (int i = 0; i < 11; ++i) {
				read_value(json[("fonts")][("scene_flag[") + std::to_string(i) + ("]")], fonts::scene_flag[i]);
			}
			read_value(json[("fonts")][("scene_sub_font")], fonts::scene_sub_font);
			read_value(json[("fonts")][("scene_sub_size")], fonts::scene_sub_size);
			for (int i = 0; i < 11; ++i) {
				read_value(json[("fonts")][("scene_sub_flag[") + std::to_string(i) + ("]")], fonts::scene_sub_flag[i]);
			}
			read_value(json[("fonts")][("esp_font")], fonts::esp_font);
			read_value(json[("fonts")][("esp_size")], fonts::esp_size);
			for (int i = 0; i < 11; ++i) {
				read_value(json[("fonts")][("esp_flag[") + std::to_string(i) + ("]")], fonts::esp_flag[i]);
			}
			read_value(json[("fonts")][("esp_sub_font")], fonts::esp_sub_font);
			read_value(json[("fonts")][("esp_sub_size")], fonts::esp_sub_size);
			for (int i = 0; i < 11; ++i) {
				read_value(json[("fonts")][("esp_sub_flag[") + std::to_string(i) + ("]")], fonts::esp_sub_flag[i]);
			}
			read_value(json[("fonts")][("debug_information_font")], fonts::debug_information_font);
			read_value(json[("fonts")][("debug_information_size")], fonts::debug_information_size);
			for (int i = 0; i < 11; ++i) {
				read_value(json[("fonts")][("debug_information_flag[") + std::to_string(i) + ("]")], fonts::debug_information_flag[i]);
			}

			// skins namespace
			read_value(json[("skins")][("agent_changer")], skins::agent_changer);
			read_value(json[("skins")][("agent_t")], skins::agent_t);
			read_value(json[("skins")][("agent_ct")], skins::agent_ct);
			read_value(json[("skins")][("knife_changer_enable")], skins::knife_changer_enable);
			read_value(json[("skins")][("knife_changer_wear")], skins::knife_changer_wear);
			read_value(json[("skins")][("knife_changer_model")], skins::knife_changer_model);
			read_value(json[("skins")][("knife_changer_skin_id")], skins::knife_changer_skin_id);
			read_value(json[("skins")][("knife_changer_paint_kit")], skins::knife_changer_paint_kit);
			read_value(json[("skins")][("knife_changer_vector_paint_kit")], skins::knife_changer_vector_paint_kit);
			read_value(json[("skins")][("knife_skin")], skins::knife_skin);
			read_value(json[("skins")][("gloves_endable")], skins::gloves_endable);
			read_value(json[("skins")][("gloves_model")], skins::gloves_model);
			read_value(json[("skins")][("gloves_skin")], skins::gloves_skin);
			read_value(json[("skins")][("gloves_skin_id")], skins::gloves_skin_id);
			read_value(json[("skins")][("gloves_wear")], skins::gloves_wear);
			read_value(json[("skins")][("agent_model")], skins::agent_model);
			read_value(json[("skins")][("skin_custom_clr")], skins::skin_custom_clr);
			for (int i = 0; i < 3; ++i) {
				read_value(json[("skins")][("skin_modulation1[") + std::to_string(i) + ("]")], skins::skin_modulation1[i]);
				read_value(json[("skins")][("skin_modulation2[") + std::to_string(i) + ("]")], skins::skin_modulation2[i]);
				read_value(json[("skins")][("skin_modulation3[") + std::to_string(i) + ("]")], skins::skin_modulation3[i]);
				read_value(json[("skins")][("skin_modulation4[") + std::to_string(i) + ("]")], skins::skin_modulation4[i]);
			}
		}

		path.erase(path.size() - configs.at(index).size() - 5);
	}
}