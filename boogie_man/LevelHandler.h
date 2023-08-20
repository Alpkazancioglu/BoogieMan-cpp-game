#pragma once
#include "GameObject.h"
#include <vector>
#include "../include/nlohmann/json.hpp"
using json = nlohmann::json;

namespace GameLevel
{

    class Level
    {
    public:

        Level()
        {



        }

    private:

        std::vector<GameObject> SceneObjects;
        std::vector<GameObject> NPCs;
        std::vector<Texture2D> Textures;


    };

    void ReadLevel();
    void WriteLevel();

    class Creature
    {
    public:

        Creature():name("Creature") , age(12)
        {

        }

        std::string name;
        int age;
    };
}