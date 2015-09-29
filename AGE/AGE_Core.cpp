#include "AGE.h"
#include "AGE_Graphics.h"
//#include "AGE_Input.h"

using namespace std;

SDL_Renderer *gRenderer = NULL;

bool AGE::Init(const char* windowTitle,int screenWidth, int screenHeight, bool vSync)
{
	bool success = true;
	VSynced = vSync;
	SetMaxFPS(60);
	
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			cout << "Warning: Linear texture filtering not enabled!" << endl;
		}

		gWindow = SDL_CreateWindow( windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN |SDL_WINDOW_FULLSCREEN);
		windowRect = new AGE_Rect(0,0,screenWidth,screenHeight);

		if( gWindow == NULL )
		{
			cout << "gWindow could not be created! SDL Error: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			if(VSynced)
			{
				gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			}
			else
			{
				gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);				
			}

			if( gRenderer == NULL )
			{
				cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
					success = false;
				}

				if(TTF_Init() == -1)
				{
					cout << "SDL_ttf init error" << endl;
					success = false;
				}

				// camera_age.Offset.X = 0;
				// camera_age.Offset.Y = 0;
				
				AGE_Vector zeroV(0.0f, 0.0f);
				// AGE_SetCameraTransform(zeroV);
				AGE_ViewRect.SetWidth(windowRect->GetWidth());
				AGE_ViewRect.SetHeight(windowRect->GetHeight());
			}
		}
	}

	return success;
}

void AGE::Exit()
{
	delete windowRect;
	quit = true;
}

void AGE::SetWindowSize(int Width,int Height)
{
	SDL_SetWindowSize(gWindow, Width, Height);
}

void AGE::Run(EventHandle_age eventHandler, UserUpdate_age userUpdate, UserDraw_age userDraw)
{
	AGE_Timer fpsTimer;
	AGE_Timer capTimer;
	fpsTimer.Start();

	quit = false;
	int countedFrames = 0;	

	SDL_Event e;

	while( !quit )
	{
		if(!VSynced)
		{
			capTimer.Start();
		}

		//keyboardUpdate_age();

		while(SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}

			eventHandler(&e);
			//mouseHandleEvent_age(&e);
		}

		current_fps = countedFrames / (fpsTimer.GetTicks() / 1000.f);

		userUpdate();
		lastUpdate = SDL_GetTicks();
		userDraw();

		++countedFrames;

		if(!VSynced)
		{
			int frameTicks = capTimer.GetTicks();

	        if( frameTicks < MAX_TICKS_PER_FRAME )
	        {
	            //Wait remaining time
	            SDL_Delay( MAX_TICKS_PER_FRAME - frameTicks );
	        }
    	}    	    	
	}
}

void AGE::SetMaxFPS(int maxFps)
{
	MAX_FPS = maxFps;

	if(MAX_FPS>1000)
	{
		MAX_FPS = 1000;
	}

	MAX_TICKS_PER_FRAME = 1000/MAX_FPS;
}

double AGE::DeltaSecondsGet()
{
	Uint32 milliSeconds = DeltaMilliSecondsGet();
	double result = milliSeconds / 1000.f;
	return result;
}

Uint32 AGE::DeltaMilliSecondsGet()
{
	Uint32 curr = SDL_GetTicks();
	Uint32 result = curr - lastUpdate;

	if(result<1)
	{
		result = 1;
	}

	return result;
}

void AGE::Close()
{
	//Destroy gWindow
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	
	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void AGE::FullScreenBorderless()
{
	SDL_Rect oldBounds = {0,0, windowRect->GetWidth(), windowRect->GetHeight()};

    if(SDL_GetWindowFlags(gWindow) & SDL_WINDOW_BORDERLESS)
    {
        SDL_SetWindowBordered(gWindow, SDL_TRUE);
        SDL_SetWindowSize(gWindow, oldBounds.w, oldBounds.h);
        SDL_SetWindowPosition(gWindow, oldBounds.x, oldBounds.y);

        windowRect->SetWidth(oldBounds.w);
		windowRect->SetHeight(oldBounds.h);
		AGE_ViewRect.SetWidth(windowRect->GetWidth());
		AGE_ViewRect.SetHeight(windowRect->GetHeight());

        //return oldBounds;
    }
    else
    {
        SDL_Rect curBounds;
        SDL_GetWindowPosition(gWindow, &curBounds.x, &curBounds.y);
        SDL_GetWindowSize(gWindow, &curBounds.w, &curBounds.h);

        int idx = SDL_GetWindowDisplayIndex(gWindow);
        SDL_Rect bounds;
        SDL_GetDisplayBounds(idx, &bounds);
        SDL_SetWindowBordered(gWindow, SDL_FALSE);
        SDL_SetWindowPosition(gWindow, bounds.x, bounds.y);
        SDL_SetWindowSize(gWindow, bounds.w, bounds.h);

		windowRect->SetWidth(curBounds.w);
		windowRect->SetHeight(curBounds.h);
		AGE_ViewRect.SetWidth(windowRect->GetWidth());
		AGE_ViewRect.SetHeight(windowRect->GetHeight());

        //return curBounds;
    }
}
