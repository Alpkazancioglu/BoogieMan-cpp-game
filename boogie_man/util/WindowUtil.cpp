#include "WindowUtil.h"
#include <iostream>


Vec2<float> getWsize()
{
	Vec2<float> temp(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
	return temp;
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

float Pixel2Percent(int Pixel)
{
    return (getWsize().y * Pixel) / 1080;
}

TextureCubemap cubemap::HDRItoCubeMap(Shader shader, Texture2D panorama, int size, int format)
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

    
    /*Matrix fboViews[6] = {
        MatrixLookAt({ 0.0f, 0.0f, 0.0f }, { 1.0f,  0.0f,  0.0f },  { 0.0f, -1.0f,  0.0f }),
        MatrixLookAt({ 0.0f, 0.0f, 0.0f }, { -1.0f,  0.0f,  0.0f },  { 0.0f, -1.0f,  0.0f }),
        MatrixLookAt( { 0.0f, 0.0f, 0.0f },  { 0.0f,  1.0f,  0.0f },  { 0.0f,  0.0f,  1.0f }),
        MatrixLookAt( { 0.0f, 0.0f, 0.0f }, { 0.0f, -1.0f,  0.0f },  { 0.0f,  0.0f, -1.0f }),
        MatrixLookAt({ 0.0f, 0.0f, 0.0f },  { 0.0f,  0.0f,  1.0f },  { 0.0f, -1.0f,  0.0f }),
        MatrixLookAt( { 0.0f, 0.0f, 0.0f }, { 0.0f,  0.0f, -1.0f },  { 0.0f, -1.0f,  0.0f })
    };*/

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
