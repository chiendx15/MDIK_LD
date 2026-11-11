#pragma once
#include "magic_enum.hpp"

#define UP TRUE
#define DOWN FALSE
#define KEEP -1

void LoadConveyor1(PIPCOMMAND _pCmd = nullptr);
void LoadConveyor2(PIPCOMMAND _pCmd = nullptr);
void ElevatorConveyor(PIPCOMMAND _pCmd = nullptr);
void UnloadConveyor1(PIPCOMMAND _pCmd = nullptr);
void UnloadConveyor2(PIPCOMMAND _pCmd = nullptr);
void GlassTransfer(PIPCOMMAND _pCmd = nullptr);