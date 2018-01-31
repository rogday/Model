#pragma once

#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <initializer_list>
#include <list>
#include <random>
#include <string>
#include <vector>

#define NDEBUG

#ifndef NDEBUG
#include <iostream>
#endif

class ICreature;

namespace Settings {
// aim FPS
const int FPS = 75;

// window size multiplier
const float SizeScale = 0.5;

// dimentions
const int N = 100;
const int M = N;

// cellTypes
enum Types { Fox = 0, Bunny, Carrot, None }; // None is always in the end

// LifeSpan
const int CarrotLifeSpan = 30;
const int FoxLifeSpan = 150;
const int BunnyLifeSpan = 70;

// Delay
const int FoxDelay = 35;
const int BunnyDelay = 7;

// Speed
const int FoxSpeed = 1;
const int BunnySpeed = 2;

// FOV
const int FoxFOV = 3;
const int BunnyFOV = 2;

// Nutritional Values
const int CarrotNuVal = 35;
const int BunnyNuVal = 80;

// Probablilities
const double EmptyProb = 35.0;
const double CarrotProb = 35.0;
const double FoxProb = 15.0;
const double BunnyProb = 15.0;

const std::initializer_list<double> Probs = {EmptyProb, FoxProb, BunnyProb,
											 CarrotProb};

// Fabric
extern std::array<std::function<ICreature *(int, int, bool)>, 3> Allocators;
extern std::array<std::string, 3> Names;
extern std::array<sf::Color, 3> Colors;

bool bounds(int x, int n);

unsigned long xorshf96(void);

} // namespace Settings