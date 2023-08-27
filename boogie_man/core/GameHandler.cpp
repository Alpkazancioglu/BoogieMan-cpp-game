#include "GameHandler.h"
#include <algorithm>
#include <unordered_map>
#include "far_background.h"
#include "killua.h"
#include "nebula.h"
#include "middle_Background.h"
#include "foreground.h"

game::game(int screenw_a, int screenh_a)
{
	 screenw = screenw_a;
	 screenh = screenh_a;
	 newecs = new ecs;
	 MainCamera = std::make_unique<Camera2D>();
	 screenrec.width =  screenw;
	 screenrec.height =  screenh;
	 screenrec.x = 0;
	 screenrec.y = 0;
}

//Baslatma
void game::initialize(const char* windowname , int fps)
{
	InitWindow(screenw , screenh , windowname);
	if (!IsWindowReady())
	{
		std::cerr << "Error Initializing the window!" << std::endl;
		CloseWindow();
	
	}
	
	SetWindowState(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

	SetTargetFPS(fps);


	/////            GAME_INIT                /////
	Vec2<int> screensize(screenw, screenh);
	BoogieManGame = std::make_unique<BoogieMan>(screensize);
	/////            ////                    /////



	target = std::make_shared<RenderTexture>();
	*target = LoadRenderTexture(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
	SetTextureFilter(target->texture,TEXTURE_FILTER_BILINEAR);


	MainCamera->target = { BoogieManGame->killua.Data.pos.x ,getWsize().y / 2 - (MainCamera->zoom * 60.0f)  };
	MainCamera->offset = { (float)GetMonitorWidth(GetCurrentMonitor()) / 2, (float)GetMonitorHeight(GetCurrentMonitor()) };
	MainCamera->rotation = 0.0f;
	MainCamera->zoom = 1.0f;

	newwatch.stopwatch();
}


void game::update()
{

	screenw = GetScreenWidth();
	screenh = GetScreenHeight();

	updatescreenrec();

	////          GAME_UPDATE             /////

	/////            ////                /////
	

	Update_Camera({ BoogieManGame->killua.Data.pos.x ,getWsize().y/2 - (MainCamera->zoom * 60.0f) },
		          { (float)getWsize().x / 2 - (BoogieManGame->killua.Data.rec.width * BoogieManGame->killua.scale / 2) , (getWsize().y / 2) - (MainCamera->zoom * 60.0f) } , {1.0f,-10.0f});
	
	game::fullscreen_g(900, 700);

	if (IsWindowMaximized())
	{
		scale = MIN((float)GetScreenWidth() / target->depth.width, ((float)GetScreenHeight() + (target->depth.height - GetScreenHeight())) / target->depth.height);
	}
	if (IsWindowFullscreen())
	{
		scale = MIN((float)GetScreenWidth() / target->depth.width, ((float)GetScreenHeight() + (target->depth.height - GetScreenHeight())) / target->depth.height);
	}
	else
	{
//		scale = MIN((float)GetScreenWidth() / target->depth.width, ((float)GetScreenHeight()) / target->depth.height);
	
		scale = MIN((float)GetScreenWidth() / getWsize().x, ((float)GetScreenHeight()) / getWsize().y);

	}

}


//Ekrana cizilicek yer
void game::draw()
{


	//Loading all the render info onto backbuffer 
	BeginTextureMode(*target);

	ClearBackground(GRAY);
	BoogieManGame->drawOffCamera();

	BeginMode2D(*MainCamera);

	////          GAME_DRAW_CAMERA             /////
	BoogieManGame->update(target.get());
	//AlpinoGame->draw(target.get());
	/////            ////                     /////
	

	EndMode2D();
	


	////      GAME_DRAW_OUT_OF_CAMERA     /////

	DrawText(TextFormat("%i :", newwatch.returnminutes()), 900, 100, 60, BLACK);
	DrawText(TextFormat(" %i", newwatch.returnseconds()), 980, 100, 60, BLACK);
	DrawText(TextFormat("FPS: %i", GetFPS()), 30, 20, 30, BLACK);

	/////            ////                /////
	

	EndTextureMode();


	//Loading to frontbuffer
	BeginDrawing();
	ClearBackground(WHITE);
	
	if (IsWindowMaximized())
	{
		DrawTexturePro(target->texture, { 0.0f, 0.0f, (float)target->texture.width, (float)-target->texture.height }, { (GetScreenWidth() - ((float)GetMonitorWidth(GetCurrentMonitor()) * scale)) * 0.5f - (target->depth.height - GetScreenHeight()), (GetScreenHeight() - ((float)GetMonitorHeight(GetCurrentMonitor()) * scale)) * 0.5f,(float)GetMonitorWidth(GetCurrentMonitor()) * scale + (2*(target->depth.height - GetScreenHeight())), (float)GetMonitorHeight(GetCurrentMonitor()) * scale  }, { 0, 0 }, 0.0f, WHITE);
	}
	else if(IsWindowFullscreen())
	{
		DrawTexturePro(target->texture, { 0.0f, 0.0f, (float)target->texture.width, (float)-target->texture.height }, { (GetScreenWidth() - ((float)GetMonitorWidth(GetCurrentMonitor()) * scale)) * 0.5f, (GetScreenHeight() - ((float)GetMonitorHeight(GetCurrentMonitor()) * scale)) * 0.5f,(float)GetMonitorWidth(GetCurrentMonitor()) * scale, (float)GetMonitorHeight(GetCurrentMonitor()) * scale + (target->depth.height - GetScreenHeight()) }, { 0, 0 }, 0.0f, WHITE);
	}
    else if(!IsWindowMaximized() && !IsWindowFullscreen())
	{
		DrawTexturePro(target->texture, { 0.0f, 0.0f, (float)target->texture.width, (float)-target->texture.height }, { (GetScreenWidth() - ((float)GetMonitorWidth(GetCurrentMonitor()) * scale)) * 0.5f, (GetScreenHeight() - ((float)GetMonitorHeight(GetCurrentMonitor()) * scale)) * 0.5f,(float)GetMonitorWidth(GetCurrentMonitor()) * scale, (float)GetMonitorHeight(GetCurrentMonitor()) * scale }, { 0, 0 }, 0.0f, WHITE);
	}

	EndDrawing();
}

void game::Clean(game* currentgame)
{
	UnloadRenderTexture(*target);
	delete newecs;
	delete currentgame;

	CloseWindow();
}
void game::Update_Camera(Vector2 target, Vector2 Offset, Vec2<float> Zoom)
{
	MainCamera->target = target;
	MainCamera->offset = Offset;
	MainCamera->zoom += ((float)GetMouseWheelMove() * 0.05f);

	if (MainCamera->zoom > Zoom.x) MainCamera->zoom = Zoom.x;
	else if (MainCamera->zoom < Zoom.y) MainCamera->zoom = Zoom.y;
}









void game::updatescreenrec()
{
	screenrec.width = screenw;
	screenrec.height = screenh;

	if (IsWindowFullscreen())
	{
		screenrec.width = GetMonitorWidth(GetCurrentMonitor());
		screenrec.height = GetMonitorHeight(GetCurrentMonitor());
	}

	screenrec.x = 0;
	screenrec.y = 0;

}

float game::entity_object_pos_calculate_x(float position, Rectangle initialrec)
{
	return (position * screenrec.width) / initialrec.width;
}

float game::entity_object_pos_calculate_y(float position, Rectangle initialrec)
{
	return (position * screenrec.height) / initialrec.height;
}








//Control(sol) + space basildiginda tam ekran yapan fonksiyon
void game::fullscreen_g(int screenw_a, int screenh_a)
{
	static int lastkey = NULL;
	static int incr = 0;

	if (!IsWindowFullscreen())
	{
		if (lastkey == NULL)
		{
			lastkey = GetKeyPressed();

		}
		if (lastkey)
		{
			incr++;

			if (incr == 20)
			{
				lastkey = NULL;
				incr = 0;
			}
		}
		if (lastkey == KEY_LEFT_CONTROL)
		{
			if (IsKeyPressed(KEY_SPACE))
			{
				lastkey = NULL;

				RestoreWindow();
				SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
				ToggleFullscreen();
				
				//SetWindowState(FLAG_FULLSCREEN_MODE);
			}
			
		}
		if (lastkey == KEY_SPACE)
		{
			if (IsKeyPressed(KEY_LEFT_CONTROL))
			{
				lastkey = NULL;
				RestoreWindow();
				SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
				ToggleFullscreen();
				
				//SetWindowState(FLAG_FULLSCREEN_MODE);
				
			}
		}
	}
	if (IsWindowFullscreen())
	{
		if (lastkey == NULL)
		{
			lastkey = GetKeyPressed();

		}
		if (!lastkey)
		{
			incr++;

			if (incr == 20)
			{
				lastkey = NULL;
				incr = 0;
			}
		}
		if (lastkey == KEY_LEFT_CONTROL)
		{
			
			if (IsKeyPressed(KEY_SPACE))
			{
				lastkey = NULL;

				ClearWindowState(FLAG_FULLSCREEN_MODE);
				SetWindowSize(screenw_a, screenh_a);
				SetWindowPosition((GetMonitorWidth(0) / 2) - (GetScreenWidth() / 2), (GetMonitorHeight(0) / 2) - (GetScreenHeight() / 2));
				
			}
			
		}
		if (lastkey == KEY_SPACE)
		{
			if (IsKeyPressed(KEY_LEFT_CONTROL))
			{
				lastkey = NULL;
				ClearWindowState(FLAG_FULLSCREEN_MODE);
				SetWindowSize(screenw_a, screenh_a);
				SetWindowPosition((GetMonitorWidth(0) / 2) - (GetScreenWidth() /2) , (GetMonitorHeight(0) / 2) - (GetScreenHeight() /2) );
				
			}
			
		}
	}
	
}



