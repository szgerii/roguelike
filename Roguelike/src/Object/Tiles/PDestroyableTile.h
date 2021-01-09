#pragma once
#include "Object/Tiles/DestroyableTile.h"

namespace CR::Objects {
    class PDestroyableTile : public DestroyableTile {
    public:
        PDestroyableTile(char skin, unsigned short skinColor, const Vector2<int>& pos, int initialHealth)
            : DestroyableTile(skin, skinColor, pos, initialHealth) {}

        virtual void hurt(float amount, GameObject* from);
    };
}
