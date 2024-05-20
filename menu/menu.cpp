#include "menu.hpp"
#include "config/config.hpp"
#include "fonts/menu_font.hpp"
#include "fonts/icon.h"
#include "../hooks/hooks.hpp"
#include "../features/misc/misc.hpp"
#include "../features/movement/movement.hpp"
#include "../features/skins/skins.hpp"
#include "../includes/imgui/imgui_internal.h"
#include "../features/notifications/notifications.hpp"

static const char* mov_toggle_type[] = { "always on", "on hotkey", "toggle hotkey" };
static const char* key_back[4] = { "-w", "-s", "-a", "-d" };
static const char* key_back_for[4] = { "ej", "mj", "lj", "lej" };
static const char* edge_jump_for[3] = { "lj", "mj", "lej" };
static const char* detect_chat_for[3] = { "jb", "eb", "ps" };
static const char* indicators[14] = { "jb", "ej", "lj", "eb", "mj", "sh", "ps", "al", "ad", "as", "bb", "msl", "so", "null" };
const char* font_flags[] = { "no hinting","no autohint","light hinting","mono hinting","bold","italic","no antialiasing","load color","bitmap","dropshadow","outline" };
const char* fnt_tab[] = { "main indicator font", "sub indicator font", "esp font", "sub esp font", "scene font", "sub scene font" };
static const char* materials[] = { "regular", "flat", "crystal", "pearlescent", "reverse pearlescent", "fog", "damascus", "model" };
static const char* chams_overlay_types[] = { "glow", "outline", "metallic", "snow" };
static const char* flags[7] = { "armour", "money", "flashed", "scoped","reloading", "defusing", "rescuing" };
static const char* outline_type[2] = { "outter", "inner" };
static const char* line_type[3] = { "top", "center", "bottom" };

ImGuiColorEditFlags no_alpha = ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar;
ImGuiColorEditFlags w_alpha = ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar;


void legitbot(c::aimbot::aimbot_value_t& aimbot_value, bool show_override_from_default = false)
{
    ImGuiStyle& style = ImGui::GetStyle();

    ImGui::Columns(2, nullptr, false); {
        ImGui::BeginChild(("legitbot.main"), ImVec2{ }, true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted("legit");
                ImGui::EndMenuBar();
            }

            static const ctab legit_tabs[] =
            {
                ctab{ "trigger", []()
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));

                ImGui::Checkbox("enable##trigger", &c::triggerbot::enable);
                if (c::triggerbot::enable)
                {
                    ImGui::Keybind("##trigger key", &c::triggerbot::key, &c::triggerbot::key_s);
                }

                ImGui::PopStyleVar();
            }},
                ctab{ "backtrack", []()
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));

                ImGui::Checkbox("enable##backtrack", &c::backtrack::enable);
                if (c::backtrack::enable)
                {
                    ImGui::Keybind("##backtrack key", &c::backtrack::key, &c::backtrack::key_s);
                    ImGui::Checkbox("aim at backtrack", &c::aimbot::aim_at_backtrack);

                    ImGui::Separator();

                    ImGui::Checkbox("extended", &c::backtrack::fake_latency);
                    if (c::backtrack::fake_latency)
                    {
                        ImGui::SliderFloat("ping", &c::backtrack::fake, 0, 200, "%.2f");
                    }
                }

                ImGui::PopStyleVar();
            }}
            };

            menu::render_tab("legit_tabs", legit_tabs, 2U, &menu::aim_tab, style.Colors[ImGuiCol_TabHovered]);

            ImGui::EndChild();
        }
    }
    ImGui::NextColumn(); {
        ImGui::BeginChild(("legitbot.settings"), ImVec2{ }, true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted("aimbot");
                ImGui::EndMenuBar();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, 0));

            ImGui::Text("weapon settings");
            ImGui::Combo("##settings", &menu::subtab, "general\0pistols\0heavy pistols\0smgs\0shotguns\0rifles\0snipers\0");

            ImGui::Separator();

            ImGui::Checkbox("enable", &c::aimbot::enable);
            if (c::aimbot::enable)
            {
                if (!c::aimbot::shoot_at_aim_key)
                {
                    ImGui::Keybind("##aim key", &c::aimbot::key, &c::aimbot::key_s);
                }
                ImGui::Checkbox("work on aimkey", &c::aimbot::shoot_at_aim_key);
                if (show_override_from_default)
                {
                    ImGui::Checkbox("override general", &aimbot_value.override_general);
                }      
                if (show_override_from_default && aimbot_value.override_general)
                {
                    if (ImGui::BeginCombo("hitbox", "select", ImGuiComboFlags_NoArrowButton))
                    {
                        ImGui::Selectable("head", &aimbot_value.head, ImGuiSelectableFlags_DontClosePopups);
                        ImGui::Selectable("neck", &aimbot_value.neck, ImGuiSelectableFlags_DontClosePopups);
                        ImGui::Selectable("chest", &aimbot_value.chest, ImGuiSelectableFlags_DontClosePopups);
                        ImGui::Selectable("stomach", &aimbot_value.stomach, ImGuiSelectableFlags_DontClosePopups);
                        ImGui::Selectable("pelvis", &aimbot_value.pelvis, ImGuiSelectableFlags_DontClosePopups);
                        ImGui::Selectable("foot", &aimbot_value.foot, ImGuiSelectableFlags_DontClosePopups);

                        ImGui::EndCombo();
                    }

                    ImGui::Separator();

                    ImGui::SliderInt("field of view", &aimbot_value.fov, 0, 360, "%d");

                    ImGui::Separator();

                    ImGui::Checkbox("auto pistol", &aimbot_value.autopistol);

                    ImGui::Separator();

                    ImGui::Checkbox("silent", &aimbot_value.silent);

                    ImGui::Separator();

                    ImGui::Checkbox("compensate recoil", &aimbot_value.recoil_compensate);

                    ImGui::Separator();

                    ImGui::Checkbox("auto wall", &aimbot_value.autowall);
                    if (aimbot_value.autowall)
                    {
                        ImGui::SliderInt("penetration minimum damage", &aimbot_value.autowall_damage, 0, 100, "%d");
                    }
                }
                else if (!show_override_from_default)
                {
                    if (ImGui::BeginCombo("hitbox", "select", ImGuiComboFlags_NoArrowButton))
                    {
                        ImGui::Selectable("head", &aimbot_value.head, ImGuiSelectableFlags_DontClosePopups);
                        ImGui::Selectable("neck", &aimbot_value.neck, ImGuiSelectableFlags_DontClosePopups);
                        ImGui::Selectable("chest", &aimbot_value.chest, ImGuiSelectableFlags_DontClosePopups);
                        ImGui::Selectable("stomach", &aimbot_value.stomach, ImGuiSelectableFlags_DontClosePopups);
                        ImGui::Selectable("pelvis", &aimbot_value.pelvis, ImGuiSelectableFlags_DontClosePopups);
                        ImGui::Selectable("foot", &aimbot_value.foot, ImGuiSelectableFlags_DontClosePopups);

                        ImGui::EndCombo();
                    }

                    ImGui::Separator();

                    ImGui::SliderInt("field of view", &aimbot_value.fov, 0, 360, "%d");

                    ImGui::Separator();

                    ImGui::Checkbox("auto pistol", &aimbot_value.autopistol);

                    ImGui::Separator();

                    ImGui::Checkbox("silent", &aimbot_value.silent);

                    ImGui::Separator();

                    ImGui::Checkbox("compensate recoil", &aimbot_value.recoil_compensate);

                    ImGui::Separator();

                    ImGui::Checkbox("auto wall", &aimbot_value.autowall);
                    if (aimbot_value.autowall)
                    {
                        ImGui::SliderInt("penetration minimum damage", &aimbot_value.autowall_damage, 0, 100, "%d");
                    }
                }
            }

            ImGui::PopStyleVar();
            ImGui::EndChild();
        }
    }

    ImGui::Columns(1);
}

void visuals() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 vecWindowPosition = ImGui::GetWindowPos();

    ImGui::Columns(2, nullptr, false); {
        ImGui::BeginChild(("visuals.esp"), ImVec2{0, 190 }, true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted("overlay");
                ImGui::EndMenuBar();
            }

            static const ctab overlay_tabs[] =
            {
                ctab{ "esp", []()
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
                ImGui::Checkbox("enable esp", &c::visuals::enable);
                if (c::visuals::enable) 
                {
                    ImGui::Keybind("esp key", &c::visuals::key, &c::visuals::key_s);
                    ImGui::Checkbox("fade animations", &c::visuals::players::fade::enable);
                    if (c::visuals::players::fade::enable)
                    {
                        ImGui::SliderFloat("fade time", &c::visuals::players::fade::time, 0.0f, 1.0f, ("%.2f"));
                    }
                    ImGui::Checkbox("engine radar", &c::visuals::players::engine_radar);
                    ImGui::Checkbox("custom color##all", &c::visuals::players::colors::custom);
                    if (!c::visuals::players::colors::custom)
                    {
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##visible color", c::visuals::players::colors::visible, w_alpha);
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##invisible color", c::visuals::players::colors::invisible, w_alpha);
                    }
                    ImGui::Checkbox("box", &c::visuals::players::box::enable);
                    if (c::visuals::players::box::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##player box color", c::visuals::players::colors::box, w_alpha);
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##player outline box color", c::visuals::players::colors::box_outline, w_alpha);
                        }
                        ImGui::Text("box type");
                        ImGui::Combo("##boxtype", &c::visuals::players::box::type, "filled\0corner");
                        ImGui::Text("outline");
                        ImGui::MultiCombo("##outlinetype", outline_type, c::visuals::players::box::outline, 2);

                        if (c::visuals::players::box::type == 1)
                        {
                            ImGui::SliderFloat("corner lenght", &c::visuals::players::box::lenght, 1.55f, 20.0f, ("%.2f"));
                        }
                    }
                    ImGui::Checkbox("name", &c::visuals::players::name::enable);
                    if (c::visuals::players::name::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##name color", c::visuals::players::colors::name, w_alpha);
                        }
                        ImGui::Checkbox("bot tag", &c::visuals::players::name::bot_check);
                    }
                    ImGui::Checkbox("health bar", &c::visuals::players::health_bar::enable);
                    if (c::visuals::players::health_bar::enable)
                    {
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##healthbar outline color", c::visuals::players::colors::health_bar_outline, w_alpha);
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##healthbar color", c::visuals::players::colors::health_bar, w_alpha);
                        }
                        ImGui::Checkbox("gradient color", &c::visuals::players::health_bar::gradient);
                        if (c::visuals::players::health_bar::gradient)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##healthbar upper color", c::visuals::players::colors::health_bar_upper, w_alpha);
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##healthbar lower color", c::visuals::players::colors::health_bar_lower, w_alpha);
                        }

                        ImGui::Checkbox("color based on health##bar", &c::visuals::players::health_bar::baseonhealth);
                        ImGui::Checkbox("fill outline background##healthbar", &c::visuals::players::health_bar::background);
                        ImGui::Checkbox("text on bar", &c::visuals::players::health_bar::text);
                        ImGui::SliderInt("bar width##healthbar", &c::visuals::players::health_bar::size, 1, 10, ("%.d"));
                    }
                    ImGui::Checkbox("skeleton", &c::visuals::players::skeleton::enable);
                    if (c::visuals::players::skeleton::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##skeleton color", c::visuals::players::colors::skeleton, w_alpha);
                        }

                        ImGui::SliderFloat("thickness##skeleton", &c::visuals::players::skeleton::thickness, 0.50f, 2.0f, ("%.2f"));
                    }
                    ImGui::Checkbox("backtrack skeleton", &c::visuals::players::backtrack_skeleton::enable);
                    if (c::visuals::players::backtrack_skeleton::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##backtrack skeleton color", c::visuals::players::colors::backtrack_skeleton, w_alpha);
                        }

                        ImGui::SliderFloat("thickness##backtrack", &c::visuals::players::backtrack_skeleton::thickness, 0.50f, 2.0f, ("%.2f"));
                    }
                    ImGui::Checkbox("backtrack dot", &c::visuals::players::backtrack_dot::enable);
                    if (c::visuals::players::backtrack_dot::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##backtrack dot color", c::visuals::players::colors::backtrack_dot, w_alpha);
                        }
                    }
                    ImGui::Checkbox("view angle", &c::visuals::players::view_angle::enable);
                    if (c::visuals::players::view_angle::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##view angle color", c::visuals::players::colors::view_angle, w_alpha);
                        }
                        ImGui::SliderFloat("thickness##viewangle", &c::visuals::players::view_angle::thickness, 0.50f, 2.0f, ("%.2f"));
                        ImGui::SliderFloat("distance##viewangle", &c::visuals::players::view_angle::distance, 0.0f, 150.0f, ("%.2f"));
                    }
                    ImGui::Checkbox("line to box", &c::visuals::players::snapline::enable);
                    if (c::visuals::players::snapline::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##snap line color", c::visuals::players::colors::snapline, w_alpha);
                        }
                        ImGui::Text("line type");
                        ImGui::MultiCombo("##linetype", line_type, c::visuals::players::snapline::type, 3);
                        ImGui::SliderFloat("thickness##linetobox", &c::visuals::players::snapline::thickness, 0.50f, 2.0f, ("%.2f"));
                    }
                    ImGui::Checkbox("flags", &c::visuals::players::flags::enable);
                    if (c::visuals::players::flags::enable)
                    {
                        ImGui::Text("flags type");
                        ImGui::MultiCombo("##flagstype", flags, c::visuals::players::flags::flags, 7);
                    }
                    ImGui::Checkbox("ammo bar", &c::visuals::players::ammo_bar::enable);
                    if (c::visuals::players::ammo_bar::enable)
                    {
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##ammobar outline color", c::visuals::players::colors::ammo_bar_outline, w_alpha);
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##ammobar color", c::visuals::players::colors::ammo_bar, w_alpha);
                        }
                        ImGui::Checkbox("fill outline background##ammobar", &c::visuals::players::ammo_bar::background);
                        ImGui::SliderInt("bar width##ammobar", &c::visuals::players::ammo_bar::size, 1, 10, ("%.d"));
                    }
                    ImGui::Checkbox("health text", &c::visuals::players::health_text::enable);
                    if (c::visuals::players::health_text::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##health text color", c::visuals::players::colors::health_text, w_alpha);
                        }
                        ImGui::Checkbox("hide suffix", &c::visuals::players::health_text::hide_suffix);
                        ImGui::Checkbox("hide at max", &c::visuals::players::health_text::hide_at_max);
                        ImGui::Checkbox("color based on health##text", &c::visuals::players::health_text::baseonhealth);
                    }
                    ImGui::Checkbox("weapon icon", &c::visuals::players::weapon_icon::enable);
                    if (c::visuals::players::weapon_icon::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##weapon icon color", c::visuals::players::colors::weapon_icon, w_alpha);
                        }
                    }
                    ImGui::Checkbox("weapon text", &c::visuals::players::weapon_text::enable);
                    if (c::visuals::players::weapon_text::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##weapon text color", c::visuals::players::colors::weapon_text, w_alpha);
                        }
                    }
                    ImGui::Checkbox("ammo text", &c::visuals::players::ammo_text::enable);
                    if (c::visuals::players::ammo_text::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##ammo icon color", c::visuals::players::colors::ammo_text, w_alpha);
                        }
                    }
                    ImGui::Checkbox("distance", &c::visuals::players::distance::enable);
                    if (c::visuals::players::distance::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##distancecolor", c::visuals::players::colors::distance, w_alpha);
                        }
                    }
                    ImGui::Checkbox("out of fov", &c::visuals::players::out_of_view::enable);
                    if (c::visuals::players::out_of_view::enable)
                    {
                        if (c::visuals::players::colors::custom)
                        {
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##oofcolor", c::visuals::players::colors::out_of_view, w_alpha);
                        }
                        ImGui::SliderFloat("size", &c::visuals::players::out_of_view::size, 0.0f, 50.0f, ("%.2f"));
                        ImGui::SliderFloat("distance", &c::visuals::players::out_of_view::distance, 0.0f, 300.0f, ("%.2f"));
                    }
                    ImGui::Checkbox("emitted sound", &c::visuals::players::emitted_sound::enable);
                    if (c::visuals::players::emitted_sound::enable)
                    {
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##soundscolor", c::visuals::players::colors::sounds, w_alpha);
                    }
                }

                ImGui::PopStyleVar();
            }},
                ctab{ "world", []()
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));

                ImGui::Checkbox("dropped weapon box (wip)", &c::visuals::players::dropped_weapon::box::enable);
                if (c::visuals::players::dropped_weapon::box::enable)
                {
                    if (c::visuals::players::colors::custom)
                    {
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##entity box color", c::visuals::players::colors::box, w_alpha);
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##entity outline box color", c::visuals::players::colors::box_outline, w_alpha);
                    }
                    ImGui::Text("box type");
                    ImGui::Combo("##boxtype", &c::visuals::players::dropped_weapon::box::type, "filled\0corner");
                    ImGui::Text("outline");
                    ImGui::MultiCombo("##outlinetype", outline_type, c::visuals::players::dropped_weapon::box::outline, 2);

                    if (c::visuals::players::dropped_weapon::box::type == 1)
                    {
                        ImGui::SliderFloat("corner lenght", &c::visuals::players::dropped_weapon::box::lenght, 1.55f, 20.0f, ("%.2f"));
                    }
                }
                ImGui::Checkbox("dropped weapon name", &c::visuals::players::dropped_weapon::text::enable);
                ImGui::Checkbox("dropped weapon icon", &c::visuals::players::dropped_weapon::icon::enable);
                ImGui::Checkbox("dropped weapon ammo text", &c::visuals::players::dropped_weapon::ammo_text::enable);
                if (c::visuals::players::dropped_weapon::text::enable || c::visuals::players::dropped_weapon::icon::enable || c::visuals::players::dropped_weapon::ammo_text::enable)
                {
                    ImGui::Text("dropped weapon text color");
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##dropped weapon color", c::visuals::players::dropped_weapon::color_text, w_alpha);
                }
                ImGui::Checkbox("dropped weapon ammo bar (wip)", &c::visuals::players::dropped_weapon::ammo_bar::enable);
                ImGui::Checkbox("thrown grenade name", &c::visuals::players::thrown_grenade::text::enable);
                ImGui::Checkbox("thrown grenade icon", &c::visuals::players::thrown_grenade::icon::enable);
                if (c::visuals::players::thrown_grenade::text::enable || c::visuals::players::thrown_grenade::icon::enable)
                {
                    ImGui::Text("thrown grenade color");
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##thrown grenade color", c::visuals::players::thrown_grenade::color, w_alpha);
                }
                ImGui::PopStyleVar();
            }}
            };

            menu::render_tab(("overlay_tabs"), overlay_tabs, 2U, &menu::esp_tab, style.Colors[ImGuiCol_TabHovered]);

            ImGui::EndChild();
        }

        ImGui::BeginChild(("visuals.glow"), ImVec2{ }, true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted("glow");
                ImGui::EndMenuBar();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, 0));

            ImGui::Checkbox("enable glow", &c::visuals::glow);
            if (c::visuals::glow)
            {
                ImGui::SameLine();
                ImGui::ColorEdit4("##glow visible color", c::visuals::glow_visible_clr, w_alpha);
                ImGui::SameLine();
                ImGui::ColorEdit4("##glow invisible color", c::visuals::glow_invisible_clr, w_alpha);
                ImGui::Text("glow style");
                ImGui::Combo("##glow type", &c::visuals::glow_style, "default\0pusling\0outline\0outline pulse\0");
            }
            ImGui::Checkbox("enable dlight", &c::visuals::players::dlight::enable);
            if (c::visuals::players::dlight::enable)
            {
                ImGui::SameLine();
                ImGui::ColorEdit4("##dlight color", c::visuals::players::dlight::color, w_alpha);

                ImGui::SliderFloat("radius", &c::visuals::players::dlight::radius, 25.0f, 200.0f, ("%.2f"));
            }

            ImGui::PopStyleVar();
            ImGui::EndChild();
        }
    }
    ImGui::NextColumn(); {
        ImGui::BeginChild(("visuals.chams"), ImVec2{0, 190 }, true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted("chams");
                ImGui::EndMenuBar();
            }

            static const ctab cham_tabs[] =
            {
                ctab{ "enemy", []()
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));

                ImGui::Checkbox(("enable visible chams"), &c::chams::visible_chams);
                ImGui::SameLine();
                ImGui::ColorEdit4(("##visible clr"), c::chams::visible_chams_clr, w_alpha);
                ImGui::Checkbox(("visible wireframe"), &c::chams::visible_wireframe);
                ImGui::Checkbox(("enable invisible chams"), &c::chams::invisible_chams);
                ImGui::SameLine();
                ImGui::ColorEdit4(("##invisible clr"), c::chams::invisible_chams_clr, w_alpha);
                ImGui::Checkbox(("invisible wireframe"), &c::chams::invisible_wireframe);
                ImGui::Text(("material"));
                ImGui::Combo(("##vismat"), &c::chams::cham_type, materials, IM_ARRAYSIZE(materials));
                ImGui::Checkbox(("overlay chams"), &c::chams::visible_chams_ov);
                ImGui::SameLine();
                ImGui::ColorEdit4(("##ovcolor0"), c::chams::visible_chams_clr_ov, w_alpha);
                if (c::chams::visible_chams_ov) {
                    ImGui::Checkbox(("overlay wireframe"), &c::chams::visible_wireframe_ov);
                    ImGui::Text(("material"));
                    ImGui::Combo(("##overlay"), &c::chams::cham_type_overlay, chams_overlay_types, IM_ARRAYSIZE(chams_overlay_types));
                }

                ImGui::PopStyleVar();
            }},

                ctab{ "backtrack", []()
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
                ImGui::Checkbox(("enable backtrack chams##bt"), &c::chams::backtrack_chams);
                ImGui::SameLine();
                ImGui::ColorEdit4(("##playerbt1"), c::chams::backtrack_chams_clr2, w_alpha);
                ImGui::Checkbox(("enable invisible chams##bt"), &c::chams::backtrack_chams_invisible);
                ImGui::Checkbox(("draw all ticks##bt"), &c::chams::backtrack_chams_draw_all_ticks);
                if (c::chams::backtrack_chams_draw_all_ticks) {
                    ImGui::Checkbox(("gradient##bt"), &c::chams::backtrack_chams_gradient);
                    ImGui::SameLine();
                    ImGui::ColorEdit4(("##playerbt2"), c::chams::backtrack_chams_clr1, w_alpha);
                }
                ImGui::Text(("material"));
                ImGui::Combo(("##backtrack"), &c::chams::cham_type_bt, materials, IM_ARRAYSIZE(materials));

                ImGui::PopStyleVar();
            }}
            };

            menu::render_tab(("cham_tabs"), cham_tabs, 2U, &menu::chams_tab, style.Colors[ImGuiCol_TabHovered]);

            ImGui::EndChild();
        }

        ImGui::BeginChild(("visuals.other"), ImVec2{ }, true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted("other");
                ImGui::EndMenuBar();
            }

            static const ctab visu_tabs[] =
            {
                ctab{ "scene", []()
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));

                ImGui::Checkbox("spectator list", &c::misc::spectators_list);
                if (c::misc::spectators_list)
                {
                    ImGui::Checkbox("avatars (wip)", &c::misc::spectators_list_avatars);
                    ImGui::Checkbox("mode", &c::misc::spectators_list_mode);
                }

                ImGui::Checkbox("debug information", &c::misc::debug_information::enable);
                if (c::misc::debug_information::enable)
                {
                    ImGui::Checkbox("weapon ready to fire", &c::misc::debug_information::can_fire::enable);
                    if (c::misc::debug_information::can_fire::enable)
                    {
                        ImGui::Text("ready to shoot");
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##active color", c::misc::debug_information::can_fire::active_color, no_alpha);
                        ImGui::Text("not ready to shoot");
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##inactive color", c::misc::debug_information::can_fire::inactive_color, no_alpha);
                    }
                    ImGui::Checkbox("checkpoint system", &c::misc::debug_information::check_point_system::enable);
                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("only if turn on practice binds");
                    }
                    if (c::misc::debug_information::check_point_system::enable)
                    {
                        ImGui::Text("saved point");
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##active colorsp", c::misc::debug_information::check_point_system::active_color, no_alpha);
                        ImGui::Text("succesfull teleported");
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##inactive colortp", c::misc::debug_information::check_point_system::inactive_color, no_alpha);
                    }
                }
                ImGui::Separator();

                ImGui::Checkbox("change weapon sway scale", &c::misc::swayscale);
                if (c::misc::swayscale)
                {
                    ImGui::SliderFloat("weapon sway scale", &c::misc::swayscale_value, 0.f, 100.f, ("%.2f"));
                }
                ImGui::Checkbox("change viewmodel fov", &c::misc::enable_fov);
                if (c::misc::enable_fov)
                {
                    ImGui::SliderFloat("viewmodel fov", &c::misc::field_of_view, 54.0f, 120.0f, ("%.2f"));
                }
                ImGui::Checkbox("change viewmodel offsets", &c::misc::view_model);
                if (c::misc::view_model)
                {
                    ImGui::SliderFloat("offset x", &c::misc::view_model_x, -25.0f, 25.0f, ("%.2f"));
                    ImGui::SliderFloat("offset y", &c::misc::view_model_y, -25.0f, 25.0f, ("%.2f"));
                    ImGui::SliderFloat("offset z", &c::misc::view_model_z, -25.0f, 25.0f, ("%.2f"));
                }

                ImGui::Separator();

                ImGui::Checkbox("force crosshair", &c::misc::force_crosshair);
                ImGui::Checkbox("sniper crosshair", &c::misc::sniper_crosshair);
                ImGui::Checkbox("recoil crosshair", &c::misc::recoil_crosshair);
                ImGui::Checkbox("spread circle", &c::misc::spread_circle::enable);
                if (c::misc::spread_circle::enable)
                {
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##spread circle color", c::misc::spread_circle::color, no_alpha);
                }
                ImGui::Checkbox("penetration reticle (wip)", &c::misc::penetration_reticle::enable);

                ImGui::Separator();

                ImGui::Checkbox("free camera", &c::misc::freecam);
                if (c::misc::freecam)
                {
                    ImGui::Keybind("free camera key", &c::misc::freecam_key, &c::misc::freecam_key_s);
                }
                ImGui::Checkbox("preverse killfeed", &c::misc::preverse_killfeed::enable);
                if (c::misc::preverse_killfeed::enable)
                {
                    ImGui::SliderFloat("time", &c::misc::preverse_killfeed::time, 0.0f, 10.0f, ("%.2f"));
                }

                ImGui::Separator();

                ImGui::Checkbox("grenade prediction", &c::misc::nadepred);
                if (c::misc::nadepred)
                {
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##grenade prediction color", c::misc::nadepred_clr, no_alpha);
                }

                ImGui::Spacing();

                ImGui::PopStyleVar();
            }},

                ctab{ "world", []()
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));

                ImGui::Checkbox("fog modulation", &c::visuals::world::fog::enable);
                if (c::visuals::world::fog::enable)
                {
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##fog color", c::visuals::world::fog::color, w_alpha);
                    ImGui::SliderFloat("start", &c::visuals::world::fog::start, -5000.0f, 600.0f, ("%.2f"));
                    ImGui::SliderFloat("end", &c::visuals::world::fog::end, 0, 5000.0f, ("%.2f"));
                    ImGui::SliderFloat("density", &c::visuals::world::fog::density, 0.0f, 1.0f, ("%.2f"));
                }
                ImGui::Checkbox("shadow modulation", &c::visuals::world::shadow::enable);
                if (c::visuals::world::shadow::enable)
                {
                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("if you want restore shadows setup all on zero");
                    }

                    ImGui::SliderFloat("rotation x", &c::visuals::world::shadow::x, -1000.0f, 1000.0f, ("%.2f"));
                    ImGui::SliderFloat("rotation y", &c::visuals::world::shadow::y, -1000.0f, 1000.0f, ("%.2f"));
                    ImGui::SliderFloat("rotation z", &c::visuals::world::shadow::z, -1000.0f, 1000.0f, ("%.2f"));
                }
                ImGui::Checkbox("skybox modulation", &c::visuals::world::skybox::enable);
                if (c::visuals::world::skybox::enable)
                {
                    ImGui::Text("change skybox");
                    ImGui::Combo("##skybox", &c::visuals::world::skybox::style, "none\0baggage\0tibet\0embassy\0italy\0jungle\0nuke\0office\0daylight 1\0daylight 2\0daylight 3\0daylight 4\0cloudy 1\0night 1\0night 2\0day 2 hdr\0day 2\0dust\0rural\0venice\0vertigo hdr\0vertigo\0vertigo blue hdr\0vietnam\0lunacy\0aztec\0");
                }
                ImGui::Checkbox("world modulation color", &c::visuals::world::world_modulation::enable);
                if (c::visuals::world::world_modulation::enable)
                {
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##world color", c::visuals::world::world_modulation::color, w_alpha);
                }
                ImGui::Checkbox("sky modulation color", &c::visuals::world::skybox_modulation::enable);
                if (c::visuals::world::skybox_modulation::enable)
                {
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##sky color", c::visuals::world::skybox_modulation::color, w_alpha);
                }
                ImGui::Checkbox("props modulation color", &c::visuals::world::props_modulation::enable);
                if (c::visuals::world::props_modulation::enable)
                {
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##props color", c::visuals::world::props_modulation::color, w_alpha);
                }
                ImGui::Checkbox("bloom modulation", &c::visuals::world::bloom::enable);
                if (c::visuals::world::bloom::enable)
                {
                    ImGui::SliderFloat("exposure min", &c::visuals::world::bloom::exposure_min, 0.0f, 10.0f, ("%.2f"));
                    ImGui::SliderFloat("exposure max", &c::visuals::world::bloom::exposure_max, 0.0f, 10.0f, ("%.2f"));
                    ImGui::SliderFloat("scale", &c::visuals::world::bloom::scale, 0.0f, 10.0f, ("%.2f"));
                }
                ImGui::Checkbox("brightness modulation", &c::visuals::world::brightness::enable);
                if (c::visuals::world::brightness::enable)
                {
                    ImGui::SliderFloat("amount", &c::visuals::world::brightness::amount, 0.0f, 10.0f, ("%.2f"));
                }
                ImGui::Checkbox("particles color", &c::visuals::world::particles::enable);
                if (c::visuals::world::particles::enable)
                {
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##particles color", c::visuals::world::particles::color, w_alpha);
                }
                ImGui::Checkbox("full bright", &c::visuals::world::fullbright::enable);
                ImGui::Checkbox("ragdoll modulation", &c::visuals::world::ragdoll::enable);
                if (c::visuals::world::ragdoll::enable)
                {
                    ImGui::Text("style");
                    ImGui::Combo("##ragdoll", &c::visuals::world::ragdoll::style, "none\0from us\0to us\0to sky\0");
                }

                ImGui::Separator();

                ImGui::Checkbox("remove/reduce flash", &c::visuals::flash::enable);
                if (c::visuals::flash::enable)
                {
                    ImGui::Checkbox("remove flash", &c::visuals::flash::full);

                    if (!c::visuals::flash::full)
                    {
                        ImGui::SliderInt("flash reduce", &c::visuals::flash::alpha, 0, 100, "%d");
                    }
                }
                ImGui::Checkbox("remove smoke", &c::visuals::smoke::enable);
                ImGui::Checkbox("remove scope", &c::visuals::scope::enable);
                if (c::visuals::scope::enable)
                {
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##scope color", c::visuals::scope::color, w_alpha);
                }
                ImGui::Checkbox("remove panorama blur", &c::visuals::panorama_blur::enable);
                ImGui::Checkbox("remove post processing", &c::visuals::post_processing::enable);
                ImGui::Checkbox("remove player model", &c::visuals::player_model::enable);
                ImGui::Checkbox("remove player sleeves", &c::visuals::player_sleeves::enable);

                ImGui::Spacing();

                ImGui::PopStyleVar();
            }}
            };

            menu::render_tab(("visual_tabs"), visu_tabs, 2U, &menu::visu_tab, style.Colors[ImGuiCol_TabHovered]);

            ImGui::EndChild();
        }
    }
    ImGui::Columns(1);
}

void miscellaneous() {
    ImGuiStyle& style = ImGui::GetStyle();

    ImGui::Columns(2, nullptr, false); {
        ImGui::BeginChild(("misc.movement"), ImVec2(0, 190), true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted(("movement"));
                ImGui::EndMenuBar();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, 0));

            ImGui::Checkbox("auto bunny hop", &c::movement::bhop);
            ImGui::Checkbox("auto strafe", &c::movement::auto_strafe);
            if (c::movement::auto_strafe)
            {
                ImGui::Keybind("##auto strafe key", &c::movement::auto_strafe_key, &c::movement::auto_strafe_key_s);
                ImGui::SliderFloat("minimum speed", &c::movement::auto_strafe_minimum_speed, 0.0f, 30.0f, ("%.1f"));
                ImGui::SliderFloat("maximum speed", &c::movement::auto_strafe_maximum_speed, 0.0f, 400.0f, ("%.1f"));
            }
            ImGui::Checkbox("auto edge jump", &c::movement::edge_jump);
            if (c::movement::edge_jump)
            {
                ImGui::Keybind("##edge jump key", &c::movement::edge_jump_key, &c::movement::edge_jump_key_s);
            }
            ImGui::Checkbox("auto long jump", &c::movement::long_jump);
            if (c::movement::long_jump)
            {
                ImGui::Keybind("##long jump key", &c::movement::long_jump_key, &c::movement::long_jump_key);
            }
            ImGui::Checkbox("auto mini jump", &c::movement::mini_jump);
            if (c::movement::mini_jump)
            {
                ImGui::Keybind("##mini jump key", &c::movement::mini_jump_key, &c::movement::mini_jump_key_s);
            }
            ImGui::Checkbox("adaptive key cancelling", &c::movement::adaptive_key_cancelling);
            if (c::movement::adaptive_key_cancelling)
            {
                ImGui::MultiCombo("##unpress keys", key_back, c::movement::lj_null, 4);
                ImGui::Text("while");
                ImGui::MultiCombo("##adaptive key for", key_back_for, c::movement::adaptive_key_for, 4);
            }
            ImGui::Checkbox("adaptive edge jump", &c::movement::adaptive_edge_jump);
            if (c::movement::adaptive_edge_jump)
            {
                ImGui::Text("while");
                ImGui::MultiCombo("##adaptive edge jump for", edge_jump_for, c::movement::adaptive_edge_jump_for, 3);
            }
            ImGui::Checkbox("auto jump bug", &c::movement::jump_bug);
            if (c::movement::jump_bug)
            {
                ImGui::Keybind("##jump bug key", &c::movement::jump_bug_key, &c::movement::jump_bug_key_s);
            }
            ImGui::Checkbox("auto edge bug (wip)", &c::movement::edge_bug);
            if (c::movement::edge_bug)
            {
                ImGui::Keybind("##edge bug key", &c::movement::edge_bug_key, &c::movement::edge_bug_key_s);
                ImGui::Checkbox("asisst to edge", &c::movement::edge_bug_asisst);
                ImGui::SliderFloat("time for prediction", &c::movement::edge_bug_time, 0.0f, 2.0f, ("%.2fs"));
            }
            ImGui::Checkbox("auto ladder edge jump", &c::movement::ladder_edge_jump);
            if (c::movement::ladder_edge_jump)
            {
                ImGui::Keybind("##ladder edge jump key", &c::movement::ladder_edge_jump_key, &c::movement::ladder_edge_jump_key_s);
            }           
            ImGui::Checkbox("auto align (wip)", &c::movement::auto_align);
            ImGui::Checkbox("auto pixel surf (wip)", &c::movement::pixel_surf);
            if (c::movement::pixel_surf)
            {
                ImGui::Keybind("##pixel surf key", &c::movement::pixel_surf_key, &c::movement::pixel_surf_key_s);
                ImGui::Checkbox("free look on surf", &c::movement::freelook_surf);
                ImGui::Checkbox("fix pixel surf", &c::movement::pixel_surf_fix);
            }    
            ImGui::Checkbox("auto stamina hop", &c::movement::delay_hop);
            if (c::movement::delay_hop)
            {
                ImGui::Keybind("##delay hop key", &c::movement::delay_hop_key, &c::movement::delay_hop_key_s);
                ImGui::SliderInt("delay tick", &c::movement::dh_tick, 1, 16);
            }
            ImGui::Checkbox("nulls", &c::movement::null_strafing);
            ImGui::Checkbox("strafe optimizer", &c::movement::strafe_optimizer);
            if (c::movement::strafe_optimizer)
            {
                ImGui::Keybind("##strafe optimizer key", &c::movement::strafe_optimizer_key, &c::movement::strafe_optimizer_key_s);
                ImGui::SliderFloat("minimum speed##1", &c::movement::strafe_optimizer_minimum_speed, 0.0f, 30.0f, ("%.1f"));
                ImGui::SliderFloat("maximum speed##1", &c::movement::strafe_optimizer_maximum_speed, 0.0f, 400.0f, ("%.1f"));
                ImGui::SliderFloat("gain", &c::movement::strafe_optimizer_gain, 0.0f, 15.0f, ("%.1f"));
            }
            ImGui::Checkbox("mouse strafe limiter", &c::movement::mouse_strafe_limiter);
            if (c::movement::mouse_strafe_limiter)
            {
                ImGui::Keybind("##mouse strafe limiter key", &c::movement::mouse_strafe_limiter_key, &c::movement::mouse_strafe_limiter_key_s);
                ImGui::Checkbox("change yaw/pitch", &c::movement::mouse_strafe_limiter_affect_pitch);
                ImGui::SliderFloat("strafe limit", &c::movement::mouse_strafe_limiter_value, 0.0f, 100.0f, ("%.1f"));
            }
            ImGui::Checkbox("infinity duck", &c::movement::fastduck);

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();

        ImGui::BeginChild(("misc.indicators"), ImVec2{ }, true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted(("scene"));
                ImGui::EndMenuBar();
            }

            static const ctab misc_tabs[] =
            {
                ctab{ "indicators", []()
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));

                ImGui::Checkbox("velocity", &c::movement::indicators::velocity::enable);
                if (c::movement::indicators::velocity::enable)
                {
                    if (c::movement::indicators::velocity::custom_color)
                    {
                        ImGui::Text("-velocity");
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##velocity color 1", c::movement::indicators::velocity::color_1, w_alpha);
                        ImGui::Text("+velocity");
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##velocity color 2", c::movement::indicators::velocity::color_2, w_alpha);
                    }
                    else
                    {
                        ImGui::Text("velocity positive");
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##velocity color 3",  c::movement::indicators::velocity::color_3, no_alpha);
                        ImGui::Text("velocity neutral");
                        ImGui::SameLine();     
                        ImGui::ColorEdit4("##velocity color 4",  c::movement::indicators::velocity::color_4, no_alpha);
                        ImGui::Text("velocity negative");
                        ImGui::SameLine();            
                        ImGui::ColorEdit4("##velocity color 5",  c::movement::indicators::velocity::color_5, no_alpha);
                    }

                    ImGui::Checkbox("take off##velocity", &c::movement::indicators::velocity::takeoff);
                    ImGui::Checkbox("disable takeoff on ground##velocity", &c::movement::indicators::velocity::disable_takeoff_on_ground);
                    ImGui::Checkbox("custom color##velocity", &c::movement::indicators::velocity::custom_color);
                }
                ImGui::Checkbox("stamina", &c::movement::indicators::stamina::enable);
                if (c::movement::indicators::stamina::enable)
                {
                    if (c::movement::indicators::stamina::custom_color)
                    {
                        ImGui::Text("-stamina");
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##stamina color 1", c::movement::indicators::stamina::color_1, w_alpha);
                        ImGui::Text("+stamina");
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##stamina color 2", c::movement::indicators::stamina::color_2, w_alpha);
                    }
                    else
                    {
                        ImGui::Text("stamina color");
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##stamina color", c::movement::indicators::stamina::color, no_alpha);
                    }

                    ImGui::Checkbox("take off##stamina", &c::movement::indicators::stamina::takeoff);
                    ImGui::Checkbox("disable takeoff on ground##stamina", &c::movement::indicators::stamina::disable_takeoff_on_ground);
                    ImGui::Checkbox("custom color##stamina", &c::movement::indicators::stamina::custom_color);
                }
                ImGui::Checkbox("key strokes", &c::movement::indicators::keys::enable);
                if (c::movement::indicators::keys::enable)
                {
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##keys color", c::movement::indicators::keys::color, no_alpha);
                    ImGui::Checkbox("background (wip)", &c::movement::indicators::keys::background);
                    ImGui::Checkbox("mouse direction", &c::movement::indicators::keys::mouse_direction);
                    ImGui::Checkbox("under lines", &c::movement::indicators::keys::under_line);
                    ImGui::Checkbox("null switch color", &c::movement::indicators::keys::nulling);
                    ImGui::SliderInt("keys position", &c::movement::indicators::keys::position, 0, 500, "%d");
                }
                if (c::movement::indicators::velocity::enable || c::movement::indicators::stamina::enable)
                {
                    ImGui::SliderInt("position", &c::movement::indicators::position, 0, 500, "%d");
                }

                ImGui::PopStyleVar();
            }},
                ctab{ "binds", []()
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));

                ImGui::Checkbox("enable", &c::movement::indicators::binds::enable);
                if (c::movement::indicators::binds::enable)
                {
                    ImGui::ColorEdit4("##indicator color", c::movement::indicators::binds::color, no_alpha);
                    ImGui::ColorEdit4("##indicator color 1", c::movement::indicators::binds::color_1, no_alpha);
                    ImGui::ColorEdit4("##indicator color 2", c::movement::indicators::binds::color_2, no_alpha);
                    ImGui::ColorEdit4("##indicator color 3", c::movement::indicators::binds::color_3, no_alpha);
                    ImGui::ColorEdit4("##indicator color 4", c::movement::indicators::binds::color_4, no_alpha);
                    ImGui::Text("indicators");
                    ImGui::MultiCombo("##indicators", indicators, c::movement::indicators::binds::list, 14);
                    ImGui::SliderFloat("speed animation", &c::movement::indicators::binds::speed, 0.0f, 50.0f, ("%.1f"));
                    ImGui::SliderInt("same line", &c::movement::indicators::binds::sameline, 0, 10, "%d");
                    ImGui::SliderInt("position", &c::movement::indicators::position, 0, 500, "%d");
                }

                ImGui::PopStyleVar();

            }},
                    ctab{ "graphs", []()
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));

                ImGui::Checkbox("velocity", &c::movement::indicators::graphs::velocity::enable);
                if (c::movement::indicators::graphs::velocity::enable)
                {
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##velocity graph", c::movement::indicators::graphs::velocity::color, no_alpha);
                    ImGui::Checkbox("draw velocity", &c::movement::indicators::graphs::velocity::draw_velocity);
                    ImGui::Checkbox("draw edgebug (wip)", &c::movement::indicators::graphs::velocity::draw_edgebug);
                    ImGui::Checkbox("draw jumpbug", &c::movement::indicators::graphs::velocity::draw_jumpbug);
                    ImGui::Checkbox("draw pixelsurf (wip)", &c::movement::indicators::graphs::velocity::draw_pixelsurf);
                }
                ImGui::Checkbox("stamina", &c::movement::indicators::graphs::stamina::enable);
                if (c::movement::indicators::graphs::stamina::enable)
                {
                    ImGui::SameLine();
                    ImGui::ColorEdit4("##stamina graph", c::movement::indicators::graphs::stamina::color, no_alpha);
                }
                if (c::movement::indicators::graphs::velocity::enable || c::movement::indicators::graphs::stamina::enable)
                {
                    ImGui::Checkbox("fade on graphs", &c::movement::indicators::graphs::fade);
                    ImGui::SliderFloat("size of graphs", &c::movement::indicators::graphs::size, 0.50f, 3.00f, ("%.2f"));
                    ImGui::SliderInt("position", &c::movement::indicators::position, 0, 500, "%d");
                }

                ImGui::PopStyleVar();

            }},
            };

            menu::render_tab("misc_tab", misc_tabs, 3U, &menu::indicators_tab, style.Colors[ImGuiCol_TabHovered]);
        }
        ImGui::EndChild();
    }
    ImGui::NextColumn(); {
        ImGui::BeginChild(("misc.config"), ImVec2(0, 190), true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted(("configuration"));
                ImGui::EndMenuBar();
            }

            static int config_index = -1;
            static char buffer[260];

            ImGui::Columns(2, ("#config"), false); {
                ImGui::PushItemWidth(-1);
               
                const std::vector<std::string>& configs_vector = c::configs;
                if (ImGui::ListBoxHeader(("##configs"), configs_vector.size(), 5)) {
                    for (std::size_t i = 0u; i < configs_vector.size(); ++i) {
                        std::string const& config_name = configs_vector[i].data();
                        if (ImGui::Selectable(config_name.c_str(), i == config_index)) {
                            config_index = i;
                        }
                    }
                    ImGui::ListBoxFooter();
                }
                ImGui::PopItemWidth();
            }
            ImGui::NextColumn(); {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, 0));
                ImGui::PushItemWidth(-1);
               /* if (ImGui::InputTextWithHint("##config.file", "create new...", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    c::create_file(buffer);

                    memset(buffer, 0, sizeof(buffer)); // clear the buffer
                }*/

                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("press enter to create a new config");
                }

                if (ImGui::Button("save", ImVec2(-1, 15)))
                {
                    ImGui::OpenPopup("config save popup");
                }

                if (ImGui::BeginPopup("config save popup"))
                {
                    ImGui::Text("are you sure you want to save selected config? ");

                    if (ImGui::Button("yes", ImVec2(-1, 15)))
                    {
                        //c::save(config_index);
                        features::notification::run("saved config", "#_print_saved", true, true, true);
                    }

                    if (ImGui::Button("no", ImVec2(-1, 15)))
                    {
                        features::notification::run("canceled save config", "#_print_canceled_save", true, true, true);
                    }

                    ImGui::EndPopup();
                }

                if (ImGui::Button("load", ImVec2(-1, 15)))
                {
                    //c::load(config_index);

                    features::notification::run("loaded config", "#_print_loaded", true, true, true);

                    if (interfaces::engine->is_in_game()) {
                        features::skins::forcing_update = true;
                    }

                    menu::load_font_index();
                    menu::fonts_initialized = true;
                }
                if (ImGui::Button("refresh", ImVec2(-1, 15))) {
                    //c::update_configs();

                    features::notification::run("refreshed config list", "#_print_refreshed", true, true, true);
                }

                ImGui::PopItemWidth();
                ImGui::PopStyleVar();
            }

            ImGui::Columns(1);



            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, 0));

            ImGui::Separator();

            ImGui::Text(("menu accent"));
            ImGui::SameLine( );
            ImGui::ColorEdit4(("##main_theme"), menu::menu_accent, no_alpha);
            ImGui::Text("menu key");
            ImGui::Keybind(("##menu key"), &c::misc::menu_key);

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();

        ImGui::BeginChild(("misc.main"), ImVec2{ }, true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted(("misc"));
                ImGui::EndMenuBar();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, 0));
			
            ImGui::Text("detection print in chat while");
            ImGui::MultiCombo("##detection chat", detect_chat_for, c::movement::detection_printf, 3);

            ImGui::Separator();

            ImGui::Checkbox("clan tag", &c::misc::misc_clantag_spammer);
            if (c::misc::misc_clantag_spammer) {
                ImGui::Text("type");
                ImGui::Combo("##cltype", &c::misc::misc_clantag_type, "static\0animated custom\0synchronized\0");
                if (c::misc::misc_clantag_type == 1) {
                    ImGui::Checkbox("reverse rolling direction", &c::misc::misc_clantag_rotation);
                    ImGui::SliderFloat("speed", &c::misc::misc_clantag_speed, 0.1f, 2.0f, ("%.2f"));
                    ImGui::Text("custom text");
                    ImGui::PushItemWidth(200.f);
                    ImGui::InputText("##clantagtext", c::misc::misc_clantag_text, IM_ARRAYSIZE(c::misc::misc_clantag_text));
                }
            }

            ImGui::Separator();

            ImGui::Checkbox("hit chams (wip)", &c::misc::misc_hitchams);
            if (c::misc::misc_hitchams)
            {
                ImGui::SameLine();
                ImGui::ColorEdit4("##hit chams color", c::misc::misc_hitchams_clr, w_alpha);
            }
            ImGui::Checkbox("hit marker", &c::misc::misc_hitmarker);
            ImGui::Checkbox("hit sound", &c::misc::misc_hitmarker_sound);
            if (c::misc::misc_hitmarker_sound)
			{
                ImGui::Combo("##sound_type", &c::misc::misc_hitmarker_sound_type, "arena_switch_press_02\0button22\0money_collect_01\0beep07");
            }
			
            static const char* hitinfo[3] = { "chat", "screen", "console" };

            ImGui::Text("hit info");
            ImGui::MultiCombo(("##hitinfo"), hitinfo, c::misc::misc_hit_info, 3);

            ImGui::Separator();

            ImGui::Checkbox("offline practice tool", &c::misc::practice);
            if (c::misc::practice)
			{
                ImGui::Text("save position");
                ImGui::Keybind(("save pos"), &c::misc::savepos, &c::misc::savepos_s);
                ImGui::Text("teleport to position");
                ImGui::Keybind(("load pos"), &c::misc::loadpos, &c::misc::loadpos_s);
            }
            ImGui::Checkbox("enable sv cheats", &c::misc::sv_cheats::enable);
            ImGui::Checkbox("mouse fix delta", &c::misc::mousefix);

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
    }
    ImGui::Columns(1);
}

void font() {
    ImGuiStyle& style = ImGui::GetStyle();

    ImGui::Columns(1, nullptr, false); {
        ImGui::BeginChild(("fonts.main"), ImVec2{ }, true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted(("fonts"));
                ImGui::EndMenuBar();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, 0));

            static ImGuiTextFilter filter;
            menu::load_font_index();
            ImGui::Text("selected font");
            ImGui::Combo("##font_part_selector", &menu::font_tab, fnt_tab, IM_ARRAYSIZE(fnt_tab));

            ImGui::Text("font list");

            if (menu::font_tab == 0) {
                ImGui::ListBoxHeader("##font_list_indicators", ImVec2(-1, 190)); {
                    for (int i = 0; i < menu::fonts.size(); i++) {

                        std::string fonts = menu::fonts[i];

                        if (filter.PassFilter(fonts.c_str())) {
                            if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_main_indi)) {
                                fonts::selected_font_index_main_indi = i;
                            }
                        }
                    }
                    ImGui::ListBoxFooter();
                }

                if (fonts::selected_font_index_main_indi >= 0) {
                    if (menu::fonts[fonts::selected_font_index_main_indi] == "default") {
                        fonts::font_directory_indicator = "C:/windows/fonts/tahomabd.ttf";
                    }
                    else {
                        fonts::font_directory_indicator = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_main_indi];
                    }
                    c::fonts::indi_font = fonts::selected_font_index_main_indi;
                }

                ImGui::Text("search font");
                filter.Draw("##filter_skin");
                ImGui::SliderInt("font size##indicators", &c::fonts::indi_size, 1, 50);
                ImGui::Text("font flags");
                ImGui::MultiCombo("##indicators_font_flags", font_flags, c::fonts::indi_font_flag, 11);
            }
            else if (menu::font_tab == 1) {
                ImGui::ListBoxHeader("##font_list_sub_indicators", ImVec2(-1, 190)); {
                    for (int i = 0; i < menu::fonts.size(); i++) {

                        std::string fonts = menu::fonts[i];

                        if (filter.PassFilter(fonts.c_str())) {
                            if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_sub_indi)) {
                                fonts::selected_font_index_sub_indi = i;
                            }
                        }
                    }
                    ImGui::ListBoxFooter();
                }

                if (fonts::selected_font_index_sub_indi >= 0) {
                    if (menu::fonts[fonts::selected_font_index_sub_indi] == "default") {
                        fonts::font_directory_sub_indicator = "C:/windows/fonts/tahoma.ttf";
                    }
                    else {
                        fonts::font_directory_sub_indicator = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_sub_indi];
                    }
                    c::fonts::sub_indi_font = fonts::selected_font_index_sub_indi;
                }

                ImGui::Text("search font");
                filter.Draw("##filter_skin");
                ImGui::SliderInt("font size##sub_indicators", &c::fonts::sub_indi_size, 1, 50);
                ImGui::Text("font flags");
                ImGui::MultiCombo("##sub_indicators_font_flags", font_flags, c::fonts::sub_indi_font_flag, 11);
            }
            else if (menu::font_tab == 2) {
                ImGui::ListBoxHeader("##font_list_esp", ImVec2(-1, 190)); {
                    for (int i = 0; i < menu::fonts.size(); i++) {

                        std::string fonts = menu::fonts[i];

                        if (filter.PassFilter(fonts.c_str())) {
                            if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_main_esp)) {
                                fonts::selected_font_index_main_esp = i;
                            }
                        }
                    }
                    ImGui::ListBoxFooter();
                }

                if (fonts::selected_font_index_main_esp >= 0) {
                    if (menu::fonts[fonts::selected_font_index_main_esp] == "default") {
                        fonts::font_directory_esp = "C:/windows/fonts/tahoma.ttf";
                    }
                    else {
                        fonts::font_directory_esp = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_main_esp];
                    }
                    c::fonts::esp_font = fonts::selected_font_index_main_esp;
                }

                ImGui::Text("search font");
                filter.Draw("##filter_skin");
                ImGui::SliderInt("font size##esp", &c::fonts::esp_size, 1, 50);
                ImGui::Text("font flags");
                ImGui::MultiCombo("##esp_font_flags", font_flags, c::fonts::esp_flag, 11);
            }
            else if (menu::font_tab == 3) {
                ImGui::ListBoxHeader("##font_list_sub_esp", ImVec2(-1, 190)); {
                    for (int i = 0; i < menu::fonts.size(); i++) {

                        std::string fonts = menu::fonts[i];

                        if (filter.PassFilter(fonts.c_str())) {
                            if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_sub_esp)) {
                                fonts::selected_font_index_sub_esp = i;
                            }
                        }
                    }
                    ImGui::ListBoxFooter();
                }

                if (fonts::selected_font_index_sub_esp >= 0) {
                    if (menu::fonts[fonts::selected_font_index_sub_esp] == "default") {
                        fonts::font_directory_sub_esp = "C:/windows/fonts/tahoma.ttf";
                    }
                    else {
                        fonts::font_directory_sub_esp = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_sub_esp];
                    }
                    c::fonts::esp_sub_font = fonts::selected_font_index_sub_esp;
                }

                ImGui::Text("search font");
                filter.Draw("##filter_skin");
                ImGui::SliderInt("font size##sub_esp", &c::fonts::esp_sub_size, 1, 50);
                ImGui::Text("font flags");
                ImGui::MultiCombo("##sub_esp_font_flags", font_flags, c::fonts::esp_sub_flag, 11);
            }
            else if (menu::font_tab == 4) {
                ImGui::ListBoxHeader("##font_list_scene", ImVec2(-1, 190)); {
                    for (int i = 0; i < menu::fonts.size(); i++) {

                        std::string fonts = menu::fonts[i];

                        if (filter.PassFilter(fonts.c_str())) {
                            if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_main_scene)) {
                                fonts::selected_font_index_main_scene = i;
                            }
                        }
                    }
                    ImGui::ListBoxFooter();
                }

                if (fonts::selected_font_index_main_scene >= 0) {
                    if (menu::fonts[fonts::selected_font_index_main_scene] == "default") {
                        fonts::font_directory_scene = "C:/windows/fonts/tahomabd.ttf";
                    }
                    else {
                        fonts::font_directory_scene = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_main_scene];
                    }
                    c::fonts::scene_font = fonts::selected_font_index_main_scene;
                }

                ImGui::Text("search font");
                filter.Draw("##filter_skin");
                ImGui::SliderInt("font size##scene", &c::fonts::scene_size, 1, 50);
                ImGui::Text("font flags");
                ImGui::MultiCombo("##scene_font_flags", font_flags, c::fonts::scene_flag, 11);
            }
            else if (menu::font_tab == 5) {
                    ImGui::ListBoxHeader("##font_list_sub_scene", ImVec2(-1, 190)); {
                        for (int i = 0; i < menu::fonts.size(); i++) {

                            std::string fonts = menu::fonts[i];

                            if (filter.PassFilter(fonts.c_str())) {
                                if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_sub_scene)) {
                                    fonts::selected_font_index_sub_scene = i;
                                }
                            }
                        }
                        ImGui::ListBoxFooter();
                    }

                    if (fonts::selected_font_index_sub_scene >= 0) {
                        if (menu::fonts[fonts::selected_font_index_sub_scene] == "default") {
                            fonts::font_directory_sub_scene = "C:/windows/fonts/tahomabd.ttf";
                        }
                        else {
                            fonts::font_directory_sub_scene = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_sub_scene];
                        }
                        c::fonts::scene_sub_font = fonts::selected_font_index_sub_scene;
                    }

                    ImGui::Text("search font");
                    filter.Draw("##filter_skin");
                    ImGui::SliderInt("font size##sub_scene", &c::fonts::scene_sub_size, 1, 50);
                    ImGui::Text("font flags");
                    ImGui::MultiCombo("##sub_scene_font_flags", font_flags, c::fonts::scene_sub_flag, 11);
            }

            if (ImGui::Button("reset fonts"))
            {
                fonts::reset_fonts();

                features::notification::run("reseted fonts", "#_print_reseted", true, true, true);
            }
            ImGui::SameLine();
            if (ImGui::Button("reload fonts"))
            {
                menu::fonts_initialized = true;
                menu::load_font_index();

                features::notification::run("reloaded fonts", "#_print_reloaded", true, true, true);;
            }

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
    }
}

void skins() {
    ImGuiStyle& style = ImGui::GetStyle();

    ImGui::Columns(2, nullptr, false); {
        ImGui::BeginChild(("skins.item"), ImVec2{0, 190 }, true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted(("item"));
                ImGui::EndMenuBar();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, 0));


            ImGui::Text(("item"));
            ImGui::Combo("##item", &menu::skin_selection, "knife\0glove\0");

            switch (menu::skin_selection) {
            case 0:
                ImGui::Checkbox(("enabled##knife"), &c::skins::knife_changer_enable);
                ImGui::Combo(("##knifemodel"), &c::skins::knife_changer_model, "default\0bayonet\0m9\0karambit\0bowie\0butterfly\0falchion\0flip\0gut\0huntsman\0shaddow-daggers\0navaja\0stiletto\0talon\0ursus\0default ct\0default t\0skeleton\0");
                break;

            case 1:
                ImGui::Checkbox(("enabled##gloves"), &c::skins::gloves_endable);
                ImGui::Combo("##glovesmodel", &c::skins::gloves_model, "default\0blood\0sport\0slick\0leather\0moto\0speci\0hydra");
                break;
            }

            ImGui::PopStyleVar();
            ImGui::EndChild();
        }

        ImGui::BeginChild(("skins.models"), ImVec2{ }, true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted(("other"));
                ImGui::EndMenuBar();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));

            ImGui::Checkbox(("model changer##agent changer"), &c::skins::agent_changer);
            if (c::skins::agent_changer) {
                ImGui::Text(("terrorist"));
                ImGui::Combo(("##terrorist"), &c::skins::agent_t, menu::agents, IM_ARRAYSIZE(menu::agents));
                ImGui::Text(("counter terrorist"));
                ImGui::Combo(("##counter terrorist"), &c::skins::agent_ct, menu::agents, IM_ARRAYSIZE(menu::agents));
            }

            ImGui::PopStyleVar();
            ImGui::EndChild();
        }

    }
    ImGui::NextColumn(); {
        ImGui::BeginChild(("skins.skins"), ImVec2{ }, true, ImGuiWindowFlags_MenuBar); {
            if (ImGui::BeginMenuBar()) {
                ImGui::TextUnformatted(("skins"));
                ImGui::EndMenuBar();
            }


            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, 0));

            float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
            static char search_knife_skin[256] = "";
            static char search_glove_skin[256] = "";
            static ImGuiTextFilter filter;

            switch (menu::skin_selection) {
            case 0:
                ImGui::Text(("knife skin"));
                ImGui::ListBoxHeader("##knifeskin", ImVec2(-1, 200)); {
                    for (int i = 0; i < features::skins::parser_skins.size(); i++) {
                        bool is_selected = (c::skins::knife_changer_vector_paint_kit == i);

                        std::string name = features::skins::parser_skins[i].name;

                        if (filter.PassFilter(name.c_str())) {

                            if (name == features::skins::parser_skins[i].name)
                                ImGui::PushID(std::hash<std::string>{}(features::skins::parser_skins[i].name)* i);

                                if (ImGui::Selectable(features::skins::parser_skins[i].name.c_str(), &is_selected)) {
                                    c::skins::knife_changer_vector_paint_kit = i;
                                    c::skins::knife_changer_paint_kit = features::skins::parser_skins[c::skins::knife_changer_vector_paint_kit].id;
                                }

                            if (name == features::skins::parser_skins[i].name)
                                ImGui::PopID();

                            name = features::skins::parser_skins[i].name;
                        }

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::ListBoxFooter();
                }

                ImGui::Text(("search skin"));
                filter.Draw("##filter_skin");

                ImGui::Text(("wear"));
                ImGui::Combo(("##knifecondition"), &c::skins::knife_changer_wear, "factory-new\0minimal-wear\0field-tested\0well-worn\0battle-scarred");

                ImGui::Checkbox(("custom color"), &c::skins::skin_custom_clr);
                if (c::skins::skin_custom_clr) {
                    ImGui::SameLine(group_w - 20);
                    ImGui::ColorEdit4(("##modulate1"), c::skins::skin_modulation1, no_alpha);
                    ImGui::SameLine(group_w - 40);
                    ImGui::ColorEdit4(("##modulate2"), c::skins::skin_modulation2, no_alpha);
                    ImGui::SameLine(group_w - 60);
                    ImGui::ColorEdit4(("##modulate3"), c::skins::skin_modulation3, no_alpha);
                    ImGui::SameLine(group_w - 80);
                    ImGui::ColorEdit4(("##modulate4"), c::skins::skin_modulation4, no_alpha);
                }
                break;
            case 1:
                ImGui::Text(("glove skin"));
                ImGui::ListBoxHeader("##glovesskin", ImVec2(-1, 200)); {
                    for (int i = 0; i < features::skins::parser_gloves.size(); i++) {
                        bool is_selected = (c::skins::gloves_skin == i);

                        std::string name = features::skins::parser_gloves[i].name;

                        if (filter.PassFilter(name.c_str())) {

                            if (name == features::skins::parser_gloves[i].name)
                                ImGui::PushID(std::hash<std::string>{}(features::skins::parser_gloves[i].name)* i);

                            if (ImGui::Selectable(features::skins::parser_gloves[i].name.c_str(), &is_selected)) {
                                c::skins::gloves_skin = i;
                                c::skins::gloves_skin_id = features::skins::parser_gloves[c::skins::gloves_skin].id;
                            }

                            if (name == features::skins::parser_gloves[i].name)
                                ImGui::PopID();

                            name = features::skins::parser_gloves[i].name;
                        }

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::ListBoxFooter();
                }


                ImGui::BeginListBox("##glovesskin", ImVec2(-1, 200)); {
                    for (int i = 0; i < features::skins::parser_gloves.size(); i++) {
                        std::string name = features::skins::parser_gloves[i].name;

                        if (filter.PassFilter(name.c_str())) {

                            std::string label = features::skins::parser_gloves[i].name + "##" + std::to_string(i);

                            if (ImGui::Selectable(label.c_str(), c::skins::gloves_skin == i)) {
                                c::skins::gloves_skin = i;
                                c::skins::gloves_skin_id = features::skins::parser_gloves[c::skins::gloves_skin].id;
                            }
                        }
                    }
                    ImGui::EndListBox();
                }

                ImGui::Text(("search skin"));
                filter.Draw("##filter_skin_gloves");
                ImGui::Text(("wear"));
                ImGui::Combo("##gloveswear", &c::skins::gloves_wear, "factory-new\0minimal-wear\0field-tested\0well-worn\0battle-scarred");

                break;
            }

            if (ImGui::Button("update"))
            {
                features::notification::run("force updated", "#_print_updated_hud", true, true, true);

                if (interfaces::engine->is_in_game())
                {
                    features::skins::forcing_update = true;
                }
            }

            ImGui::PopStyleVar();
            ImGui::EndChild();
        }
    }
    ImGui::Columns(1);
}

void aimbot_page()
{
    switch (menu::subtab)
    {
        case 0: legitbot(c::aimbot::general); break;
        case 1: legitbot(c::aimbot::pistols, true); break;
        case 2: legitbot(c::aimbot::heavy_pistols, true); break;
        case 3: legitbot(c::aimbot::smg, true); break;
        case 4: legitbot(c::aimbot::shotguns, true); break;
        case 5: legitbot(c::aimbot::rifles, true); break;
        case 6: legitbot(c::aimbot::snipers, true); break;
    }
}
void menu::render() {
    if (!menu::open) 
        return;

    auto& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();
    auto flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse;
    const ImVec2 s = io.DisplaySize;

    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_SeparatorActive, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_ResizeGrip, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_DragDropTarget, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));

    ImGui::SetNextWindowPos(ImVec2(s.x * 0.5f, s.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(550, 427), ImGuiCond_Always);
    ImGui::Begin(("menu"), &menu::open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar); {
        menu_pos = ImGui::GetCursorScreenPos();
        const float width = ImGui::GetWindowWidth();
        menu_size = ImGui::GetWindowSize();
        ImDrawList* d = ImGui::GetWindowDrawList();

        // push clip so separator will be drawn at maximal window bounds
        ImGui::PushClipRect(ImVec2(menu_pos.x - 8.f, menu_pos.y - 8.f), ImVec2(menu_pos.x + width - 8.f, menu_pos.y - 6.f), false);

        // restore cliprect
        ImGui::PopClipRect();

        // add tabs
        static const ctab tabs[] = 
        {
            ctab{ "legit", &aimbot_page },
            ctab{ "visuals", &visuals },
            ctab{ "misc", &miscellaneous },
            ctab{ "fonts", &font },
            ctab{ "skins", &skins },
        };

        render_tab("main_tabs", tabs, 5U, &main_tab, style.Colors[ImGuiCol_TabHovered]);

        ImGui::End();
    }
    ImGui::PopStyleColor(15);
}