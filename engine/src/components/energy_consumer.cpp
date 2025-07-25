/*
 * energy_consumer.cpp
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

// -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include "components/energy_consumer.h"

double EnergyConsumer::simulation_atom_var = 0.1;

EnergyConsumerSource GetSource(const int source) {
    switch(source) {
        case 0: return EnergyConsumerSource::Infinite;
        case 1: return EnergyConsumerSource::Fuel;
        case 2: return EnergyConsumerSource::Energy;
        case 3: return EnergyConsumerSource::FTLEnergy;
        default: return EnergyConsumerSource::NoSource;
    }
}

EnergyConsumer::~EnergyConsumer()
= default;

EnergyConsumer::EnergyConsumer(EnergyContainer *source,
                               bool partial,
                               double consumption, bool infinite):
                               consumption(consumption),
                               atom_consumption(consumption * simulation_atom_var),
                               source(source),
                               partial(partial),
                               infinite(false) {}

bool EnergyConsumer::CanConsume() const {
    if(infinite) {
        return true;
    }

    // TODO: need to check if operational somewhere else
    if(!source) {
        return false;
    }

    return source->Level() > atom_consumption;
}

double EnergyConsumer::Consume() {
    if(infinite) {
        return atom_consumption;
    }

    if(!source) {
        return 0.0;
    }

    return source->Deplete(partial, atom_consumption);
}

double EnergyConsumer::GetConsumption() const {
    return consumption;
}

double EnergyConsumer::GetAtomConsumption() const {
    return atom_consumption;
}

void EnergyConsumer::SetConsumption(double consumption) {
    this->consumption = consumption;
    atom_consumption = consumption * simulation_atom_var;
}

void EnergyConsumer::SetSource(EnergyContainer* source) {
    this->source = source;
}

void EnergyConsumer::ZeroSource() {
    if(!source) {
        return;
    }

    source->Zero();
}
