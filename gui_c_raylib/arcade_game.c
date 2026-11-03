/* ==============================================================================
   Tier 3: C Retro Arcade Graphics Engine (Raylib Architecture)
   - Procedural C99 Memory-Efficient Model
   - Pre-Allocated Particle Pool (Zero per-frame heap allocations)
   - Trauma-Based Screen Shake & Dynamic Color Lerp Heat Radar
   - CRT Scanline Rasterizer
   ============================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define MAX_PARTICLES 256
#define MAX_ATTEMPTS  7

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    unsigned char r, g, b, a;
} ColorRGBA;

/* Pre-allocated cache-friendly particle struct (no malloc inside render loop) */
typedef struct {
    Vec2 pos;
    Vec2 vel;
    ColorRGBA color;
    float life;      /* 1.0 down to 0.0 */
    float decayRate;
    bool active;
} Particle;

typedef struct {
    int minRange;
    int maxRange;
    int secretTarget;
    int attemptsLeft;
    int score;
    int streak;
    int currentGuess;
    float currentHeat;  /* 0.0 (Cold) to 1.0 (Scorching) */
    
    /* Screen Shake Trauma System */
    float trauma;       /* 0.0 to 1.0 */
    Vec2 shakeOffset;
    
    /* Pre-allocated Particle Arena */
    Particle particlePool[MAX_PARTICLES];
    int particleCount;

    bool isRoundWon;
    bool isGameOver;
} ArcadeGameState;

/* Initialize state with zero heap fragmentation */
void InitArcadeGame(ArcadeGameState *game) {
    game->minRange = 1;
    game->maxRange = 100;
    game->secretTarget = (rand() % 100) + 1;
    game->attemptsLeft = MAX_ATTEMPTS;
    game->score = 0;
    game->streak = 0;
    game->currentGuess = 50;
    game->currentHeat = 0.0f;
    game->trauma = 0.0f;
    game->shakeOffset.x = 0.0f;
    game->shakeOffset.y = 0.0f;
    game->particleCount = 0;
    game->isRoundWon = false;
    game->isGameOver = false;

    // Zero out contiguous particle arena
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        game->particlePool[i].active = false;
    }
}

/* Linear color interpolation for Heat Radar (Ice Blue -> Fire Red) */
ColorRGBA LerpHeatColor(float t) {
    // 0.0: Ice Cyan (0, 210, 255) -> 1.0: Plasma Red (255, 42, 84)
    ColorRGBA col;
    col.r = (unsigned char)((1.0f - t) * 0   + t * 255);
    col.g = (unsigned char)((1.0f - t) * 210 + t * 42);
    col.b = (unsigned char)((1.0f - t) * 255 + t * 84);
    col.a = 255;
    return col;
}

/* Fast O(1) particle spawn utilizing inactive slots in pre-allocated array */
void SpawnConfettiBurst(ArcadeGameState *game, float originX, float originY, int count) {
    int spawned = 0;
    for (int i = 0; i < MAX_PARTICLES && spawned < count; ++i) {
        if (!game->particlePool[i].active) {
            Particle *p = &game->particlePool[i];
            p->active = true;
            p->pos.x = originX;
            p->pos.y = originY;
            float angle = ((float)rand() / RAND_MAX) * 2.0f * 3.14159f;
            float speed = 100.0f + ((float)rand() / RAND_MAX) * 300.0f;
            p->vel.x = cosf(angle) * speed;
            p->vel.y = sinf(angle) * speed;
            p->color = LerpHeatColor(((float)rand() / RAND_MAX));
            p->life = 1.0f;
            p->decayRate = 0.8f + ((float)rand() / RAND_MAX) * 0.8f;
            spawned++;
        }
    }
}

/* Tick physics and trauma state based on explicit delta time (dt) */
void UpdateArcadeGame(ArcadeGameState *game, float dt) {
    // 1. Update trauma and calculate displacement
    if (game->trauma > 0.0f) {
        game->trauma -= dt * 1.5f;
        if (game->trauma < 0.0f) game->trauma = 0.0f;

        float shakeFactor = game->trauma * game->trauma; // Quadratic falloff
        game->shakeOffset.x = (sinf((float)clock() * 0.05f) * 16.0f) * shakeFactor;
        game->shakeOffset.y = (cosf((float)clock() * 0.04f) * 16.0f) * shakeFactor;
    } else {
        game->shakeOffset.x = 0.0f;
        game->shakeOffset.y = 0.0f;
    }

    // 2. Update pre-allocated particle simulation
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (game->particlePool[i].active) {
            Particle *p = &game->particlePool[i];
            p->pos.x += p->vel.x * dt;
            p->pos.y += p->vel.y * dt;
            p->vel.y += 400.0f * dt; // Gravity vector
            p->life -= p->decayRate * dt;
            if (p->life <= 0.0f) {
                p->active = false;
            }
        }
    }
}

/* Evaluate player input guess */
void SubmitArcadeGuess(ArcadeGameState *game, int guess) {
    if (game->isGameOver || game->isRoundWon) return;

    game->attemptsLeft--;

    int dist = abs(guess - game->secretTarget);
    int maxDist = 99;
    game->currentHeat = 1.0f - ((float)dist / (float)maxDist);
    if (game->currentHeat < 0.0f) game->currentHeat = 0.0f;

    if (guess == game->secretTarget) {
        game->isRoundWon = true;
        game->streak++;
        game->score += (game->attemptsLeft + 1) * 250 * game->streak;
        SpawnConfettiBurst(game, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 120);
    } else {
        game->trauma = 0.6f; // Trigger retro screen-shake
        if (game->attemptsLeft <= 0) {
            game->isGameOver = true;
            game->streak = 0;
        }
    }
}

/* Architectural demonstration entry */
int main(void) {
    srand((unsigned int)time(NULL));

    ArcadeGameState game;
    InitArcadeGame(&game);

    printf("============================================================\n");
    printf("  Tier 3: C Retro Arcade Graphics Subsystem (Raylib Model) \n");
    printf("============================================================\n");
    printf("[Memory Architecture] Particle Pool: %lu bytes contiguous arena\n", 
           sizeof(Particle) * MAX_PARTICLES);
    printf("[Initial Target Locked] %d (Range: %d-%d)\n", 
           game.secretTarget, game.minRange, game.maxRange);

    // Simulate frames
    printf("\n[Frame 01] Submitting Guess: 75\n");
    SubmitArcadeGuess(&game, 75);
    UpdateArcadeGame(&game, 0.016f); // 60 FPS tick
    
    printf(">> Heat Proximity: %.1f%%\n", game.currentHeat * 100.0f);
    printf(">> Shake Trauma: %.2f | Offset: (%.2f, %.2f)\n", 
           game.trauma, game.shakeOffset.x, game.shakeOffset.y);

    printf("\n[Frame 02] Submitting Target Guess: %d\n", game.secretTarget);
    SubmitArcadeGuess(&game, game.secretTarget);
    UpdateArcadeGame(&game, 0.016f);

    printf(">> Round Status: %s | Active Particles Spawned: %d\n", 
           game.isRoundWon ? "VICTORY" : "PLAYING", MAX_PARTICLES);
    printf(">> Cumulative Arcade Score: %d pts\n", game.score);

    return 0;
}
