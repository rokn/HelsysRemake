#include "AGE.h"
#include "AGE_Graphics.h"

// bool testIterator(void *rSprite);
void renderSpriteIterator(renderSprite_age, AGE_Camera*);
void renderSpriteIteratorGUI(renderSprite_age);

AGE_Rect AGE_ViewRect;

void AGE_SpriteBatch::DrawBegin()
{
	this->renderSprites.clear();
}

void AGE_SpriteBatch::Render(AGE_Sprite* sprite, AGE_Vector *pos, AGE_Rect* clip, short depth, double rotation, AGE_Vector *origin, SDL_RendererFlip flip)
{
	SDL_Rect renderRect = {(int)(pos->GetX()), (int)(pos->GetY()), sprite->GetWidth(), sprite->GetHeight()};

	renderSprite_age renderSprite;

	if(clip!=NULL)
	{
		renderRect.w = clip->GetWidth();
		renderRect.h = clip->GetHeight();
		renderSprite.clip = (SDL_Rect){clip->GetX(), clip->GetY(), clip->GetWidth(), clip->GetHeight()};
	}

	AGE_Rect imageRect = (AGE_Rect){renderRect.x, renderRect.y, renderRect.w, renderRect.h};	

	if(!imageRect.Intersects(AGE_ViewRect))
	{

		return;
	}

	renderSprite.sprite = sprite;
	renderSprite.pos = pos;	
	renderSprite.renderRect = renderRect;
	renderSprite.rotation = rotation;
	renderSprite.flip = flip;
	renderSprite.depth = depth;

	SDL_Point sdlOrigin;

	if(origin!=NULL)
	{
		sdlOrigin.x = (int)origin->GetX();
		sdlOrigin.y = (int)origin->GetY();
	}
	else
	{
		sdlOrigin.x = 0;
		sdlOrigin.y = 0;
	}

	renderSprite.origin = sdlOrigin;

	
	bool inserted = false;
	for (vector<renderSprite_age>::iterator it = this->renderSprites.begin(); it != this->renderSprites.end(); ++it)
	{		
		if((*it).depth>renderSprite.depth)
		{
			this->renderSprites.insert(it, renderSprite);
			inserted = true;			
			break;
		}
	}

	if(!inserted)
	{
		this->renderSprites.push_back(renderSprite);		
	}
}




void AGE_SpriteBatch::DrawEnd(AGE_Camera* camera)
{	
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);	

	vector<renderSprite_age>::iterator it;

	// AGE_ListForEach( ,testIterator);

	for (it = this->renderSprites.begin(); it != this->renderSprites.end(); ++it)
	{				
		renderSpriteIterator(*it, camera);
	}

	SDL_RenderPresent(gRenderer);
}

// bool testIterator(void *rSprite)
// {
// 	renderSprite_age* rS = (renderSprite_age*)rSprite;
// 	// printf("%d\n",rS->depth);
// }
void renderSpriteIterator(renderSprite_age rSprite, AGE_Camera* camera)
{
	if(camera != NULL)
	{
		rSprite.renderRect.x -= (int)camera->GetOffset().GetX();
		rSprite.renderRect.y -= (int)camera->GetOffset().GetY();
	}	
	cout << rSprite.renderRect.x << " "<<rSprite.renderRect.y << " "<<rSprite.renderRect.w << " "<<rSprite.renderRect.h << endl;

	SDL_RenderCopyEx(gRenderer, rSprite.sprite->GetTexture(), NULL, &(rSprite.renderRect), rSprite.rotation, &rSprite.origin, rSprite.flip);
	// printf("ERROR");
}

void renderSpriteIteratorGUI(renderSprite_age rSprite)
{
	SDL_RenderCopyEx(gRenderer, rSprite.sprite->GetTexture(), &rSprite.clip, &(rSprite.renderRect), rSprite.rotation, &rSprite.origin, rSprite.flip);
	// printf("ERROR");
}

