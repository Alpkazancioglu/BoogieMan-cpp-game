#include "WindowUtil.h"
#include <iostream>
#include "VectorMath.h"


Vec2<float> getWsize()
{
    return Vec2<float>(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
}

std::string GetRelativeTexturePath(std::string textureName)
{
	std::string WorkingDir(GetWorkingDirectory());

	for (size_t i = 0; i < WorkingDir.size(); i++)
	{
		if (WorkingDir.at(i) == '\\') {
			WorkingDir.at(i) = '/';
		}
	}

	std::cout << "WORKING DIR: " << (WorkingDir + "/textures/" + textureName).c_str() << std::endl;

	return (WorkingDir + "/textures/" + textureName);
}

void LoadTexture2DfromHeader(Texture2D* texture, unsigned int format, unsigned int height, unsigned int width, unsigned char* data, int mipmaps)
{
    Image image = { 0 };
    image.format = format;
    image.height = height;
    image.width = width;
    image.data = data;
    image.mipmaps = mipmaps;
    *texture = LoadTextureFromImage(image);
}

float bgGL::Pixel2Percent(int Pixel)
{
    return (getWsize().y * Pixel) / 1080;
}

glm::mat4 bgGL::RaylibMatrix2Mat4(Matrix matrix)
{
    glm::mat4 finalMat(1.0f);
    
    finalMat[0][0] = matrix.m0;
    finalMat[0][1] = matrix.m1;
    finalMat[0][2] = matrix.m2;
    finalMat[0][3] = matrix.m3;
    finalMat[1][0] = matrix.m4;
    finalMat[1][1] = matrix.m5;
    finalMat[1][2] = matrix.m6;
    finalMat[1][3] = matrix.m7;
    finalMat[2][0] = matrix.m8;
    finalMat[2][1] = matrix.m9;
    finalMat[2][2] = matrix.m10;
    finalMat[2][3] = matrix.m11;
    finalMat[3][0] = matrix.m12;
    finalMat[3][1] = matrix.m13;
    finalMat[3][2] = matrix.m14;
    finalMat[3][3] = matrix.m15;

    return finalMat;
}

Matrix bgGL::glmMat4ToRaylibMatrix(const glm::mat4& glmMatrix)
{
    Matrix raylibMatrix;

    raylibMatrix.m0 = glmMatrix[0][0];
    raylibMatrix.m1 = glmMatrix[1][0];
    raylibMatrix.m2 = glmMatrix[2][0];
    raylibMatrix.m3 = glmMatrix[3][0];

    raylibMatrix.m4 = glmMatrix[0][1];
    raylibMatrix.m5 = glmMatrix[1][1];
    raylibMatrix.m6 = glmMatrix[2][1];
    raylibMatrix.m7 = glmMatrix[3][1];

    raylibMatrix.m8 = glmMatrix[0][2];
    raylibMatrix.m9 = glmMatrix[1][2];
    raylibMatrix.m10 = glmMatrix[2][2];
    raylibMatrix.m11 = glmMatrix[3][2];

    raylibMatrix.m12 = glmMatrix[0][3];
    raylibMatrix.m13 = glmMatrix[1][3];
    raylibMatrix.m14 = glmMatrix[2][3];
    raylibMatrix.m15 = glmMatrix[3][3];

    return raylibMatrix;
}

Vec2<float> bgGL::ScreenToWorldCoord(Vec2<float> screenCoord)
{
    return screenCoord / getWsize();
}

std::vector<glm::vec3> bgGL::MakeInstanceOffsetArray(int InstanceCount, Vec2<float> position, Vec2<float> offsetBetween, float scale)
{

    std::vector<glm::vec3> offsets;
    offsets.resize(InstanceCount);

    for (int instanceID = 0; instanceID < InstanceCount; instanceID += 1)
    {
        glm::vec3 translation;
        translation.x = position.x + (instanceID * offsetBetween.x);
        translation.y = position.y + (instanceID * offsetBetween.y);
        translation.z = scale;
        offsets[instanceID] = translation;
    }

    return offsets;
}

std::vector<glm::vec3> bgGL::MakeInstanceOffsetArray(int InstanceCount, Vec2<float> offsetBetween, float scale, Vec2<float>* position)
{
    bool defaultState = false;
    if (position == nullptr)
    {
        position = new Vec2<float>;
        defaultState = true;
    }
   
    std::vector<glm::vec3> offsets;
    offsets.resize(InstanceCount);

    for (int instanceID = 0; instanceID < InstanceCount; instanceID += 1)
    {
        if (defaultState)
        {
            position->SetValues(instanceID, instanceID);
        }

        glm::vec3 translation;
        translation.x = position->x + (instanceID * offsetBetween.x);
        translation.y = position->y + (instanceID * offsetBetween.y);
        translation.z = scale;
        offsets[instanceID] = translation;
    }

    if (defaultState)
    {
        delete position;
    }

    return offsets;
}


std::vector<glm::vec3> bgGL::MakeInstanceOffsetArray(int InstanceCount, Vec2<float> offsetBetween, float scale, float position_y, float* position_x)
{
    bool defaultState = false;
    if (position_x == nullptr)
    {
        position_x = new float;
        defaultState = true;
    }

    std::vector<glm::vec3> offsets;
    offsets.resize(InstanceCount);

    for (int instanceID = 0; instanceID < InstanceCount; instanceID += 1)
    {
        if (defaultState)
        {
            *position_x = instanceID;
        }

        glm::vec3 translation;
        translation.x = *position_x + (instanceID * offsetBetween.x);
        translation.y = position_y + (instanceID * offsetBetween.y);
        translation.z = scale;
        offsets[instanceID] = translation;
    }

    if (defaultState)
    {
        delete position_x;
    }

    return offsets;
}

std::vector<glm::vec3> bgGL::MakeInstanceOffsetArray(int InstanceCount, Vec2<float> offsetBetween, std::function<float()> scale, float position_y, float* position_x)
{
    bool defaultState = false;
    if (position_x == nullptr)
    {
        position_x = new float;
        defaultState = true;
    }

    std::vector<glm::vec3> offsets;
    offsets.resize(InstanceCount);

    for (int instanceID = 0; instanceID < InstanceCount; instanceID += 1)
    {
        if (defaultState)
        {
            *position_x = instanceID;
        }

        glm::vec3 translation;
        translation.x = *position_x + (instanceID * offsetBetween.x);
        translation.y = position_y + (instanceID * offsetBetween.y);
        translation.z = scale();
        offsets[instanceID] = translation;
    }

    if (defaultState)
    {
        delete position_x;
    }

    return offsets;
}

std::vector<glm::vec3> MakeInstanceOffsetArray(int InstanceCount, Vec2<float> offsetBetween, std::function<float()> scale, float position_y, std::function<float()> position_x)
{
    std::vector<glm::vec3> offsets;
    offsets.resize(InstanceCount);

    for (int instanceID = 0; instanceID < InstanceCount; instanceID += 1)
    {
        glm::vec3 translation;
        translation.x = position_x() + (instanceID * offsetBetween.x);
        translation.y = position_y + (instanceID * offsetBetween.y);
        translation.z = scale();
        offsets[instanceID] = translation;
    }
    return offsets;
}





TextureCubemap bgGL::cubemap::HDRItoCubeMap(Shader shader, Texture2D panorama, int size, int format)
{
    TextureCubemap cubemap = { 0 };

    rlDisableBackfaceCulling();     

   
    unsigned int rbo = rlLoadTextureDepth(size, size, true);
    cubemap.id = rlLoadTextureCubemap(0, size, format);

    unsigned int fbo = rlLoadFramebuffer(size, size);
    rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
    rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);

    if (rlFramebufferComplete(fbo)) TraceLog(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", fbo);
    
    rlEnableShader(shader.id);

   
    Matrix matFboProjection = MatrixPerspective(90.0 * DEG2RAD, 1.0, RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);
    rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matFboProjection);

    Matrix fboViews[6] = {
          MatrixLookAt({ 0.0f, 0.0f, 0.0f }, { -1.0f,  0.0f,  0.0f },  { 0.0f, 1.0f,  0.0f }),
          MatrixLookAt({ 0.0f, 0.0f, 0.0f }, { 1.0f,  0.0f,  0.0f },  { 0.0f, 1.0f,  0.0f }),
          MatrixLookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, -1.0f,  0.0f },  { 0.0f,  0.0f, 1.0f }),
          MatrixLookAt({ 0.0f, 0.0f, 0.0f },  { 0.0f,  1.0f,  0.0f },  { 0.0f,  0.0f,  -1.0f }),
          MatrixLookAt({ 0.0f, 0.0f, 0.0f },  { 0.0f,  0.0f,  1.0f },  { 0.0f, 1.0f,  0.0f }),
          MatrixLookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f,  0.0f, -1.0f },  { 0.0f, 1.0f,  0.0f })
    };

    rlViewport(0, 0, size, size);  

    
    rlActiveTextureSlot(0);
    rlEnableTexture(panorama.id);

    for (int i = 0; i < 6; i++)
    {
      
        rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], fboViews[i]);

        
        rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X + i, 0);
        rlEnableFramebuffer(fbo);

        
        rlClearScreenBuffers();
        rlLoadDrawCube();

        
    }
   
    rlDisableShader();          
    rlDisableTexture();         
    rlDisableFramebuffer();     
    rlUnloadFramebuffer(fbo);   

    
    rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());
    rlEnableBackfaceCulling();
   

    cubemap.width = size;
    cubemap.height = size;
    cubemap.mipmaps = 1;
    cubemap.format = format;

    return cubemap;
}


bgGL::InstancedTexture2D::InstancedTexture2D(int instanceCount , Texture2D &texture2draw, std::vector<glm::vec3> &positionoffsets)
{

    instanceShader = std::make_unique<Util::Shader>(GetRelativeTexturePath("shaders/Basic.vs").c_str(), GetRelativeTexturePath("shaders/Basic.fs").c_str());
    ShadowMapShader = std::make_unique<Util::Shader>(GetRelativeTexturePath("shaders/ShadowMap.vs").c_str(), GetRelativeTexturePath("shaders/ShadowMap.fs").c_str());
    
    //this->instanceShader = instanceShader_i;
    //this->ShadowMapShader = ShadowMapShader_i;

    float vertices[] = {
        // Position (X, Y, Z)    Normal (NX, NY, NZ)    Texture Coordinates (U, V)
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,     0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,     1.0f, 0.0f, // Bottom-right
         0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,     1.0f, -1.0f, // Top-right
        -0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,     0.0f, -1.0f, // Top-left
    };


    texture = &texture2draw;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

 
    offsets.assign(positionoffsets.begin(), positionoffsets.end());

    offsets.resize(500);
    
    for (size_t i = positionoffsets.size(); i < 500; i++)
    {
        offsets[i] = { 0,0 , 0 };
    }

   /* std::cout << "Offsets :";

    for (size_t i = 0; i < offsets.size(); i++)
    {
        std::cout << "(" << offsets[i].x << " , " << offsets[i].y << " , " << offsets[i].z<< ")";
    }

    std::cout << "\n";*/

    instanceAmount = instanceCount;

    Util::UseShaderProgram(instanceShader->GetID());

    float aspect_ratio_hw = (float)texture->height / getWsize().y;
    float aspect_ratio_wh = (float)texture->width / getWsize().x;

    glm::mat4 ImageScaleRatioMat(1.0f);
    //ImageScaleRatioMat = glm::scale(ImageScaleRatioMat, glm::vec3(3.0f, 3.0f, 3.0f));
    ImageScaleRatioMat = glm::scale(ImageScaleRatioMat, glm::vec3(aspect_ratio_wh, aspect_ratio_hw, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(this->instanceShader->GetID(), "ratioMat"), 1, GL_FALSE, glm::value_ptr(ImageScaleRatioMat));
    glUniform3fv(glGetUniformLocation(instanceShader->GetID(), "offsets"), 500, (GLfloat*)&offsets[0]);
    
    Util::UseShaderProgram(ShadowMapShader->GetID());

    glUniformMatrix4fv(glGetUniformLocation(this->ShadowMapShader->GetID(), "ratioMat"), 1, GL_FALSE, glm::value_ptr(ImageScaleRatioMat));
    glUniform3fv(glGetUniformLocation(ShadowMapShader->GetID(), "offsets"), 500, (GLfloat*)&offsets[0]);

    Util::UseShaderProgram(0);

    

}

bgGL::InstancedTexture2D::InstancedTexture2D(int instanceCount, Texture2D& texture2draw, std::vector<glm::vec3>& positionoffsets, Util::Shader &instanceShader)
{

    *this->instanceShader = instanceShader;

    float vertices[] = {
    -0.5f, -0.5f,0.0f, 0.0f, 0.0f,
     0.5f, -0.5f,0.0f, -1.0f, 0.0f,
     0.5f,  0.5f,0.0f, -1.0f, -1.0f,
    -0.5f,  0.5f,0.0f, 0.0f, -1.0f
    };

    texture = &texture2draw;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    offsets.assign(positionoffsets.begin(), positionoffsets.end());
    offsets.resize(500);

    for (size_t i = positionoffsets.size(); i < 500; i++)
    {
        offsets[i] = { 0,0 , 0 };
    }

    std::cout << "Offsets :";

    for (size_t i = 0; i < offsets.size(); i++)
    {
        std::cout << "(" << offsets[i].x << " , " << offsets[i].y << " , " << offsets[i].z << ")";
    }

    std::cout << "\n";

    instanceAmount = instanceCount;

    Util::UseShaderProgram(this->instanceShader->GetID());

    float aspect_ratio_hw = (float)texture->height / getWsize().y;
    float aspect_ratio_wh = (float)texture->width / getWsize().x;

    glm::mat4 ImageScaleRatioMat(1.0f);
    ImageScaleRatioMat = glm::scale(ImageScaleRatioMat, glm::vec3(aspect_ratio_wh, aspect_ratio_hw, 1.0f));
    ImageScaleRatioMat = glm::scale(ImageScaleRatioMat, glm::vec3(3.0f, 3.0f, 3.0f));
    glUniformMatrix4fv(glGetUniformLocation(this->instanceShader->GetID(), "ratioMat"), 1, GL_FALSE, glm::value_ptr(ImageScaleRatioMat));
    glUniform3fv(glGetUniformLocation(this->instanceShader->GetID(), "offsets"), 500, (GLfloat*)&offsets[0]);

    Util::UseShaderProgram(0);

}

void bgGL::InstancedTexture2D::draw(Color tint)
{
    glUseProgram(instanceShader->GetID());

    Vector4 tintColor = ColorNormalize(tint);

    glUniform4f(glGetUniformLocation(instanceShader->GetID(), "tint"), tintColor.x, tintColor.y, tintColor.z, tintColor.w);

    glm::mat4 projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    glUniformMatrix4fv(glGetUniformLocation(instanceShader->GetID(), "cameraMat"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform1i(glGetUniformLocation(instanceShader->GetID(), "texture0"), GL_TEXTURE0);


    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, instanceAmount);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(0);

    glUseProgram(0);
}

void bgGL::InstancedTexture2D::draw(Camera2D& camera, Color tint, Texture2D SkyFBO, float ParallaxCoefficient)
{
    glUseProgram(instanceShader->GetID());

    Vector4 tintColor = ColorNormalize(tint);

    glUniform4f(glGetUniformLocation(instanceShader->GetID(), "tint"), tintColor.x, tintColor.y, tintColor.z, tintColor.w);

    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, ParallaxCoefficient / 10));
    modelMat = glm::scale(modelMat, glm::vec3(3.0f, 3.0f, 3.0f));

    glUniformMatrix4fv(glGetUniformLocation(this->instanceShader->GetID(), "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));

    glm::mat4 projectionMatrix = CalculateCameraMatrix(camera, ParallaxCoefficient);

    glUniformMatrix4fv(glGetUniformLocation(instanceShader->GetID(), "cameraMat"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform1i(glGetUniformLocation(instanceShader->GetID(), "texture0"), GL_TEXTURE0);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, SkyFBO.id);

    glGenerateMipmap(GL_TEXTURE_2D);
    glUniform1i(glGetUniformLocation(instanceShader->GetID(), "skytexture"), 1);

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(glGetUniformLocation(instanceShader->GetID(), "shadowMap"), 2);

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, instanceAmount);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(0);
    glUseProgram(0);
}

void bgGL::InstancedTexture2D::draw(Camera2D& camera , Color tint, Texture2D SkyFBO  ,GLuint ShadowMap, float ParallaxCoefficient)
{    
    glUseProgram(instanceShader->GetID());

    if (this->tint.r != tint.r && this->tint.g != tint.g && this->tint.b != tint.b)
    {
        this->tint = tint;

        Vector4 tintColor = ColorNormalize(tint);
        glUniform4f(glGetUniformLocation(instanceShader->GetID(), "tint"), tintColor.x, tintColor.y, tintColor.z, tintColor.w);
    }
    if (this->ParallaxCoefficient != ParallaxCoefficient)
    {
        this->ParallaxCoefficient = ParallaxCoefficient;

        //modelMat = glm::scale(modelMat, glm::vec3(3.0f, 3.0f, 3.0f));
        glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, ParallaxCoefficient ));
        glUniformMatrix4fv(glGetUniformLocation(this->instanceShader->GetID(), "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
    }
   
    glm::mat4 projectionMatrix = CalculateCameraMatrix(camera , ParallaxCoefficient);
    
    glUniformMatrix4fv(glGetUniformLocation(instanceShader->GetID(), "cameraMat"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glUniform3f(glGetUniformLocation(instanceShader->GetID(), "LightPosition"), LightPosition.x, LightPosition.y, LightPosition.z);

    glUniformMatrix4fv(glGetUniformLocation(instanceShader->GetID(), "LightProjection"), 1, GL_FALSE, glm::value_ptr(LightProjection));


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform1i(glGetUniformLocation(instanceShader->GetID(), "texture0"), GL_TEXTURE0);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, SkyFBO.id);
    
    glGenerateMipmap(GL_TEXTURE_2D); 
    glUniform1i(glGetUniformLocation(instanceShader->GetID(), "skytexture"), 1);

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, ShadowMap);
    glUniform1i(glGetUniformLocation(instanceShader->GetID(), "shadowMap"), 2);

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, instanceAmount);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(0);
    glUseProgram(0);

}

void bgGL::InstancedTexture2D::drawShadowMap(Camera2D& camera, glm::vec3 LightPos, float ParallaxCoefficient)
{

    glUseProgram(ShadowMapShader->GetID());


    //glm::mat4 projectionMatrix = CalculateCameraMatrix(camera , ParallaxCoefficient);
    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, ParallaxCoefficient));
    
    glUniformMatrix4fv(glGetUniformLocation(this->ShadowMapShader->GetID(), "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));

    

    float zoom = camera.zoom;
    //std::cout << "Camera: " << camera3d.target.x << " " << camera3d.target.y << " " << camera3d.target.z << std::endl;


    //glm::mat4 LightProj = glm::lookAt(glm::vec3(camera3d.position.x, camera3d.position.y, camera3d.position.z), glm::vec3(camera3d.position.x + camera3d.target.x, camera3d.position.y + camera3d.target.y, camera3d.position.z + camera3d.target.z), glm::vec3(0.0, 1.0f, 0.0f));
    //glm::mat4 LightProj = glm::perspective(glm::radians(45.0f), getWsize().x / getWsize().y, -10.0f, 10.0f);
    glm::mat4 projectionMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -5.0f, 5.0f);
    //glm::mat4 projectionMatrix = CalculateCameraMatrix(camera, ParallaxCoefficient);
    
    glm::vec3 CameraTarget(camera.target.x / getWsize().x , camera.target.y / getWsize().y, 0.0f);
    CameraTarget = CameraTarget * 2.0f;
    //glm::vec3 LightPos(0.0, 0.3f, -0.2f);
    LightProjection = glm::lookAt(LightPos + CameraTarget, CameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));

    this->LightPosition = LightPos;
    //glm::mat4 LightProj = glm::lookAt(LightPosition + glm::vec3(camera.target.x / getWsize().x, 0.0f, 0.0f), glm::vec3(camera.target.x / getWsize().x, 0, 0), glm::vec3(0.0, 1.0f, 0.0f));
   
    LightProjection = projectionMatrix * LightProjection;

    glUniformMatrix4fv(glGetUniformLocation(ShadowMapShader->GetID(), "LightProjection"), 1, GL_FALSE, glm::value_ptr(LightProjection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform1i(glGetUniformLocation(ShadowMapShader->GetID(), "texture0"), GL_TEXTURE0);

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, instanceAmount);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(0);
    glUseProgram(0);

}

void bgGL::InstancedTexture2D::clean()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    Util::DeleteShaderProgram(instanceShader->GetID());
    Util::DeleteShaderProgram(this->ShadowMapShader->GetID());
}

glm::mat4 bgGL::CalculateCameraMatrix(Camera2D& camera)
{
    float zoom = camera.zoom;
    Vec2<float> offset(camera.offset.x * 2, camera.offset.y * 2);
    offset(ScreenToWorldCoord(offset));
    Vec2<float> target(((camera.target.x) / getWsize().x) * zoom * 2, ((camera.target.y / getWsize().y) * zoom * 2));
    glm::mat4 projectionMatrix = glm::ortho((-1.0f + target.x) / zoom, (1.0f + target.x) / zoom, (-1.0f + target.y) / zoom, (1.0f + target.y) / zoom, -1.0f, 1.0f);
    return projectionMatrix;
}

glm::mat4 bgGL::CalculateCameraMatrix(Camera2D& camera, float ParallaxCoefficient)
{
    float zoom = camera.zoom;
    Vec2<float> target(((camera.target.x) / getWsize().x) * zoom * ParallaxCoefficient, ((camera.target.y / getWsize().y) * zoom * ParallaxCoefficient));
    glm::mat4 projectionMatrix = glm::ortho((-1.0f + target.x) / zoom, (1.0f + target.x) / zoom, (-1.0f + target.y) / zoom, (1.0f + target.y) / zoom, -5.0f,5.0f);
    return projectionMatrix;

}

Vec2<float> bgGL::FindCenterAABB(Vec4<float> rec)
{
    return Vec2<float>(rec.x + (rec.z/2) , rec.y + (rec.w/2));
}

void bgGL::ClearColorBufferBit(Color color)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(color.r, color.g, color.b, color.a);
}



static RenderTexture2D CurrentBindFBO;

RenderTexture2D bgGL::GetCurrentFBO()
{
    return CurrentBindFBO;
}

void bgGL::SetCurrentFBOtracker(RenderTexture2D FBO)
{
    CurrentBindFBO = FBO;
}

//Load a texture from a header file consists of an image byte array
void bgGL::LoadTexture2DfromHeader(Texture2D* texture, unsigned int format, unsigned int height, unsigned int width, unsigned char* data, int mipmaps)
{
    Image image = { 0 };
    image.format = format;
    image.height = height;
    image.width = width;
    image.data = data;
    image.mipmaps = mipmaps;
    *texture = LoadTextureFromImage(image);
}

void bgGL::BindFBO(RenderTexture2D &fbo)
{
    BeginTextureMode(fbo);
    bgGL::SetCurrentFBOtracker(fbo);
}

void bgGL::BindDefaultFBO()
{
    EndTextureMode();
    RenderTexture2D fbo;
    fbo.id = 0;
    bgGL::SetCurrentFBOtracker(fbo);
}

