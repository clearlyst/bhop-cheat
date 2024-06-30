#include "menu.hpp"
#include "../hooks/hooks.hpp"
#include "config/config.hpp"
#include "fonts/menu_font.hpp"
#include "fonts/icon.h"
#include "../features/misc/misc.hpp"
#include "../features/movement/movement.hpp"
#include "../features/skins/skins.hpp"
#include "../includes/imgui/imgui_internal.h"
#include "../features/skins/skins.hpp"
#include <string>
#include <map>

void menu::render_tab(const char* szTabBar, const ctab* arrTabs, const std::size_t nTabsCount, int* nCurrentTab, const ImVec4& colActive, ImGuiTabBarFlags flags) {
    // set active tab color
    ImGui::PushStyleColor(ImGuiCol_TabHovered, colActive);
    ImGui::PushStyleColor(ImGuiCol_TabActive, colActive);
    if (ImGui::BeginTabBar(szTabBar, flags)) {
        for (std::size_t i = 0U; i < nTabsCount; i++) {
            // add tab
            if (ImGui::BeginTabItem(arrTabs[i].szName)) {
                // set current tab index
                *nCurrentTab = i;
                ImGui::EndTabItem();
            }
        }

        // render inner tab
        if (arrTabs[*nCurrentTab].render_function != nullptr)
            arrTabs[*nCurrentTab].render_function();

        ImGui::EndTabBar();
    }
    ImGui::PopStyleColor(2);
}

std::vector<std::string> menu::get_installed_fonts() {
    std::vector<std::string> fonts;
    fonts.push_back("default");
    for (const auto& entry : std::filesystem::directory_iterator("C:\\Windows\\Fonts")) {
        std::string fontName = entry.path().filename().string();
        if (fontName.find(".ttf") != std::string::npos) {
            fonts.push_back(fontName);
        }
    }
    return fonts;
}

void menu::load_font_index() {
    fonts::selected_font_index_main_indi = c::fonts::indi_font;
    if (fonts::selected_font_index_main_indi >= 0) {
        if (menu::fonts[fonts::selected_font_index_main_indi] == "default") {
            fonts::font_directory_indicator = "C:/windows/fonts/tahomabd.ttf";
        }
        else {
            fonts::font_directory_indicator = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_main_indi];
        }
        c::fonts::indi_font = fonts::selected_font_index_main_indi;
    }

    fonts::selected_font_index_sub_indi = c::fonts::sub_indi_font;
    if (fonts::selected_font_index_sub_indi >= 0) {
        if (menu::fonts[fonts::selected_font_index_sub_indi] == "default") {
            fonts::font_directory_sub_indicator = "C:/windows/fonts/tahomabd.ttf";
        }
        else {
            fonts::font_directory_sub_indicator = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_sub_indi];
        }
        c::fonts::sub_indi_font = fonts::selected_font_index_sub_indi;
    }

    fonts::selected_font_index_main_scene = c::fonts::scene_font;
    if (fonts::selected_font_index_main_scene >= 0) {
        if (menu::fonts[fonts::selected_font_index_main_scene] == "default") {
            fonts::font_directory_scene = "C:/windows/fonts/tahomabd.ttf";
        }
        else {
            fonts::font_directory_scene = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_main_scene];
        }
        c::fonts::scene_font = fonts::selected_font_index_main_scene;
    }

    fonts::selected_font_index_sub_scene = c::fonts::scene_sub_font;
    if (fonts::selected_font_index_sub_scene >= 0) {
        if (menu::fonts[fonts::selected_font_index_sub_scene] == "default") {
            fonts::font_directory_sub_scene = "C:/windows/fonts/tahomabd.ttf";
        }
        else {
            fonts::font_directory_sub_scene = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_sub_scene];
        }
        c::fonts::scene_sub_font = fonts::selected_font_index_sub_scene;
    }

    fonts::selected_font_index_main_esp = c::fonts::esp_font;
    if (fonts::selected_font_index_main_esp >= 0) {
        if (menu::fonts[fonts::selected_font_index_main_esp] == "default") {
            fonts::font_directory_esp = "C:/windows/fonts/tahomabd.ttf";
        }
        else {
            fonts::font_directory_esp = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_main_esp];
        }
        c::fonts::esp_font = fonts::selected_font_index_main_esp;
    }

    fonts::selected_font_index_sub_esp = c::fonts::esp_sub_font;
    if (fonts::selected_font_index_sub_esp >= 0) {
        if (menu::fonts[fonts::selected_font_index_sub_esp] == "default") {
            fonts::font_directory_sub_esp = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_sub_esp = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_sub_esp];
        }
        c::fonts::esp_sub_font = fonts::selected_font_index_sub_esp;
    }

    fonts::selected_font_index_debug_information = c::fonts::debug_information_font;
    if (fonts::selected_font_index_debug_information >= 0) {
        if (menu::fonts[fonts::selected_font_index_debug_information] == "default") {
            fonts::font_directory_debug_information = "C:/windows/fonts/tahomabd.ttf";
        }
        else {
            fonts::font_directory_debug_information = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_debug_information];
        }
        c::fonts::debug_information_font = fonts::selected_font_index_debug_information;
    }
}


bool menu::iskeydown(int key) {
    return HIWORD(GetKeyState(key));
}

bool menu::iskeyup(int key) {
    return !HIWORD(GetKeyState(key));
}

bool menu::checkkey(int key, int keystyle) {
    switch (keystyle) {
    case 0:
        return true;
    case 1:
        return menu::iskeydown(key);
    case 2:
        return LOWORD(GetKeyState(key));
    case 3:
        return menu::iskeyup(key);
    default:
        return true;
    }
}

void  menu::switch_font_cfg(ImFontConfig& f, bool cfg[9]) {
    f.FontBuilderFlags = 0;
    if (cfg[0]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoHinting;
    if (cfg[1]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoAutoHint;
    if (cfg[2]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LightHinting;
    if (cfg[3]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_MonoHinting;
    if (cfg[4]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bold;
    if (cfg[5]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Oblique;
    if (cfg[6]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome;
    if (cfg[7]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
    if (cfg[8]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bitmap;
}
