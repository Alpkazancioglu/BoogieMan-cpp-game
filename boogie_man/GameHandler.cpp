#include "GameHandler.h"
#include "ECS.h"
#include "FileHandler.h"
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "Alpino/far_background.h"
#include "Alpino/killua.h"
#include "Alpino/nebula.h"
#include "Alpino/game.h"
#include "Alpino/middle_Background.h"
#include "Alpino/foreground.h"
#include "StopWatch.h"
#include <vector>
#include <memory>


ecs *newecs;
std::unique_ptr<Alpino> AlpinoGame;
RenderTexture2D* target;
float scale;
stopwatch_t newwatch;
static int newrandomnum = 0;

game::game(int screenw_a, int screenh_a)
{
	 screenw = screenw_a;
	 screenh = screenh_a;
	 MainCamera = new Camera2D;
	 newecs = new ecs;
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
	AlpinoGame = std::make_unique<Alpino>(screensize);
	/////            ////                    /////



	target = new RenderTexture;
	*target = LoadRenderTexture(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
	SetTextureFilter(target->texture,TEXTURE_FILTER_BILINEAR);

	MainCamera->target = { 0,0 };
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
	
	//Update_Camera({ AlpinoGame->killuaData.pos.x  + (AlpinoGame->killuaData.rec.width/2),AlpinoGame->killuaData.pos.y });
    //Update_Camera({ 0,AlpinoGame->killuaData.pos.y });
	//Update_Camera({ (float)getWsize().x / 2 ,AlpinoGame->killuaData.pos.y });
	Update_Camera({ getWsize().x / 2 ,getWsize().y/2 - (MainCamera->zoom * 60.0f) });
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
		scale = MIN((float)GetScreenWidth() / target->depth.width, ((float)GetScreenHeight()) / target->depth.height);
	}

}


//Ekrana cizilicek yer
void game::draw()
{

	//Loading all the render info onto backbuffer 
	BeginTextureMode(*target);

	ClearBackground(GRAY);

	BeginMode2D(*MainCamera);

	////          GAME_DRAW_CAMERA             /////
	AlpinoGame->update(target);
	AlpinoGame->draw(target);
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
	if(IsWindowFullscreen())
	{
		DrawTexturePro(target->texture, { 0.0f, 0.0f, (float)target->texture.width, (float)-target->texture.height }, { (GetScreenWidth() - ((float)GetMonitorWidth(GetCurrentMonitor()) * scale)) * 0.5f, (GetScreenHeight() - ((float)GetMonitorHeight(GetCurrentMonitor()) * scale)) * 0.5f,(float)GetMonitorWidth(GetCurrentMonitor()) * scale, (float)GetMonitorHeight(GetCurrentMonitor()) * scale + (target->depth.height - GetScreenHeight()) }, { 0, 0 }, 0.0f, WHITE);
	}
	if(!IsWindowMaximized() && !IsWindowFullscreen())
	{
		DrawTexturePro(target->texture, { 0.0f, 0.0f, (float)target->texture.width, (float)-target->texture.height }, { (GetScreenWidth() - ((float)GetMonitorWidth(GetCurrentMonitor()) * scale)) * 0.5f, (GetScreenHeight() - ((float)GetMonitorHeight(GetCurrentMonitor()) * scale)) * 0.5f,(float)GetMonitorWidth(GetCurrentMonitor()) * scale, (float)GetMonitorHeight(GetCurrentMonitor()) * scale }, { 0, 0 }, 0.0f, WHITE);
	}

	EndDrawing();
}

void game::Clean(game* currentgame)
{
	delete MainCamera;
	UnloadRenderTexture(*target);
	delete target;
	delete newecs;
	delete currentgame;

	CloseWindow();
}
void game::Update_Camera(Vector2 target)
{
	
	MainCamera->target = target;
	//MainCamera->offset = { (float)getWsize().x /2 , (float)GetMonitorHeight(GetCurrentMonitor()) - AlpinoGame->killua.height - 3 - (MainCamera->zoom * 60.0f)};
	MainCamera->offset = { (float)getWsize().x / 2 , (getWsize().y / 2) - (MainCamera->zoom * 60.0f) };
	MainCamera->zoom += ((float)GetMouseWheelMove() * 0.05f);

	if (MainCamera->zoom > 1.3f) MainCamera->zoom = 1.3f;
	else if (MainCamera->zoom < 1.0f) MainCamera->zoom = 1.0f;
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

int GiveRandomNumf(int min, int max, int sizeofarray, bool exclude_on_off, int numtoexclude) {

	static int timesthisfunctioncalled = 0;
	int* randarray = new int[sizeofarray];
	*randarray = NULL;
	int finalresult;

	srand((unsigned)time(NULL));
	if (min < max)
	{
		for (size_t i = 0; i < sizeofarray; i++)
		{
			randarray[i] = rand() % max;

			if (randarray[i] < min)
			{
				i--;
			}
			if (exclude_on_off == true)
			{
				if (randarray[i] == numtoexclude)
				{

					i--;

				}

			}

		}
	}

	timesthisfunctioncalled++;

	if (timesthisfunctioncalled > sizeofarray - 1)
	{
		timesthisfunctioncalled = (timesthisfunctioncalled % (sizeofarray - 1));
	}

	finalresult = randarray[timesthisfunctioncalled];
	delete[] randarray;
	return finalresult;


}


