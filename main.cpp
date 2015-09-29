#include <iostream>
#include "AGE/AGE.h"
#include "AGE/AGE_Graphics.h"
#include "AGE/AGE_Input.h"
#include "main.h"

void EventHandle(SDL_Event *event);
void Update(AGE_Engine*);
void Draw(AGE_Engine*);

AGE_SpriteBatch *sB;

AGE_Sprite sprite;
AGE_Vector *pos;
AGE_Vector *pos2;

AGE_Keyboard *keyboard;
AGE_Mouse *mouse;

void UpdateInput();

int main()
{
	AGE_Engine* engine = new AGE_Engine;
	engine->Init("Helsys", 1680, 1050, true);

	sB = new AGE_SpriteBatch(engine);

	pos = new AGE_Vector(0.0f,0.0f);
	pos2 = new AGE_Vector(400.0f,400.0f);

	mouse = new AGE_Mouse;
	keyboard = new AGE_Keyboard;

	sprite.Load(engine, "test.png");

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
	sB->Render(&sprite, pos, NULL, 0, 0.0, NULL, SDL_FLIP_NONE);
	sB->DrawEnd(NULL);
}