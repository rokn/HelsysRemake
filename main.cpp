#include <iostream>
#include "AGE/AGE.h"
#include "AGE/AGE_Graphics.h"
#include "AGE/AGE_Input.h"
#include "main.h"
//#include "BOX2D/Box2D.h"

void EventHandle(SDL_Event *event);
void Update(AGE_Engine*);
void Draw(AGE_Engine*);

AGE_SpriteBatch *sB;

AGE_Keyboard *keyboard;
AGE_Mouse *mouse;

void UpdateInput();

int main()
{
	AGE_Engine* engine = new AGE_Engine;
	engine->Init("Helsys", 1680, 1050, true);

	sB = new AGE_SpriteBatch(engine);

	mouse = new AGE_Mouse;
	keyboard = new AGE_Keyboard;

	engine->Run(EventHandle, Update, Draw);	
}

void EventHandle(SDL_Event *event)
{
	mouse->Update(event);
}

void Update(AGE_Engine* engine)
{
	keyboard->Update();

	if(keyboard->KeyIsDown(SDL_SCANCODE_ESCAPE))
	{
		engine->Exit();
		engine->Destroy();
	}
}

void Draw(AGE_Engine* engine)
{
	sB->DrawBegin();
	sB->DrawEnd(NULL);
}