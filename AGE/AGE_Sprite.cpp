#include "AGE.h"
#include "AGE_Graphics.h"

void AGE_Sprite::Destroy()
{	
	freeTexture();
}

void AGE_Sprite::freeTexture()
{
	if( this->texture != NULL )
	{
		SDL_DestroyTexture( this->texture );
		this->texture = NULL;
		this->width = 0;
		this->height = 0;
	}
}

bool AGE_Sprite::CreateBlank(int width, int height, SDL_TextureAccess access)
{
    this->texture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);

    if( this->texture == NULL )
    {
        printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        this->width = width;
        this->height = height;
    }

    return this->texture != NULL;
}

void AGE_Sprite::SetRenderTarget()
{
	SDL_SetRenderTarget(gRenderer, this->texture);
}

bool AGE_Sprite::Load(char *path)
{
	freeTexture();
	
	SDL_Texture *finalTexture = NULL;
	SDL_Surface *loadedSurface = IMG_Load(path);

	if(loadedSurface == NULL)
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
	else
	{

		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        finalTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );

		if( finalTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			this->width = loadedSurface->w;
			this->height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	this->texture = finalTexture;
	return finalTexture != NULL;
}

void AGE_Sprite::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod( this->texture, red, green, blue );
}

void AGE_Sprite::SetBlendMode(SDL_BlendMode blendMode)
{
	SDL_SetTextureBlendMode( this->texture, blendMode );
}
		
void AGE_Sprite::SetAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(this->texture, alpha);
}

// void AGE_Sprite::RenderGUI(AGE_Vector *pos, AGE_Rect* clip, short depth, double rotation = 0.0, AGE_Vector *origin = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE)
// {
// 	SDL_Rect renderRect = {(int)pos->GetX(), (int)pos->GetY(), this->width, this->height};

// 	if(clip!=NULL)
// 	{
// 		renderRect.w = clip->GetWidth();
// 		renderRect.h = clip->GetHeight();
// 	}

// 	AGE_Rect imageRect = (AGE_Rect){renderRect.x, renderRect.y, renderRect.w, renderRect.h};	

// 	if(!imageRect.Intersects(AGE_ViewRect))
// 	{
// 		return;
// 	}

// 	renderSprite_age renderSprite;
// 	renderSprite.sprite = this;
// 	renderSprite.pos = pos;
// 	renderSprite.clip = (SDL_Rect){clip->GetX(), clip->GetY(), clip->GetWidth(), clip->GetHeight()};
// 	renderSprite.renderRect = renderRect;
// 	renderSprite.rotation = rotation;
// 	renderSprite.flip = flip;
// 	renderSprite.depth = depth;

// 	SDL_Point sdlOrigin;

// 	if(origin!=NULL)
// 	{
// 		sdlOrigin.x = (int)origin->GetX();
// 		sdlOrigin.y = (int)origin->GetY();
// 	}
// 	else
// 	{
// 		sdlOrigin.x = 0;
// 		sdlOrigin.y = 0;
// 	}

// 	renderSprite.origin = sdlOrigin;

// 	bool inserted = false;
// 	for (vector<renderSprite_age>::iterator it = spriteBatchGui_age.renderSprites.begin(); it != spriteBatchGui_age.renderSprites.end(); ++it)
// 	{		
// 		if((*it).depth>renderSprite.depth)
// 		{
// 			spriteBatchGui_age.renderSprites.insert(it, renderSprite);
// 			inserted = true;
// 			break;
// 		}
// 	}

// 	if(!inserted)
// 	{
// 		spriteBatchGui_age.renderSprites.push_back(renderSprite);
// 	}
// }

bool AGE_Sprite::LoadFromText(char* text, SDL_Color textColor, TTF_Font* font)
{
	freeTexture();

	SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
	
	if(textSurface == NULL)
	{
		printf("Text redner error '%s'", TTF_GetError());
	}
	else
	{		
		this->texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

		if(this->texture == NULL)
		{
			printf("Text render texture from surface error '%s'", text);
		}
		else
		{
			this->width = textSurface->w;
			this->height = textSurface->h;
		}

		SDL_FreeSurface(textSurface);
	}

	return this->texture != NULL;
}

SDL_Texture* AGE_Sprite::GetTexture()
{
	return this->texture;
}

int AGE_Sprite::GetWidth()
{
	return this->width;
}

int AGE_Sprite::GetHeight()
{
	return this->height;
}