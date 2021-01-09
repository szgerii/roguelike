#pragma once
#include "Object/Tiles/Tile.h"

namespace CR::Objects {
    class EndTile : public Tile {
    public:
        EndTile(const Vector2<int>& pos)
            : Tile(' ', BACKGROUND_GREEN, pos) {}

        virtual void hurt(float amount, GameObject* from);
    };
}
