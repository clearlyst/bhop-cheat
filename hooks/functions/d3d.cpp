#include "../hooks.hpp"
#include "../../menu/menu.hpp"
#include "../../features/misc/misc.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/movement/movement.hpp"
#include "../../sdk/math/math.hpp"
#include <TlHelp32.h>

LRESULT __stdcall sdk::hooks::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	if (message == WM_KEYDOWN && LOWORD(wparam) == c::misc::menu_key)
		menu::open = !menu::open;

	LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	ImGui_ImplWin32_WndProcHandler(window, message, wparam, lparam);

	return CallWindowProcW(wndproc_original, hwnd, message, wparam, lparam);
}

long __stdcall sdk::hooks::present::present(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region) {

	if (menu::fonts_initialized) {
		im_render.reload_setup_font();
		menu::fonts_initialized = false;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame(); {
		const auto im_draw = ImGui::GetBackgroundDrawList();
		im_render.render_present(im_draw);
		menu::render();
	}
	ImGui::EndFrame();
	ImGui::Render();

	if (device->BeginScene() == D3D_OK) {
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	return ofunc(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

LRESULT __stdcall sdk::hooks::reset::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp) {
	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto result = ofunc(device, pp);

	ImGui_ImplDX9_CreateDeviceObjects();

	return result;
}