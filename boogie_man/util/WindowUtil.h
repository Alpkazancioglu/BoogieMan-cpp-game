#pragma once

#include <raylib.h>
#include <rlgl.h>
#include "VectorMath.h"
#include <iostream>
#include <glew.h>
#include "../util/Shader.h"
#include "../../include/glm/gtc/type_ptr.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"

#include <raymath.h>
#include <string>
#include <functional>


#define PLATFORM_DESKTOP

#if defined(PLATFORM_DESKTOP)

  #define GLSL_VERSION  330

#endif

Vec2<float> getWsize();
std::string GetRelativeTexturePath(std::string textureName);

void LoadTexture2DfromHeader(Texture2D* texture, unsigned int format, unsigned int height, unsigned int width, unsigned char* data, int mipmaps);


namespace bgGL
{
    
#define BEGIN_INTERNAL_CAMERA(Camera2d) BeginMode2D(Camera2d)
#define END_INTERNAL_CAMERA EndMode2D()

    float Pixel2Percent(int Pixel);
    glm::mat4 RaylibMatrix2Mat4(Matrix matrix);
    Matrix glmMat4ToRaylibMatrix(const glm::mat4& glmMatrix);
    Vec2<float> ScreenToWorldCoord(Vec2<float> screenCoord);
    glm::mat4 CalculateCameraMatrix(Camera2D& camera);
    glm::mat4 CalculateCameraMatrix(Camera2D& camera, float ParallaxCoefficient);
    Vec2<float> FindCenterAABB(Vec4<float> rec);
    void ClearColorBufferBit(Color color);
    RenderTexture2D GetCurrentFBO();
    void SetCurrentFBOtracker(RenderTexture2D FBO);
    void BindDefaultFBO();


    void BindFBO(RenderTexture2D &fbo);

    std::vector<glm::vec3> MakeInstanceOffsetArray(int InstanceCount, Vec2<float> position, Vec2<float> offsetBetween, float scale);
    std::vector<glm::vec3> MakeInstanceOffsetArray(int InstanceCount, Vec2<float> offsetBetween, float scale, Vec2<float> *position = nullptr);
    std::vector<glm::vec3> MakeInstanceOffsetArray(int InstanceCount, Vec2<float> offsetBetween, float scale, float position_y, float* position_x = nullptr);
    std::vector<glm::vec3> MakeInstanceOffsetArray(int InstanceCount, Vec2<float> offsetBetween, std::function<float()> scale, float position_y, float* position_x = nullptr);
    std::vector<glm::vec3> MakeInstanceOffsetArray(int InstanceCount, Vec2<float> offsetBetween, std::function<float()> scale, float position_y, std::function<float()> position_x);
    

    class cubemap
    {
    public:

        Camera camera = { 0 };
        Model skybox;
        Shader CubeMapShader;
        Texture2D panorama;
        float CameraRotationSpeed;

        float RotationAngle = 0;

        RenderTexture2D SkyFbo;

        Shader SkyShaderId;

        TextureCubemap HDRItoCubeMap(Shader shader, Texture2D panorama, int size, int format);


        cubemap(const char* fileName, bool useHDR, float CameraRotationSpeed, int cubemapIslandSize)
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

            SkyFbo = LoadRenderTexture(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
            SetTextureFilter(SkyFbo.texture, TEXTURE_FILTER_BILINEAR);

            RotationAngle = 2.0f;
        }

     


        void Draw()
        {
            //UpdateCamera(&camera, CAMERA_FIRST_PERSON);
            

            //BeginTextureMode(SkyFbo);
            bgGL::BindFBO(SkyFbo);

            //ClearBackground(RAYWHITE);
            UpdateCameraPro(&this->camera, { 0,0,0 }, { 0,0,0 }, 0);
         
            BeginMode3D(this->camera);
            ClearBackground(WHITE);
            rlDisableBackfaceCulling();
            rlDisableDepthMask();

            RotationAngle += this->CameraRotationSpeed * (PI * 180) * GetFrameTime();

            Util::UseShaderProgram(skybox.materials[0].shader.id);
       
            glUniform1f(glGetUniformLocation(skybox.materials[0].shader.id, "rotationAngle"), RotationAngle);
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_LEQUAL);
            DrawModel(skybox, { 0,0,0 }, 1.0f, WHITE);
            rlEnableBackfaceCulling();
            rlEnableDepthMask();
            EndMode3D();

            //EndTextureMode();
            bgGL::BindDefaultFBO();

            glDepthFunc(GL_LESS);
            glDepthMask(GL_TRUE);

        }

        void drawFBO()
        {
            //ClearBackground(WHITE);
            bgGL::ClearColorBufferBit(WHITE);
            DrawTexturePro(SkyFbo.texture, { 0,0,(float)SkyFbo.texture.width,-(float)SkyFbo.texture.height }, { 0,0,getWsize().x,getWsize().y }, { 0,0 }, 0.0f, WHITE);
        }

        Texture* GetFBOtexture() { return &SkyFbo.texture; };

        void clear()
        {
            UnloadShader(skybox.materials[0].shader);
            UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
            UnloadTexture(panorama);
            UnloadRenderTexture(this->SkyFbo);
        }

        

    };


    class shadowmap
    {
    public:

        shadowmap(uint shadowMapWidth, uint shadowMapHeight)
        {

            glGenFramebuffers(1, &shadowMapfbo);

            this->shadowMapWidth = shadowMapWidth;
            this->shadowMapHeight = shadowMapHeight;

            glGenTextures(1, &shadowMap);
            glBindTexture(GL_TEXTURE_2D, shadowMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->shadowMapWidth, this->shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            float clampcolor[] = { 1.0f,1.0f,1.0f,1.0f };
            glTextureParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampcolor);

            glBindFramebuffer(GL_FRAMEBUFFER, shadowMapfbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);

            GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (result == GL_FRAMEBUFFER_COMPLETE)
            {
                std::cerr << "Framebuffer is complete." << "\n";

            }
            else
            {
                std::cerr << "Framebuffer is not complete." << "\n";

            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        };

        ~shadowmap()
        {
            glDeleteFramebuffers(1, &shadowMapfbo);
            glDeleteTextures(1, &shadowMap);
        };

        GLuint GetShadowMapFBO() { return this->shadowMapfbo; };
        uint GetShadowMapImage() { return this->shadowMap; };
        Vec2<uint> GetShadowMapSize() { return { this->shadowMapWidth, this->shadowMapHeight }; };
        glm::mat4 GetLightProjection() { return this->lp; };

    private:

        GLuint shadowMapfbo;
        GLuint shadowMap;
        uint shadowMapWidth;
        glm::mat4 lp;
        uint shadowMapHeight;

    };




    class InstancedTexture2D
    {
    public:

        InstancedTexture2D(int instanceCount, Texture2D& texture2draw, std::vector<glm::vec3>& positionoffsets);
        InstancedTexture2D(int instanceCount, Texture2D& texture2draw, std::vector<glm::vec3>& positionoffsets , Util::Shader &instanceShader);
        Util::Shader GetShader() { return *this->instanceShader; };
        void draw(Color tint);
        void draw(Camera2D& camera, Color tint, Texture2D SkyFBO, float ParallaxCoefficient = 2.0f);
        void draw(Camera2D& camera, Color tint,Texture2D SkyFBO,GLuint ShadowMap,  float ParallaxCoefficient = 2.0f);
        void drawShadowMap(Camera2D& camera, glm::vec3 LightPos, float ParallaxCoefficient = 2.0f);
        void clean();

    private:

        Texture2D* texture;
        std::unique_ptr<Util::Shader> instanceShader;
        std::unique_ptr<Util::Shader> ShadowMapShader;
        GLuint vbo, vao;
        std::vector<glm::vec3> offsets;
        int OffsetBufferIndex;
        int instanceAmount;

        //Store the inputs so send them to gpu only when they are changed
        Color tint;
        float ParallaxCoefficient;

        glm::vec3 LightPosition;
        glm::mat4 LightProjection;
    };

}
