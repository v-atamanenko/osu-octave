#include <cstdint>
#include "DifficultyManager.h"

OOFloat DifficultyManager::HP = 5;
OOFloat DifficultyManager::CS = 5;
OOFloat DifficultyManager::OD = 5;
OOFloat DifficultyManager::SliderMultiplier = 1.0;
OOFloat DifficultyManager::SliderTickRate = 1.0;

OOTime DifficultyManager::PreemptMs = 0;
OOFloat DifficultyManager::CircleDiameterPx = 0;
OOFloat DifficultyManager::RequiredRPS = 0;
OOTime DifficultyManager::FadeInMs = 0;
OOTime DifficultyManager::HitWindow300 = 0;
OOTime DifficultyManager::HitWindow100 = 0;
OOTime DifficultyManager::HitWindow50 = 0;
OOTime DifficultyManager::HitWindow = 0;
