#pragma once

#include <raylib.h>
#include <rlgl.h>
#include "VectorMath.h"
#include <iostream>

//#include "../rcamera.h"

#include <raymath.h>
#include <string>

#define PLATFORM_DESKTOP

#if defined(PLATFORM_DESKTOP)

  #define GLSL_VERSION  330

#endif

Vec2<float> getWsize();
std::string GetRelativeTexturePath(std::string textureName);

class cubemap
{
public:

	Camera camera = { 0 };
	Model skybox;
	Shader CubeMapShader;
    float CameraRotationSpeed = 0.0001f;

	cubemap(const char* fileName)
	{
        
        camera = { 0 };
        camera.position = { 1.0f, 1.0f, 1.0f };    
        camera.target = { 4.0f, 1.0f, 4.0f };      
        camera.up = { 0.0f, 1.0f, 0.0f };          
        camera.fovy = 45.0f;                                
        camera.projection = CAMERA_PERSPECTIVE;             

        Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
        skybox = LoadModelFromMesh(cube);

        bool useHDR = false;

        skybox.materials[0].shader = LoadShader(TextFormat(GetRelativeTexturePath("skybox.vs").c_str(), GLSL_VERSION),
            TextFormat(GetRelativeTexturePath("skybox.fs").c_str(), GLSL_VERSION));

        int UseHDRuniform = useHDR ? 1 : 0;

        MaterialMapIndex cubeMapIndex = MATERIAL_MAP_CUBEMAP;

        SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), &cubeMapIndex, SHADER_UNIFORM_INT);
        SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), &UseHDRuniform, SHADER_UNIFORM_INT);
        SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), &UseHDRuniform, SHADER_UNIFORM_INT);
       
        CubeMapShader = LoadShader(TextFormat(GetRelativeTexturePath("cubemap.vs").c_str(), GLSL_VERSION),
            TextFormat(GetRelativeTexturePath("cubemap.fs").c_str(), GLSL_VERSION));

        int Index = 0;

        SetShaderValue(CubeMapShader, GetShaderLocation(CubeMapShader, "equirectangularMap"), &Index, SHADER_UNIFORM_INT);

        char skyboxFileName[256] = { 0 };

        Texture2D panorama;

        Image img = LoadImage(GetRelativeTexturePath("StandardCubeMap.png").c_str());
        skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);    // CUBEMAP_LAYOUT_PANORAMA
        UnloadImage(img);
             
        DisableCursor();
    }

    

	void Draw()
	{
		UpdateCameraPro(&this->camera,{0,0,0},{0,0,0},0);
		BeginMode3D(this->camera);
		ClearBackground(WHITE);
		rlDisableBackfaceCulling();
		rlDisableDepthMask();
        camera.target = Vector3RotateByAxisAngle(camera.target, {0.0f,1.0f,0.f}, CameraRotationSpeed * (PI * 180) * GetFrameTime());
        DrawModel(skybox, { 0,0,0 },1.0f, WHITE);
		rlEnableBackfaceCulling();
		rlEnableDepthMask();
        
		EndMode3D();

	}

	void clear()
	{
		UnloadShader(skybox.materials[0].shader);
		UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
	}

};

