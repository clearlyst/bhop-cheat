#include "../hooks.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"

void __fastcall sdk::hooks::override_view::override_view( void* _this, void* _edx, view_setup_t* setup ) 
{
	features::misc::viewfov();
	features::misc::viewmodel();
	features::misc::swayscale();
	features::visuals::thirdperson();
	features::visuals::freecam(setup);
	features::visuals::motion_blur(setup);
	features::visuals::weather::precipitation_update();

	ofunc( interfaces::client_mode, _this, setup );
}

bool __stdcall sdk::hooks::is_depth_of_field_enabled::is_depth_of_field_enabled() {
	features::visuals::motion_blur(nullptr);
	return false;
}

constexpr auto VENDORID_NVIDIA = 0x10DEu;
constexpr auto VENDORID_ATI = 0x1002u;
constexpr auto VENDORID_INTEL = 0x8086u;

bool sdk::hooks::supports_resolve_depth::supports_resolve_depth()
{
	material_adapter_info_t info;
	auto adapter = interfaces::material_system->get_current_adapter();
	interfaces::material_system->get_display_adapter_info(adapter, &info);

	switch (c::visuals::world::motionblur::video_adapter)
	{
	case 0:
		if (info.vendor_id == VENDORID_ATI)
			return false;
		break;
	case 1:
		if (info.vendor_id == VENDORID_NVIDIA)
			return false;
		break;
	case 2:
		if (info.vendor_id == VENDORID_INTEL)
			return false;
		break;
	}

	return original();

}
