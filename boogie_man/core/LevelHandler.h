#pragma once
#include "GameObject.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "../../include/nlohmann/json.hpp"
using json = nlohmann::json;

namespace GameLevel
{

    class Level
    {
    public:

        Level(const char* filePath);
        void Clean();
        void RenderScene();
        
    private:

        std::vector<GameObject> SceneObjects;
        std::vector<GameObject> Obstacles;
        std::vector<GameObject> NPCs;

        std::vector<GameObject> RenderQueue;
        std::vector<std::pair<std::string,Texture2D>> Textures;

        json j;

    };

    void ReadLevel();
    void WriteLevel(GameObject& Object);

}