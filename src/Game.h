//
// Created by jansensamosa on 5/4/24.
//

#ifndef GAME_H
#define GAME_H

#include "Shader.h"
#include "World.h"

class Game {
public:
    static constexpr double TIMESTEP = 0.02;

    Game(Shader &prog);

    void Update();
    void Render();
    void brushElement(int x, int y, int radius, int ELEMENT_ID);

private:
    World world;

    // rendering data
    unsigned int gameTextureID{};
    unsigned int renderPlaneID{}; // a VAO
    unsigned int VBO{};
    Shader shaderProgram;
    std::uint8_t* buffer;

    std::uint8_t gameTime = 0; // how many ticks have elapsed since game started

    void MoveLikePowder(int x, int y);
    void MoveLikeLiquid(int x, int y);
    void MoveLikeGas(int x, int y);

    void UpdateAsSand(int x, int y);
    void UpdateAsWater(int x, int y);
    void UpdateAsOil(int x, int y);
    void UpdateAsFire(int x, int y);
    void UpdateAsBurningWood(int x, int y);
    void UpdateAsSteam(int x, int y);
    void UpdateAsInfiniteSource(int x, int y);
    void UpdateAsPlant(int x, int y);
    void UpdateAsAcid(int x, int y);
    void UpdateAsLava(int x, int y);
    void UpdateAsGunpowder(int x, int y);
    void UpdateAsSalt(int x, int y);
    void UpdateAsAcidCloud(int x, int y);
    void UpdateAsRock(int x, int y);

    void updateElementLifetime(int x, int y);

    void placeExplosion(int x, int y, int radius);
    void burnNeighbors(int x, int y);
};

#endif //GAME_H

