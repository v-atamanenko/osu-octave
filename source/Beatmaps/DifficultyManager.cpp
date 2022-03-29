#include <cstdint>
#include "DifficultyManager.h"

uint32_t DifficultyManager::preemptTime[] = {1800, 1680, 1560, 1440, 1320, 1200, 1050, 900, 750, 600, 450};
//uint32_t DifficultyManager::circleSize[] = {100, 92, 84, 76, 68, 60, 52, 44, 36, 28, 20};
uint32_t DifficultyManager::circleSize[] = {100, 92, 84, 76, 68, 60, 52, 52, 52, 52, 52};
//uint32_t DifficultyManager::circleSize[] = {120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20};
uint32_t DifficultyManager::hitWindow300[] = {80, 74, 68, 62, 56, 50, 44, 38, 32, 26, 20};
uint32_t DifficultyManager::hitWindow100[] = {140, 132, 124, 116, 108, 100, 92, 84, 76, 68, 60};
uint32_t DifficultyManager::hitWindow50[] = {200, 190, 180, 170, 160, 150, 140, 130, 120, 110, 100};
uint32_t DifficultyManager::spinnerTime[] = {525, 480, 435, 390, 345, 300, 270, 240, 210, 180, 150};
float DifficultyManager::missHpDrain[] = {-10, -18, -26, -34, -42, -50, -56, -62, -68, -74, -80};

uint8_t DifficultyManager::DifficultyHpDrain = 5;
uint8_t DifficultyManager::DifficultyCircleSize = 5;
uint8_t DifficultyManager::DifficultyOverall = 5;
float DifficultyManager::SliderMultiplier = 1.0;
float DifficultyManager::SliderTickRate = 1.0;
uint8_t DifficultyManager::DifficultyPeppyStars = 3;

long DifficultyManager::preempt_time_new = 0;
float DifficultyManager::circle_size_new = 0;
float DifficultyManager::spinner_rps = 0;
long DifficultyManager::fadeInMs = 0;
long DifficultyManager::hit_window_300 = 0;
long DifficultyManager::hit_window_100 = 0;
long DifficultyManager::hit_window_50 = 0;
long DifficultyManager::hit_window = 0;
long DifficultyManager::spinner_time = 0;
long DifficultyManager::miss_hp_drain = 0;
