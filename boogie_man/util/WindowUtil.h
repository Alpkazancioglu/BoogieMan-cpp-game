#pragma once

#include <raylib.h>
#include <rlgl.h>
#include "VectorMath.h"
#include <iostream>
//#define  RCAMERA_IMPLEMENTATION
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

	cubemap(const char* fileName)
	{
        
        camera = { 0 };
        camera.position = { 1.0f, 1.0f, 1.0f };    
        camera.target = { 4.0f, 1.0f, 4.0f };      
        camera.up = { 0.0f, 1.0f, 0.0f };          
        camera.fovy = 45.0f;                                
        camera.projection = CAMERA_PERSPECTIVE;             

        // Load skybox model
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
        
         SetCameraMode(camera, CAMERA_ORBITAL);
            
             
	}

    float angle = 0;

    Vector3 rotationAxis = { 0.0f, 1.0f, 0.0f };
    float rotationSpeed = 0.1f;

	void Draw()
	{
        //camera.target.x = ();
		UpdateCamera(&this->camera);
        
        angle += 0.0001;

        std::cout << "angle: " << angle << std::endl;
      
       

		BeginMode3D(this->camera);


		ClearBackground(WHITE);
		rlDisableBackfaceCulling();
		rlDisableDepthMask();

        
        
            // Rotation axis
        Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));

            // Rotate up direction around forward axis
        camera.up = Vector3RotateByAxisAngle(camera.up, forward, angle);
        

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

