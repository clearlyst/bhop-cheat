#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <chrono>
#include <thread>

#include "sdk/sdk.hpp"
//#include "utils/xor.hpp"
#include "menu/menu.hpp"
#include "features/events/events.hpp"
#include "hooks/hooks.hpp"
#include "menu/config/config.hpp"
#include "features/misc/misc.hpp"
#include "sdk/netvars/netvars.hpp"
#include "features/skins/skins.hpp"
#include "features/movement/movement.hpp"
#include "features/notifications/notifications.hpp"


#ifdef _DEBUG
bool unload( HMODULE module ) {
    while (!GetAsyncKeyState( VK_END )) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
    }

    if (menu::open) 
        menu::open = false;

    if (c::misc::view_model) 
        c::misc::view_model = false;
   
    c::skins::knife_changer_model = 0;

    features::skins::full_update();

    printf("ozungaware | unloading hooks\n");
    sdk::hooks::unload( );

    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

    printf("ozungaware | unloading\n");

    std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );
    fclose( stdin );
    fclose( stdout );
    FreeConsole( );

    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    FreeLibraryAndExitThread( module, 1 );
    return false;
}
#endif

static DWORD WINAPI on_attach(void* instance) {
	[&]() {
        interfaces::initialize();
        im_render.setup();
        sdk::hooks::init();
        c::create_directory();
        cvar::init();

        interfaces::engine->execute_cmd("clear");
        features::notification::run("successfully injected", "#_print_injected", true, true, true);
	}();

    while (!GetAsyncKeyState(VK_HELP))
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

	[&]() {
        events.unload();
        sdk::hooks::unload();
        im_render.unload();
	}();

	(FreeLibraryAndExitThread)(static_cast<HMODULE>(instance), 0);

	return 0;
}

int __stdcall DllMain(void* instance, unsigned long call_reason, void* reserved) {
    if (call_reason == DLL_PROCESS_ATTACH) {
        (DisableThreadLibraryCalls)(reinterpret_cast<HMODULE>(instance));

        if (const auto thread = (CreateThread)(nullptr, 0, on_attach, instance, 0, nullptr); thread)
            (CloseHandle)(thread);
    }

    return TRUE;
}