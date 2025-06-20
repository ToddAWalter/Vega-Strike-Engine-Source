/*
 * particle.h
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
#ifndef VEGA_STRIKE_ENGINE_GFX_PARTICLE_H
#define VEGA_STRIKE_ENGINE_GFX_PARTICLE_H

#include <vector>
#include <string>
#include <memory>
#include <string>
#include "aligned.h"
#include "gfx_generic/vec.h"
#include "src/gfxlib_struct.h"

class Texture;

struct ParticlePoint {
    QVector loc;
    GFXColor col;
    float size;
};

struct Particle {
    Vector velocity{};
    QVector location{};
    GFXColor color{};
    float size{};
};

/**
 * Particle system class, contains regularly updated geometry for all active
 * particles of the same kind.
 *
 * Can be instantiated statically.
 */
class ParticleTrail {
    std::vector<Particle, aligned_allocator<Particle>> particles;
    std::vector<float> particleVert;
    std::vector<float> distances;
    std::vector<unsigned short> pointIndices;
    std::vector<unsigned short> indices;
    unsigned int max_particles{};
    BLENDFUNC blendsrc, blenddst;
    float alphaMask;
    bool writeDepth, fadeColor;

    struct Config {
        std::string prefix;
        bool initialized;

        bool use;
        bool use_points;
        bool pblend;
        bool psmooth;
        float pgrow;
        float ptrans;
        float pfade;
        float psize;
        Texture *texture;

        explicit Config(const std::string &prefix);
        Config();
        ~Config();

        void init();
    } config{};

public:
    ParticleTrail(const std::string &configPrefix,
            unsigned int max,
            BLENDFUNC blendsrc = ONE,
            BLENDFUNC blenddst = ONE,
            float alphaMask = 0,
            bool writeDepth = false,
            bool fadeColor = false)
            : config(configPrefix) {
        ChangeMax(max);
        this->blendsrc = blendsrc;
        this->blenddst = blenddst;
        this->alphaMask = alphaMask;
        this->writeDepth = writeDepth;
        this->fadeColor = fadeColor;
    }

    void DrawAndUpdate();
    void AddParticle(const ParticlePoint &, const Vector &, float size);
    void ChangeMax(unsigned int max);
};

/**
 * Particle emitter, given a particle system, it emits particles with randomized
 * position and directions, based on config key. Cannot be instantiated statically
 * since it queries vsConfig at construction time (which is not available statically)
 */
class ParticleEmitter {
    ParticleTrail *particles;

public:
    struct Config {
        bool fixedSize;

        float rate;
        float speed;
        float locSpread;
        float spread;
        float absSpeed;
        float relSize;

        void init(const std::string &prefix);
    } config{};

    explicit ParticleEmitter(ParticleTrail *particleType) : particles(particleType) {
    }

    explicit ParticleEmitter(ParticleTrail *particleType, const std::string &prefix) : particles(particleType) {
        config.init(prefix);
    }

    /**
     * Launches (maybe) a particle, according to:
     *
     * @param pos Emitter center
     * @param rSize Emitter radial size
     * @param percent Emitter rate relative to configured rate (inverse rate, effective_rate = rate/percent)
     * @param basevelocity Emitter velocity directly translated to particle velocity
     * @param velocity Particle velocity relative to emitter velocity
     * @param pSize Particle size - ignored if emitter configured with fixed particle size
     * @param color Particle color
     *
     */
    void doParticles(const QVector &pos,
            float rSize,
            float percent,
            const Vector &basevelocity,
            const Vector &velocity,
            float pSize,
            const GFXColor &color) const;
};

extern ParticleTrail particleTrail;
extern ParticleTrail smokeTrail;
extern ParticleTrail debrisTrail;

#endif //VEGA_STRIKE_ENGINE_GFX_PARTICLE_H
