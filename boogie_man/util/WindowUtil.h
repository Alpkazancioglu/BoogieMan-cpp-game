#pragma once

#include <raylib.h>
#include <rlgl.h>
#include "VectorMath.h"
#include <iostream>
//#include <glew.h>
//#include "../rcamera.h"
#include <glew.h>
#include "../util/Shader.h"
#include "../../include/glm/gtc/type_ptr.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"


#include <raymath.h>
#include <string>

#define PLATFORM_DESKTOP

#if defined(PLATFORM_DESKTOP)

  #define GLSL_VERSION  330

#endif


Vec2<float> getWsize();
std::string GetRelativeTexturePath(std::string textureName);
float Pixel2Percent(int Pixel);
glm::mat4 RaylibMatrix2Mat4(Matrix matrix);
Matrix glmMat4ToRaylibMatrix(const glm::mat4& glmMatrix);
Vec2<float> ScreenToWorldCoord(Vec2<float> screenCoord);
glm::mat4 CalculateCameraMatrix(Camera2D& camera);

class cubemap
{
public:

	Camera camera = { 0 };
	Model skybox;
	Shader CubeMapShader;
    Texture2D panorama;
    float CameraRotationSpeed;


    TextureCubemap HDRItoCubeMap(Shader shader, Texture2D panorama, int size, int format);


	cubemap(const char* fileName , bool useHDR , float CameraRotationSpeed , int cubemapIslandSize)
	{
        this->CameraRotationSpeed = CameraRotationSpeed;

        camera = { 0 };
        camera.position = { 1.0f, 1.0f, 1.0f };    
        camera.target = { 4.0f, 1.0f, 4.0f };      
        camera.up = { 0.0f, 1.0f, 0.0f };          
        camera.fovy = 70.0f;                                
        camera.projection = CAMERA_PERSPECTIVE;             

        Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
        skybox = LoadModelFromMesh(cube);

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

        if (useHDR)
        {
            panorama = LoadTexture(fileName);
            skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = HDRItoCubeMap(CubeMapShader, panorama, cubemapIslandSize, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        }
        else
        {
            Image img = LoadImage(fileName);
            skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);    
            UnloadImage(img);
        }

        
        
    }

    

	void Draw()
	{
        //UpdateCamera(&camera, CAMERA_FIRST_PERSON);
		UpdateCameraPro(&this->camera,{0,0,0},{0,0,0},0);
        ClearBackground(RAYWHITE);

		BeginMode3D(this->camera);
		//ClearBackground(WHITE);
		rlDisableBackfaceCulling();
		rlDisableDepthMask();
        camera.target = Vector3RotateByAxisAngle(camera.target, {0.0f,1.0f,0.0f}, CameraRotationSpeed * (PI * 180) * GetFrameTime());
        DrawModel(skybox, { 0,0,0 },1.0f, WHITE);
		rlEnableBackfaceCulling();
		rlEnableDepthMask();
		EndMode3D();

        


	}

	void clear()
	{
		UnloadShader(skybox.materials[0].shader);
		UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
        UnloadTexture(panorama);

	}

};


class InstancedTexture2D
{
public:

    InstancedTexture2D(int instanceCount, Texture2D &texture2draw , std::vector<glm::vec3> &positionoffsets);
    void draw(Color tint);
    void draw(Camera2D& camera, Color tint);
    void clean();

private:

    Texture2D* texture;
    std::unique_ptr<Util::Shader> instanceShader;
    GLuint vbo, vao;
    std::vector<glm::vec3> offsets;
    int OffsetBufferIndex;
    int instanceAmount;

};

