
//
// Created by jansensamosa on 5/23/24.
//

#ifndef ELEMENTDEFINITIONS_H
#define ELEMENTDEFINITIONS_H

//
// Created by jansensamosa on 5/23/24.
//

#include <cstdint>

namespace props {
    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;

        Color(int r, int g, int b) : r(r), g(g), b(b) {}

        bool operator==(const Color& other) const {
            return r == other.r && g == other.g && b == other.b;
        }
    };

    struct element {
        uint8_t elementID = 0;
        uint8_t lifeTime = 2;
        uint8_t customData = 0;
        uint8_t verticalVelocity = 0;
        // this is true iff this element has been already been updated in the current timestep
        // this is necessary to prevent an element from updating multiple time
        uint8_t updated = false;

        element() {};
        element(uint8_t id) : elementID(id) {}
        element(uint8_t id, uint8_t lifeTime) : elementID(id), lifeTime(lifeTime) {}
    };


    enum elements { AIR_ID, SAND_ID, WATER_ID, OIL_ID, DIRT_ID, FIRE_ID, WOOD_ID, WOODBURNING_ID, STEAM_ID,
        INFINITESOURCE_ID, PLANT_ID, ACID_ID, LAVA_ID, GUNPOWDER_ID, ROCK_ID, SALT_ID, SALTWATER_ID, ACIDCLOUD_ID,
        SIZE_ALWAYS_AT_END};
    const uint8_t ELEMENT_DENSITIES[] = {2, 10, 5, 4, 10, 1, 255, 255, 1, 255, 255, 5, 6, 10, 255, 10, 6, 1};
    const uint8_t ELEMENT_VISCOSITIES[] = {0, 0, 4, 6, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 4, 0};
    const uint8_t ELEMENT_DECAY_RATES[] = {1, 1, 1, 1, 1, 1, 1, 2, 8, 1, 1, 1, 1, 1, 1, 1, 1, 8};

    const Color ELEMENTS_COLORS[] = {
        {36, 39, 43}, // nothing
        {255, 192, 140}, // sand
        {94, 117, 214},  // water
        {31, 24, 27}, // oil
        {74, 50, 41}, // dirt
        {217, 54, 9}, // fire
        {194, 101, 54}, // wood
        {222, 87, 4}, //wood burning
        {133, 151, 158}, // steam
        {232, 208, 74}, //infinite fire
        {0, 255, 0}, //plant
        {159, 247, 27}, // acid
        {255, 94, 41}, // lava
        {120, 108, 99}, // gunpowder
        {150, 150, 150}, // rock
        {255, 255, 255}, // salt,
        {116, 163, 237}, // salt water
        {121, 156, 126}, // acid cloud
    };


    const element ELEMENTS_TEMPLATES[]= {
        {AIR_ID, 1},
        {SAND_ID, 1},
        {WATER_ID, 1},
        {OIL_ID, 1},
        {DIRT_ID, 1},
        {FIRE_ID, 2},
        {WOOD_ID, 1},
        {WOODBURNING_ID, 200},
        {STEAM_ID, 255},
        {INFINITESOURCE_ID, 1},
        {PLANT_ID, 1},
        {ACID_ID, 1},
        {LAVA_ID, 1},
        {GUNPOWDER_ID, 1},
        {ROCK_ID, 1},
        {SALT_ID, 1},
        {SALTWATER_ID, 1},
        {ACIDCLOUD_ID, 255},
    };
}

#endif //ELEMENTDEFINITIONS_H

