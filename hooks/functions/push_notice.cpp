#include "../../features/movement/movement.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/aimbot/aimbot.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"
#include "../../sdk/sdk.hpp"
#include "../hooks.hpp"

std::string rgb_to_hex(int r, int g, int b, bool with_hash) {
	std::stringstream ss;
	if (with_hash)
		ss << "#";

	ss << std::hex << (r << 16 | g << 8 | b);
	return ss.str();
}

void __fastcall sdk::hooks::push_notice::push_notice(int ecx, int edx, const char* text, int str_len, const char* null) {
	auto print_text = [](int m_ecx, int m_edx, std::string str) {
		std::string this_str = std::vformat(
			xs("<font color=\"#bebebe\">[</font></font><font color=\"{}\">bhop cheat</font><font color=\"#bebebe\">]</font><font color=\"{}\"> {}</font>"),
			std::make_format_args(rgb_to_hex(menu::menu_accent[0] * 255, menu::menu_accent[1] * 255, menu::menu_accent[2] * 255, true), rgb_to_hex(190, 190, 190, true), str));

		return ofunc(m_ecx, m_edx, this_str.data(), strlen(this_str.data()), this_str.data());
	};

	switch (FNV1A::Hash(text)) {
	case FNV1A::HashConst("#_print_created"):
		return print_text(ecx, edx, xs("created config"));
		break;
	case FNV1A::HashConst("#_print_saved"):
		return print_text(ecx, edx, xs("saved config"));
		break;
	case FNV1A::HashConst("#_print_canceled_save"):
		return print_text(ecx, edx, xs("canceled save config"));
		break;
	case FNV1A::HashConst("#_print_loaded"):
		return print_text(ecx, edx, xs("loaded config"));
		break;
	case FNV1A::HashConst("#_print_refreshed"):
		return print_text(ecx, edx, xs("refreshed config list"));
		break;
	case FNV1A::HashConst("#_print_reloaded"):
		return print_text(ecx, edx, xs("reloaded fonts"));
		break;
	case FNV1A::HashConst("#_print_reseted"):
		return print_text(ecx, edx, xs("reseted fonts"));
		break;
	case FNV1A::HashConst("#_print_injected"):
		return print_text(ecx, edx, xs("successfully injected"));
		break;
	case FNV1A::HashConst("#_print_updated_hud"):
		return print_text(ecx, edx, xs("force updated"));
		break;
	case FNV1A::HashConst("#_print_created_checkpoint"):
		return print_text(ecx, edx, xs("succesfully created & saved checkpoint"));
		break;
	case FNV1A::HashConst("#_print_teleport_checkpoint"):
		return print_text(ecx, edx, xs("teleported to checkpoint"));
		break;
	case FNV1A::HashConst("#_print_jumpbug"):
		return print_text(ecx, edx, xs("jumpbugged"));
		break;
	case FNV1A::HashConst("#_print_edgebug"):
		return print_text(ecx, edx, xs("edgebugged"));
		break;
	case FNV1A::HashConst("#_print_pixelsurf"):
		return print_text(ecx, edx, xs("pixelsurfed"));
		break;
	case FNV1A::HashConst("#_print_hit_1"):
		return print_text(ecx, edx, std::vformat(xs("hit {} for {} damage ({} remaining)."), std::make_format_args(features::misc::hitinfo.player_name.c_str(), features::misc::hitinfo.damage, features::misc::hitinfo.health)));
		break;
	case FNV1A::HashConst("#_print_hit_2"):
		return print_text(ecx, edx, std::vformat(xs("hit {} in the {} for {} damage ({} remaining)."), std::make_format_args(features::misc::hitinfo.player_name.c_str(), features::misc::hitinfo.hitgroup, features::misc::hitinfo.damage, features::misc::hitinfo.health)));
		break;
	}

	ofunc(ecx, edx, text, str_len, null);
}