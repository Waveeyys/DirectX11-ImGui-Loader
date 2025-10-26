#include "main.h"
#include "settings.h"
#include "font_awesome.h"
#include "fonts.h"
#include "images.h"
#include "particles.h"

#include <algorithm>
#include <cctype>   // for isalnum, islower, etc.
#include <cstring>  // for strchr, strlen
#include <Windows.h>
#include <shlobj.h>
#include <fstream>
#include <random>
#include <chrono>

Framework::CUSTOM c;
Framework::GUI g;

// Global variables
std::string show_error_message = "";
static char redeem_code[50] = ""; // ADD THIS LINE

void Framework::ui::BeforeLoop()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true; icons_config.OversampleH = 3; icons_config.OversampleV = 3;

    if (images::fivem == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, fivem_p, sizeof(fivem_p), nullptr, nullptr, &images::fivem, 0);
	if (images::qh == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, qh_p, sizeof(qh_p), nullptr, nullptr, &images::qh, 0);
    if (images::rust == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, rust_p, sizeof(rust_p), nullptr, nullptr, &images::rust, 0);
    if (images::fn == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, fn_p, sizeof(fn_p), nullptr, nullptr, &images::fn, 0);
    if (images::eft == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, EFT_p, sizeof(EFT_p), nullptr, nullptr, &images::eft, 0);
    if (images::mw == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, mw_p, sizeof(mw_p), nullptr, nullptr, &images::mw, 0);
    if (images::circle == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, circle_p, sizeof(circle_p), nullptr, nullptr, &images::circle, 0);

    if (fonts::Inter_Regular == nullptr) fonts::Inter_Regular = io.Fonts->AddFontFromMemoryTTF(inter_regular_p, sizeof(inter_regular_p), 16);
    if (fonts::Sansation_Light == nullptr) fonts::Sansation_Light = io.Fonts->AddFontFromMemoryTTF(sansation_light_p, sizeof(sansation_light_p), 13);
    if (fonts::Sansation_Regular == nullptr) fonts::Sansation_Regular = io.Fonts->AddFontFromMemoryTTF(sansation_regular_p, sizeof(sansation_regular_p), 25);
    if (fonts::Sansation_Bold == nullptr) fonts::Sansation_Bold = io.Fonts->AddFontFromMemoryTTF(sansation_bold_p, sizeof(sansation_bold_p), 28);
    if (fonts::FontAwesome == nullptr) fonts::FontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);

    InitializeParticles();
    g.Blur(hwnd);
}

bool show_error = false;

bool is_valid_key(const char* key) {
    // Check if key is empty
    if (strlen(key) == 0) {
        show_error_message = "Product key cannot be empty";
        return false;
    }

    // Check for spaces
    if (strchr(key, ' ') != nullptr) {
        show_error_message = "Product key cannot contain spaces";
        return false;
    }

    // Check length is exactly 24 characters
    if (strlen(key) != 24) {
        show_error_message = "Product key must be exactly 24 characters";
        return false;
    }

    // Check if it starts with "GIA"
    if (strncmp(key, "GIA", 3) != 0) {
        show_error_message = "Product key must start with 'GIA'";
        return false;
    }

    // Check if it contains only valid characters (alphanumeric)
    for (int i = 0; key[i] != '\0'; i++) {
        if (!isalnum(key[i])) {
            show_error_message = "Product key can only contain letters and numbers";
            return false;
        }
    }

    // If all checks pass, the key format is valid
    show_error_message = "";
    return true;
}

// Function to generate random GIA keys
std::string generate_random_key(const char* product_type) {
    std::string key = "GIA5M";

    // Add product identifier
    if (strcmp(product_type, "QH") == 0) {
        key += "QH";
    }
    else if (strcmp(product_type, "RT") == 0) {
        key += "RT";
    }
    else if (strcmp(product_type, "FN") == 0) {
        key += "FN";
    }
    else if (strcmp(product_type, "EFT") == 0) {
        key += "EFT";
    }
    else if (strcmp(product_type, "ALL") == 0) {
        key += "ALL";
    }
    else {
        key += "QH"; // Default to QuantomHub
    }

    // Generate random characters for the remaining length
    std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, chars.size() - 1);

    // We need 24 total characters, so calculate remaining needed
    int remaining_chars = 24 - key.length();
    for (int i = 0; i < remaining_chars; i++) {
        key += chars[distribution(generator)];
    }

    return key;
}

// Update the mapping function to generate random keys
const char* map_redemption_to_key(const char* redemption_code) {
    static std::string generated_key; // Static to persist the key

    // Convert to uppercase for case-insensitive comparison
    std::string code = redemption_code;
    std::transform(code.begin(), code.end(), code.begin(), ::toupper);

    // Map redemption codes to product types and generate random keys
    if (code.find("QUANTOM") != std::string::npos || code.find("QH") != std::string::npos) {
        generated_key = generate_random_key("QH");
    }
    else if (code.find("RUST") != std::string::npos || code.find("RT") != std::string::npos) {
        generated_key = generate_random_key("RT");
    }
    else if (code.find("FORTNITE") != std::string::npos || code.find("FN") != std::string::npos) {
        generated_key = generate_random_key("FN");
    }
    else if (code.find("EFT") != std::string::npos) {
        generated_key = generate_random_key("EFT");
    }
    else if (code.find("ALL") != std::string::npos || code.find("FULL") != std::string::npos) {
        generated_key = generate_random_key("ALL");
    }
    else {
        // Default to QuantomHub if no specific code matched
        generated_key = generate_random_key("QH");
    }

    return generated_key.c_str();
}

bool save_key_to_file(const char* key) {
    char downloadsPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, downloadsPath))) {
        // Append Downloads folder to the path
        strcat_s(downloadsPath, "\\Downloads\\");

        // Create the full file path
        char filePath[MAX_PATH];
        strcpy_s(filePath, downloadsPath);
        strcat_s(filePath, "GIA_Product_Key.txt");

        // Create and write to the file
        std::ofstream keyFile(filePath);
        if (keyFile.is_open()) {
            keyFile << "GIA Product Key" << std::endl;
            keyFile << "===============" << std::endl;
            keyFile << "Key: " << key << std::endl;
            keyFile << "Date Activated: " << __DATE__ << " " << __TIME__ << std::endl;
            keyFile << "Products Unlocked:" << std::endl;

            // Add which products are unlocked
            auto& user_features = get_user_features();
            for (size_t i = 0; i < user_features.size(); i++) {
                if (user_features[i].unlocked) {
                    keyFile << "  - " << games[user_features[i].game_index].name << std::endl;
                }
            }

            keyFile.close();
            return true;
        }
    }
    return false;
}

void Framework::ui::Render()
{
    g.begin("Main Window", window::size_max);
    {
        UpdateParticles(0.03);
        RenderParticles();

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& s = ImGui::GetStyle();
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        float time = io.DeltaTime * 5;
        static float timer = io.DeltaTime;
        static int selected_build = 0; // 0=Stable, 1=Beta, 2=Nightly

        // Header with title and close button
        c.IconButton("###Close", ICON_FA_XMARK, window->Size.x - 35, 18, 1);
        c.Text(15, 20, fonts::Sansation_Regular, 20, "Download Loader", colors::White);

        // Subtitle
        c.Text(15, 50, fonts::Inter_Regular, 14, "Enter your product key to download the loader", colors::lwhite);

        s.Alpha = alphaColor;

        float total_width = window->Size.x - 100;
        float center_x = (window->Size.x - total_width) / 2;

        if (TAB == 0)
        {
            // Expanded animation time
            alphaColor = std::clamp(alphaColor + (1.f * ImGui::GetIO().DeltaTime * 0.5f), 0.0f, 1.f);

            // Only show product key section if not in redeem mode
            if (subtab != 6)
            {
                // Product key input - centered
                float input_y = 180;

                c.InputText("KEY", "PRODUCT KEY", center_x, input_y, total_width, License, 50, 0);

                // Error message
                if (!show_error_message.empty()) {
                    c.Text(center_x, input_y + 50, fonts::Inter_Regular, 14, show_error_message.c_str(), colors::Red);
                }

                // Download button
                float button_y = input_y + 80;
                if (c.Button("Download", "DOWNLOAD", center_x, button_y, total_width, 45)) {
                    // Validate key format
                    if (is_valid_key(License)) {
                        // Unlock features based on the key
                        unlock_features_based_on_key(License);

                        // If format is valid, proceed to next page
                        TAB = 1;
                        alphaColor = 0;
                        show_error_message = "";
                    }
                }

                // ADD REDEEM KEY SECTION
                c.Text(center_x, button_y + 70, fonts::Inter_Regular, 14, "Don't have a key?", colors::lwhite);

                if (c.Button("RedeemKey", "REDEEM KEY", center_x, button_y + 95, total_width, 35)) {
                    // Switch to redeem mode within TAB 0
                    subtab = 6;
                    alphaColor = 0;
                }
            }
            else
            {
                // REDEEM SECTION (shown instead of product key section)
                // Back button - moved lower
                if (c.Button("Back", "Back", center_x, 250, total_width, 35)) {
                    subtab = 0;
                    alphaColor = 0;
                }

                // Redeem key title - moved lower
                c.Text((window->Size.x - ImGui::CalcTextSize("Redeem Product Key").x) / 2, 120,
                    fonts::Sansation_Regular, 20, "Redeem Product Key", colors::White);

                // Instructions - moved lower
                c.Text(center_x, 170, fonts::Inter_Regular, 14,
                    "Enter your redemption code below:", colors::lwhite);

                // Redemption code input - moved lower
                c.InputText("REDEEM", "REDEMPTION CODE", center_x, 210, total_width, redeem_code, 50, 0);

                // Redeem button - moved lower
                if (c.Button("Redeem", "REDEEM CODE", center_x, 300, total_width, 45)) {
                    // Validate redemption code
                    if (strlen(redeem_code) > 0) {
                        // Map redemption code to a RANDOM product key
                        const char* activated_key = map_redemption_to_key(redeem_code);

                        // Process redemption - activate the random key
                        strcpy_s(License, sizeof(License), activated_key);

                        // Unlock features based on the activated key
                        unlock_features_based_on_key(License);

                        // Save the key to a file in Downloads folder
                        save_key_to_file(License);

                        // Clear the redemption code input
                        strcpy_s(redeem_code, sizeof(redeem_code), "");

                        subtab = 0;
                        show_error_message = "Key activated successfully! Saved to Downloads folder.";
                    }
                    else {
                        show_error_message = "Please enter a redemption code";
                    }
                }

                // Additional info - moved lower
                c.Text(center_x, 370, fonts::Inter_Regular, 12,
                    "Redemption codes can be purchased from our website", colors::Gray);
                c.Text(center_x, 390, fonts::Inter_Regular, 12,
                    "or obtained from authorized resellers.", colors::Gray);
            }
        }
        else if (TAB == 1)
        {
            if (timer <= 5) { timer += 0.5; return; }

            alphaColor = std::clamp(alphaColor + (1.f * ImGui::GetIO().DeltaTime * 1.f), 0.0f, 1.f);

            window->DrawList->AddLine({ 165, 28 }, { 165, 42 }, ImGui::GetColorU32(colors::White));

            if (c.Tab("HOME", "Home", NULL, 180, 28, ImGui::CalcTextSize("Home").x, ImGui::CalcTextSize("Home").y, subtab == 0)) { subtab = 0; }

            if (subtab == 0)
            {
                SelectablesPos = ImLerp(SelectablesPos, 15.f, time);
                ButtonPos = ImLerp(ButtonPos, 245.f, time);
                ParametersPos = ImLerp(ParametersPos, window::size_max.x - 15, time);

                c.Text(SelectablesPos, 87, fonts::Inter_Regular, 16, "Select a Game", colors::White);
                c.Text(ParametersPos - 272, 87, fonts::Inter_Regular, 16, "Product Info", colors::White);

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                g.ChildBegin("Selectable items", SelectablesPos, 106, 315, 300);
                {
                    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 0.0f); // Hide scrollbar

                    float selectable_y = 15;
                    int visible_index = 0;

                    // Only show 5M Privacy Protector if unlocked
                    if (is_game_unlocked(0)) {
                        if (c.SelectableItem("5M Privacy Protector", images::fivem, 12.5, selectable_y, 290, 80, selected == 0)) {
                            selected = 0;
                        }
                        selectable_y += 95;
                        visible_index++;
                    }

                    // Only show Rust if unlocked
                    if (is_game_unlocked(1)) {
                        if (c.SelectableItem("Rust", images::rust, 12.5, selectable_y, 290, 80, selected == 1)) {
                            selected = 1;
                        }
                        selectable_y += 95;
                        visible_index++;
                    }

                    // Only show Fortnite if unlocked
                    if (is_game_unlocked(2)) {
                        if (c.SelectableItem("Fortnite", images::fn, 12.5, selectable_y, 290, 80, selected == 2)) {
                            selected = 2;
                        }
                        selectable_y += 95;
                        visible_index++;
                    }

                    if (is_game_unlocked(3)) {
                        if (c.SelectableItem("Escape from Tarkov", images::eft, 12.5, selectable_y, 290, 80, selected == 3)) {
                            selected = 3;
                        }
                        selectable_y += 95;
                        visible_index++;
                    }

                    if (is_game_unlocked(4)) {
                        if (c.SelectableItem("QuantomHub", images::qh, 12.5, selectable_y, 290, 80, selected == 4)) {
                            selected = 4;
                        }
                        selectable_y += 95;
                        visible_index++;
                    }

                    // If no games are unlocked (shouldn't happen with default), show message
                    if (visible_index == 0) {
                        c.Text(12.5, selectable_y, fonts::Inter_Regular, 14, "No products available for your license", colors::Red);
                    }

                    ImGui::PopStyleVar(); // Scrollbar style
                }
                g.ChildEnd();
                ImGui::PopStyleVar(); // Window padding

                // Updated Parameters section - Fully dynamic based on selected game
                g.ChildBegin("Parameters", ParametersPos - 272, 105, 275, 180);
                {
                    // Get current selected game info
                    GameInfo& currentGame = games[selected];

                    // Product name - dynamic
                    window->DrawList->AddText(fonts::Inter_Regular, 16,
                        ImVec2(ParametersPos - 272 + 15, 105 + 15),
                        ImGui::GetColorU32(colors::White),
                        currentGame.name);

                    // Expiry information - dynamic per game
                    window->DrawList->AddText(fonts::Inter_Regular, 14,
                        ImVec2(ParametersPos - 272 + 15, 105 + 40),
                        ImGui::GetColorU32(colors::MainColor),
                        currentGame.expiry_time);

                    // First separator
                    window->DrawList->AddLine(
                        ImVec2(ParametersPos - 272 + 15, 105 + 65),
                        ImVec2(ParametersPos - 272 + 260, 105 + 65),
                        ImGui::GetColorU32(colors::Gray)
                    );

                    // Status section - dynamic
                    window->DrawList->AddText(fonts::Inter_Regular, 14,
                        ImVec2(ParametersPos - 272 + 15, 105 + 75),
                        ImGui::GetColorU32(colors::lwhite),
                        "Status");
                    window->DrawList->AddText(fonts::Inter_Regular, 14,
                        ImVec2(ParametersPos - 272 + 15, 105 + 95),
                        ImGui::GetColorU32(currentGame.status ? colors::Green : colors::Red),
                        currentGame.status ? "Working / Undetected" : "Updating / Detected");

                    // Second separator
                    window->DrawList->AddLine(
                        ImVec2(ParametersPos - 272 + 15, 105 + 115),
                        ImVec2(ParametersPos - 272 + 260, 105 + 115),
                        ImGui::GetColorU32(colors::Gray)
                    );

                    // Last Updated section - dynamic
                    window->DrawList->AddText(fonts::Inter_Regular, 14,
                        ImVec2(ParametersPos - 272 + 15, 105 + 125),
                        ImGui::GetColorU32(colors::lwhite),
                        "Last Updated");
                    window->DrawList->AddText(fonts::Inter_Regular, 14,
                        ImVec2(ParametersPos - 272 + 15, 105 + 145),
                        ImGui::GetColorU32(colors::lwhite),
                        currentGame.last_updated);

                    // Third separator
                    window->DrawList->AddLine(
                        ImVec2(ParametersPos - 272 + 15, 105 + 165),
                        ImVec2(ParametersPos - 272 + 260, 105 + 165),
                        ImGui::GetColorU32(colors::Gray)
                    );

                    // Load Selected Products button
                    if (c.Button("LoadProducts", "Load Selected Products",
                        ParametersPos - 272 + 15, 105 + 175, 245, 35)) {
                        // This will now load the currently selected game
                        // You can use currentGame.name or selected index here
                        // Example: LoadGame(selected);
                    }
                }
                g.ChildEnd();

                // Updated button section
                if (c.Button("Spoof", "Spoof", 340, ButtonPos + 50, 270, 30)) {
                    subtab = 5;
                    alphaColor = 0;
                }
                if (c.Button("TEMPSpoof", "Temp Spoof", 340, ButtonPos + 87, 270, 30)) {
                    // You can use the selected game info here too
                    // For example: spoof specific to games[selected]
                }
            }

            if (subtab == 5)
            {
                alphaColor = std::clamp(alphaColor + (1.f * ImGui::GetIO().DeltaTime * 1.f), 0.0f, 1.f);

                c.Text((window::size_max.x - ImGui::CalcTextSize("Spoofing").x) / 2, (window::size_max.y - ImGui::CalcTextSize("Spoofing").y) / 2, fonts::Inter_Regular, 17, "Spoofing", colors::White);
                g.AddImageRotated(images::circle, (window::size_max.x - 20) / 2, (window::size_max.y - 35) * 0.6, 35, 35);

                timer += 0.1;
                if (timer > 100) { timer = 0; alphaColor = 0; ButtonPos = window::size_max.y + 5; SelectablesPos = -335; ParametersPos = window::size_max.x + 340; subtab = 0; }
            }

            // REDEEM SUBTAB IN TAB 1
            if (subtab == 6)
            {
                alphaColor = std::clamp(alphaColor + (1.f * ImGui::GetIO().DeltaTime * 1.f), 0.0f, 1.f);

                // Back button - moved lower
                if (c.Button("Back", "Back", 15, 80, 60, 25)) {
                    subtab = 0;
                    alphaColor = 0;
                }

                // Redeem key title - moved much lower
                c.Text((window->Size.x - ImGui::CalcTextSize("Redeem Product Key").x) / 2, 120,
                    fonts::Sansation_Regular, 20, "Redeem Product Key", colors::White);

                // Instructions - moved lower
                c.Text(center_x, 170, fonts::Inter_Regular, 14,
                    "Enter your redemption code below:", colors::lwhite);

                // Redemption code input - moved lower
                c.InputText("REDEEM", "REDEMPTION CODE", center_x, 210, total_width, redeem_code, 50, 0);

                // Redeem button - moved lower
                if (c.Button("Redeem", "REDEEM CODE", center_x, 270, total_width, 45)) {
                    // Validate redemption code
                    if (strlen(redeem_code) > 0) {
                        // Process redemption - activate a key
                        strcpy_s(License, sizeof(License), "GIA5MA1B2C3D4E5F6G7H8I9"); // Example activated key

                        // Save the key to a file in Downloads folder
                        save_key_to_file(License);

                        subtab = 0;
                        show_error_message = "Key activated successfully! Saved to Downloads folder.";
                    }
                    else {
                        show_error_message = "Please enter a redemption code";
                    }
                }

                // Additional info - moved lower
                c.Text(center_x, 340, fonts::Inter_Regular, 12,
                    "Redemption codes can be purchased from our website", colors::Gray);
                c.Text(center_x, 360, fonts::Inter_Regular, 12,
                    "or obtained from authorized resellers.", colors::Gray);
            }
        }

        g.end();
    }
}