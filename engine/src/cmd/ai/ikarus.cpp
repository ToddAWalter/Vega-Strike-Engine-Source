/*
 * ikarus.cpp
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


#include "aggressive.h"
#include "event_xml.h"
#include "script.h"
#include <list>
#include <vector>
#include "root_generic/vs_globals.h"
#include "src/config_xml.h"
#include "root_generic/xml_support.h"
#include "cmd/unit_generic.h"
#include "communication.h"
#include "cmd/script/flightgroup.h"
#include "flybywire.h"
#include "hard_coded_scripts.h"
#include "cmd/script/mission.h"
#include "src/universe_util.h"
#include "gfx_generic/cockpit_generic.h"
#include "ikarus.h"
using namespace Orders;

Ikarus::Ikarus() : AggressiveAI("default.agg.xml") {
    last_time = cur_time = 0;
}

void Ikarus::ExecuteStrategy(Unit *target) {
    WillFire(target);
    if (queryType(Order::FACING) == NULL) {
        //we have nothing to do now
        FaceTarget(false);
        AddReplaceLastOrder(true);         //true stands for override any that are there
    }
    if (queryType(Order::MOVEMENT) == NULL) {
        MatchLinearVelocity(true,
                Vector(0, 0, 10000),
                false,
                true);         //all ahead full! (not with afterburners but in local coords)
        AddReplaceLastOrder(true);
    }
    //might want to enqueue both movement and facing at the same time as follows:
    if (0) {
        ReplaceOrder(new AIScript("++turntowards.xml"));
    }          //find this list of names in script.cpp

    cur_time += simulation_atom_var; //SIMULATION_ATOM;
    if (cur_time - last_time > 5) {
        //dosomething

        last_time = cur_time;
    }
}

void Ikarus::WillFire(Unit *target) {
    bool missilelockp = false;
    if (ShouldFire(target, missilelockp)) {
        // this is a function from fire.cpp  you probably want to write a better one
        // Roy Falk - This function was actually in unit_generic and moved to armed
        // The false is coerced into unsigned int for the bitmask.
        // I suspect that while this compiled, the boolean was not meant to do apply to the first parameter but to the second one -
        // either beams_target_owner or listen_to_owner
        parent->Fire(false);
    }
    if (missilelockp) {
        parent->Fire(true);         //if missiles locked fire
        parent->ToggleWeapon(true);
    }
}

///you should certainly edit this!!
void Ikarus::DecideTarget() {
    Unit *targ = parent->Target();
    if (!targ || /*some other qualifying factor for changing targets*/ 0) {
        Unit *un = NULL;
        for (UniverseUtil::PythonUnitIter i = UniverseUtil::getUnitList(); (un = *i); ++i) {
            if (parent->getRelation(un) < 0) {
                parent->Target(un);
                break;
            }
        }
    }
}

///you can ignore the function below unless it causes problems...this merely makes it so that the AI responds to your commands
void Ikarus::Execute() {
    Flightgroup *fg = parent->getFlightgroup();
    ReCommandWing(fg);
    CommunicatingAI::Execute();
    DecideTarget();
    if (!ProcessCurrentFgDirective(fg)) {
        Unit *target = parent->Target();
        bool isjumpable = target ? ((!target->GetDestinations().empty()) &&
            parent->jump_drive.IsDestinationSet()) : false;
        if (isjumpable) {
            AfterburnTurnTowards(this, parent);
        } else {
            ExecuteStrategy(target);
        }
    }
}

