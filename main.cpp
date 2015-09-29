#include <iostream>
#include "AGE/AGE.h"
#include "AGE/AGE_Graphics.h"

void EventHandle(SDL_Event *event);
void Update();
void Draw();

AGE_Sprite sprite;
AGE_Vector* pos;
AGE_SpriteBatch sB;

int main()
{
	AGE engine;
	engine.Init("Helsys", 1680, 1050, true);
	sprite.Load("test.png");
	pos = new AGE_Vector(0.0f,0.0f);
	engine.Run(EventHandle, Update, Draw);	
}

void EventHandle(SDL_Event *event)
{

}

void Update()
{

}

void Draw()
{
	sB.DrawBegin();
	sB.Render(&sprite, pos, NULL, 0, 0.0, NULL, SDL_FLIP_NONE);
	sB.DrawEnd(NULL);
}