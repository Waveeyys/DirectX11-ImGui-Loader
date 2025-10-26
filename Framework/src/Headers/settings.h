#pragma once
#include "main.h"
#include <vector>
#include <algorithm>

// Window and DirectX variables
inline ID3D11Device* g_pd3dDevice = nullptr;
inline ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
inline IDXGISwapChain* g_pSwapChain = nullptr;
inline UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
inline ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

const wchar_t project_name[] = L"GIA";

// UI state variables
inline int TAB = 0;
inline int subtab = 0;
inline char License[50] = ""; // Make sure this exists
inline float alphaColor = 0.f;
inline bool MAC = false;
inline bool Disk = false;
inline bool Clean = false;
inline bool Save = false;
inline float ButtonPos = 800 + 5; // window::size_max.y + 5
inline float SelectablesPos = -335;
inline float ParametersPos = 800 + 340; // window::size_max.x + 340

// Product info variables
inline char expiry_time[50] = "9998 days, 20 hours";
inline char last_updated[50] = "0 days, 2 hours ago";
inline bool product_status = true; // true = undetected, false = detected

// Game info structures
struct GameInfo {
    char name[50];
    char last_updated[50];
    char expiry_time[50];
    bool status; // true = undetected, false = detected
};

// Game info variables with unique expiry times for each game
inline GameInfo games[] = {
    {"5M Privacy Protector", "3 days, 18 hours ago", "Never Expires", true},
    {"Rust", "1 day, 5 hours ago", "3 days, 1 hours", false},
    {"Fortnite", "5 days, 2 hours ago", "76 days, 12 hours", false},
    {"Escape from Tarkov", "12 hours ago", "43 day, 0 hours", false},
    {"QuantomHub Trigger Finder", "2 days, 8 hours ago", "Never Expires", true}
};

inline int selected = 0; // Current selected game index

// Feature mapping structure
struct UserFeature {
    int game_index;
    bool unlocked;
};

// User session data
inline std::vector<UserFeature>& get_user_features() {
    static std::vector<UserFeature> instance;
    return instance;
}

inline void unlock_features_based_on_key(const char* key) {
    auto& user_features = get_user_features();
    user_features.clear();

    // Always unlock 5M Privacy Protector
    user_features.push_back({ 0, true });

    // Convert key to uppercase for case-insensitive comparison
    std::string key_upper = key;
    for (size_t i = 0; i < key_upper.length(); i++) {
        key_upper[i] = std::toupper(key_upper[i]);
    }

    // Check for product identifiers in the random key
    if (key_upper.find("QH") != std::string::npos) {
        user_features.push_back({ 4, true }); // QuantomHub
    }
    if (key_upper.find("RT") != std::string::npos) {
        user_features.push_back({ 1, true }); // Rust
    }
    if (key_upper.find("FN") != std::string::npos) {
        user_features.push_back({ 2, true }); // Fortnite
    }
    if (key_upper.find("EFT") != std::string::npos) {
        user_features.push_back({ 3, true }); // EFT
    }
    if (key_upper.find("ALL") != std::string::npos) {
        // Unlock all products
        user_features.push_back({ 1, true });
        user_features.push_back({ 2, true });
        user_features.push_back({ 3, true });
        user_features.push_back({ 4, true });
    }

    // Remove duplicates
    for (size_t i = 0; i < user_features.size(); i++) {
        for (size_t j = i + 1; j < user_features.size(); j++) {
            if (user_features[i].game_index == user_features[j].game_index) {
                user_features.erase(user_features.begin() + j);
                j--;
            }
        }
    }

    // If no additional products were unlocked, at least ensure 5M is there
    if (user_features.size() == 1) {
        user_features.push_back({ 0, true });
    }
}

inline bool is_game_unlocked(int game_index) {
    auto& user_features = get_user_features();
    for (size_t i = 0; i < user_features.size(); i++) {
        if (user_features[i].game_index == game_index && user_features[i].unlocked) {
            return true;
        }
    }
    return false;
}