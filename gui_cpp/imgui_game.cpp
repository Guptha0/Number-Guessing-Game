// ==============================================================================
// Tier 2: C++ High-Performance Immediate-Mode GUI (Dear ImGui + Hardware Timers)
// Demonstrating High-Resolution std::chrono, Immediate-Mode ImDrawList, and Screen-Shake
// ==============================================================================

#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>

// Standalone ImGui mock/wrapper definitions for architectural illustration
// In a full build, include <imgui.h>, <imgui_impl_glfw.h>, <imgui_impl_opengl3.h>
namespace ImGui {
    struct ImVec2 { float x, y; ImVec2(float _x = 0, float _y = 0) : x(_x), y(_y) {} };
    struct ImVec4 { float x, y, z, w; ImVec4(float _x=0, float _y=0, float _z=0, float _w=0) : x(_x),y(_y),z(_z),w(_w){} };
    typedef unsigned int ImU32;
    inline ImU32 ColorConvertFloat4ToU32(const ImVec4& in) {
        return ((ImU32)(in.w*255)<<24) | ((ImU32)(in.z*255)<<16) | ((ImU32)(in.y*255)<<8) | (ImU32)(in.x*255);
    }
}

enum class Mode { Classic, TimeAttack, Endurance };

struct GameState {
    Mode currentMode = Mode::TimeAttack;
    int minRange = 1;
    int maxRange = 100;
    int secretTarget = 0;
    int attemptsRemaining = 7;
    int maxAttempts = 7;
    int score = 0;
    int winStreak = 0;
    bool isRoundActive = true;
    float currentHeat = 0.0f; // [0.0 - 1.0]

    // High-Resolution Hardware Chronometer for Time Attack
    std::chrono::time_point<std::chrono::high_resolution_clock> roundStartTime;
    float timeLimitSeconds = 25.0f;
    float timeRemainingSeconds = 25.0f;

    // Trauma-based Screen Shake Engine (Squirrel Eiserloh model)
    float trauma = 0.0f; // [0.0, 1.0], decays linearly with dt
    ImGui::ImVec2 screenOffset = { 0.0f, 0.0f };

    std::vector<std::string> attemptHistory;
    std::mt19937 rng{ std::random_device{}() };

    void startNewRound() {
        if (currentMode == Mode::Endurance && winStreak > 0) {
            maxRange = static_cast<int>(100.0 * std::pow(1.5, winStreak));
            maxAttempts = 7 + (winStreak * 2);
        } else {
            minRange = 1;
            maxRange = 100;
            maxAttempts = 7;
        }

        std::uniform_int_distribution<int> dist(minRange, maxRange);
        secretTarget = dist(rng);
        attemptsRemaining = maxAttempts;
        isRoundActive = true;
        currentHeat = 0.0f;
        attemptHistory.clear();

        roundStartTime = std::chrono::high_resolution_clock::now();
        timeRemainingSeconds = timeLimitSeconds;
    }

    void addTrauma(float amount) {
        trauma = std::min(1.0f, trauma + amount);
    }

    void update(float deltaTime) {
        // High-Precision Hardware Timer Tick
        if (isRoundActive && currentMode == Mode::TimeAttack) {
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> elapsed = now - roundStartTime;
            timeRemainingSeconds = std::max(0.0f, timeLimitSeconds - elapsed.count());

            if (timeRemainingSeconds <= 0.0f) {
                isRoundActive = false;
                winStreak = 0;
                addTrauma(0.8f);
                attemptHistory.push_back(">> TIME EXPIRED! Defeat.");
            }
        }

        // Screen Shake Decay & Noise Offset Calculation
        if (trauma > 0.0f) {
            trauma = std::max(0.0f, trauma - (deltaTime * 1.8f)); // Linear decay
            float shakePower = trauma * trauma; // Quadratic falloff
            float time = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()).count()) * 0.001f;

            // Multi-frequency harmonic displacement
            screenOffset.x = 24.0f * shakePower * std::sin(time * 45.0f);
            screenOffset.y = 24.0f * shakePower * std::cos(time * 38.0f);
        } else {
            screenOffset = { 0.0f, 0.0f };
        }
    }

    void submitGuess(int guess) {
        if (!isRoundActive) return;

        attemptsRemaining--;
        int dist = std::abs(guess - secretTarget);
        int maxDist = std::max(secretTarget - minRange, maxRange - secretTarget);
        currentHeat = 1.0f - (static_cast<float>(dist) / static_cast<float>(maxDist));
        currentHeat = std::clamp(currentHeat, 0.0f, 1.0f);

        if (guess == secretTarget) {
            isRoundActive = false;
            winStreak++;
            int speedBonus = (currentMode == Mode::TimeAttack) ? static_cast<int>(timeRemainingSeconds * 50) : 0;
            int roundPoints = ((attemptsRemaining + 1) * 200 + speedBonus) * std::max(1, winStreak);
            score += roundPoints;
            attemptHistory.push_back(">> TARGET ACQUIRED! +" + std::to_string(roundPoints) + " pts");
        } else {
            addTrauma(0.45f); // Shake viewport on error
            std::string dir = (guess < secretTarget) ? "HIGHER ▲" : "LOWER ▼";
            attemptHistory.push_back("Guess " + std::to_string(guess) + " -> " + dir + " (" + 
                                     std::to_string(static_cast<int>(currentHeat * 100)) + "% Heat)");

            if (attemptsRemaining <= 0) {
                isRoundActive = false;
                winStreak = 0;
                addTrauma(0.9f);
                attemptHistory.push_back(">> DEFEAT! Target was: " + std::to_string(secretTarget));
            }
        }
    }
};

// ==============================================================================
// Immediate-Mode Render Pipeline (Executed 60/144 times per second)
// ==============================================================================
void RenderGameUI(GameState& state, float dt) {
    state.update(dt);

    // Apply viewport translation offset for screen-shake
    // ImGui::SetNextWindowPos(ImVec2(30.0f + state.screenOffset.x, 30.0f + state.screenOffset.y));

    std::cout << "[Dear ImGui Render Loop | 144 FPS]\n";
    std::cout << "Time Remaining Bar: [";
    int timeTicks = static_cast<int>((state.timeRemainingSeconds / state.timeLimitSeconds) * 20);
    for (int i = 0; i < 20; ++i) std::cout << (i < timeTicks ? "=" : " ");
    std::cout << "] " << state.timeRemainingSeconds << "s\n";

    std::cout << "Heat Radar Gauge:   [";
    int heatTicks = static_cast<int>(state.currentHeat * 20);
    for (int i = 0; i < 20; ++i) std::cout << (i < heatTicks ? "#" : ".");
    std::cout << "] " << static_cast<int>(state.currentHeat * 100.0f) << "%\n";
    std::cout << "Screen Trauma Offset: (" << state.screenOffset.x << ", " << state.screenOffset.y << ")\n";
}

int main() {
    GameState game;
    game.startNewRound();
    
    // Simulate interactive frames
    std::cout << "Initializing High-Performance Immediate-Mode Render Subsystem...\n";
    RenderGameUI(game, 0.016f); // 60 FPS frame delta
    
    std::cout << "\nSubmitting test guess 50...\n";
    game.submitGuess(50);
    RenderGameUI(game, 0.016f);

    return 0;
}
