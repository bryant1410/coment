#pragma once

#include "coment/systems/System.h"
#include "coment/util/EntityMap.h"

#include "components/Position.h"
#include "components/Velocity.h"

class MovementSystem
    : public coment::System<MovementSystem>
{
public:

    /* Callbacks */

    /** Move objects on update */
    void onUpdate() override
    {
        static Uint32 lastUpdate = SDL_GetTicks();

        // Update time
        Uint32 time = SDL_GetTicks();
        float dt = (float)(time - lastUpdate) * 0.001f;

        // Map of circles
        static auto mMovers = getWorld()->getEntityMap<Position, Velocity>();

        // Draw circles
        for (auto& mover : mMovers)
        {
            // Get components
            Position* position = mover.getComponent<Position>();
            Velocity* velocity = mover.getComponent<Velocity>();

            position->x += dt * velocity->x;
            position->y += dt * velocity->y;
        }

        // Update lastUpdate
        lastUpdate = time;
    }

};