#include "LevelHandler.h"
#include <iostream>
#include "util/WindowUtil.h"
#include <algorithm>

GameLevel::Level::Level(const char* filePath)
{
    std::ifstream LevelFile(filePath);
    LevelFile >> j;
    LevelFile.close();

    std::vector<std::string> TextureNames;

    const json& GameObjects = j["GameObjects"];

    for (auto object = GameObjects.begin(); object != GameObjects.end(); object++)
    {
        const json& attributes = object.value();
        
        const std::string& objectType = attributes["type"];


        bool allow = true;
        std::string textureName = std::string(attributes["texture"]);

        for (auto& texture : Textures)
        {
            if (texture.first == textureName)
            {
                allow = false;
                break;
            }
        }

        if (allow)
        {
            Texture2D NewTexture = LoadTexture(GetRelativeTexturePath(textureName).c_str());
            Textures.push_back({ textureName , NewTexture });
        }



        if (objectType == "obstacle")
        {
            GameObject obstacle;

            ObjectData temp({ { attributes["data"]["rec"]["x"],
                           attributes["data"]["rec"]["y"],
                           attributes["data"]["rec"]["width"],
                           attributes["data"]["rec"]["height"] },
                           {attributes["data"]["pos"]["x"] ,
                           attributes["data"]["pos"]["y"] },
                           attributes["data"]["frame"],
                           attributes["data"]["runningTime"],
                           attributes["data"]["updateTime"],
                           attributes["data"]["speed"] });

            obstacle.Data(temp);
            obstacle.scale = attributes["scale"];
            obstacle.rotation = attributes["rotation"];
            obstacle.tint = { attributes["tint"]["r"] ,
                              attributes["tint"]["g"] ,
                              attributes["tint"]["b"] ,
                              attributes["tint"]["a"] };

            obstacle.DuplicateCount = attributes["duplicateCount"];
            obstacle.RenderDepthIndex = attributes["renderDepthIndex"];
            obstacle.RenderMethod = attributes["renderMethod"];
            obstacle.duplicateDistance = attributes["duplicateDistance"];
            obstacle.atlasCut = { attributes["atlasCut"]["x"] ,attributes["atlasCut"]["y"] };
            obstacle.reverseSpeed = attributes["reverseSpeed"];

            for (auto& texture : Textures)
            {
                if (texture.first == textureName)
                {
                    obstacle.SetTexture(texture.second);
                    break;
                }

            }

            this->Obstacles.push_back(obstacle);
            RenderQueue.push_back(obstacle);
        }
        else if (objectType == "SceneObject")
        {
            GameObject SceneObject;

            ObjectData temp({ { attributes["data"]["rec"]["x"],
                           attributes["data"]["rec"]["y"],
                           attributes["data"]["rec"]["width"],
                           attributes["data"]["rec"]["height"] },
                           {attributes["data"]["pos"]["x"] ,
                           attributes["data"]["pos"]["y"] },
                           attributes["data"]["frame"],
                           attributes["data"]["runningTime"],
                           attributes["data"]["updateTime"],
                           attributes["data"]["speed"] });
            
            SceneObject.Data(temp);
            SceneObject.scale = attributes["scale"];
            SceneObject.rotation = attributes["rotation"];
            SceneObject.tint = { attributes["tint"]["r"] ,
                              attributes["tint"]["g"] ,
                              attributes["tint"]["b"] ,
                              attributes["tint"]["a"] };

            SceneObject.DuplicateCount = attributes["duplicateCount"];
            SceneObject.RenderDepthIndex = attributes["renderDepthIndex"];
            SceneObject.RenderMethod = attributes["renderMethod"];
            SceneObject.duplicateDistance = attributes["duplicateDistance"];

            for (auto& texture : Textures)
            {
                if (texture.first == textureName)
                {
                    SceneObject.SetTexture(texture.second);
                    break;
                }

            }

            this->SceneObjects.push_back(SceneObject);
            RenderQueue.push_back(SceneObject);
        }
        else if (objectType == "NPC")
        {

        }

    }

    std::sort(RenderQueue.begin(), RenderQueue.end());

    std::cout << "TextureCount: " << Textures.size() << std::endl;

}

void GameLevel::Level::Clean()
{
    for (auto& texture : Textures)
    {
        UnloadTexture(texture.second);
    }
}

void GameLevel::Level::RenderScene()
{
    for (auto& object : RenderQueue)
    {
        if (object.RenderMethod == "DuplicateEx")
        {
            object.RenderDuplicateEx(object.DuplicateCount, object.duplicateDistance, object.tint);
        }
        else if (object.RenderMethod == "DuplicateExLoop")
        {
            //object.RenderDuplicateExLoop(object.DuplicateCount,object.duplicateDistance,object.tint)
        }
        else if (object.RenderMethod == "DuplicateRec")
        {
            object.RenderDuplicateRec(object.DuplicateCount, object.duplicateDistance, object.tint);
        }
        else if (object.RenderMethod == "DuplicateRecLoop")
        {
            //object.RenderDuplicateRecLoop(object.DuplicateCount,object.duplicateDistance,object.tint,object)
        }
    }
}


void GameLevel::ReadLevel()
{

}

void GameLevel::WriteLevel(GameObject& Object)
{

}
