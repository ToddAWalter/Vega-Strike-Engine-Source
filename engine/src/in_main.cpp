/*
 * in_main.cpp
 *
 * Vega Strike - Space Simulation, Combat and Trading
 * Copyright (C) 2001-2025 The Vega Strike Contributors:
 * Project creator: Daniel Horn
 * Original development team: As listed in the AUTHORS file
 * Current development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy
 *
 *
 * https://github.com/vegastrike/Vega-Strike-Engine-Source
 *
 * This file is part of Vega Strike.
 *
 * Vega Strike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vega Strike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vega Strike.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <queue>
#include <list>
using std::queue;
using std::list;

#include "src/in_kb.h"
#include "src/in_mouse.h"
#include "src/in_joystick.h"
#include "src/in_handler.h"

#include <map>


queue<InputListener *> activationreqqueue;
list<InputListener *> listeners;
InputListener *activelistener;

void AddListener(InputListener *il) {
    il->mousex = &mousex;
    il->mousey = &mousey;
    listeners.push_back(il);
}

void ActivateListener(InputListener *il) {
    activationreqqueue.push(il);
}

void RemoveListener(InputListener *il) {
    listeners.remove(il);
}

void ProcessInput(size_t whichplayer) {
    ProcessKB();
    ProcessMouse();
    for (int i = 0; i < MAX_JOYSTICKS; i++) {
        if (joystick[i]->player == whichplayer) {
            ProcessJoystick(i);
        }
    }
}

void InitInput() {
    InitMouse();
    InitJoystick();
}

void DeInitInput() {
    DeInitJoystick();
}

