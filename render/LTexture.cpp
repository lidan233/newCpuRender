//
// Created by lidan on 24/12/2020.
//

#include "LTexture.h"

LTexture::LTexture(SDL_Renderer* render)
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
    mPixels = NULL;
    mPitch = 0;
    _render = render;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromBuffer( Uint32* pixels, int width,int height)
{
    //Get rid of preexisting texture
    free();
    mPixels = pixels ;
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 rmask = 0xff000000;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0x0000ff00;
    Uint32 amask = 0x000000ff;
//#else
//    Uint32 rmask = 0x000000ff;
//    Uint32 gmask = 0x0000ff00;
//    Uint32 bmask = 0x00ff0000;
//    Uint32 amask = 0xff000000;
//#endif
    mPitch = height*4 ;
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom((void*)pixels, width, height, 32,mPitch,
                                   rmask, gmask, bmask, amask);
//    SDL_Surface * surface = SDL_CreateRGBSurface(0, width, height, 32,
//                                                 0, 0, 0, 0);

    if( surface == NULL )
    {
        printf( "Unable to load image  from your pixels" );
    }
    else
    {

        newTexture = SDL_CreateTextureFromSurface(_render,surface);
        mWidth = surface->w;
        mHeight = surface->h;
//        //Create blank streamable texture
//            newTexture = SDL_CreateTexture( _render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );
//            if( newTexture == NULL )
//            {
//                printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
//            }
//            else
//            {
//                //Enable blending on texture
//                SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );
//
//                //Lock texture for manipulation
//                SDL_LockTexture( newTexture, &formattedSurface->clip_rect, &mPixels, &mPitch );
//
//                //Copy loaded/formatted surface pixels
//                memcpy( mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h );
//
//                //Get image dimensions
//                mWidth = formattedSurface->w;
//                mHeight = formattedSurface->h;
//
//                //Get pixel data in editable format
//                Uint32* pixels = (Uint32*)mPixels;
//                int pixelCount = ( mPitch / 4 ) * mHeight;
//
//                //Map colors
//                Uint32 colorKey = SDL_MapRGB( formattedSurface->format, 0, 0xFF, 0xFF );
//                Uint32 transparent = SDL_MapRGBA( formattedSurface->format, 0x00, 0xFF, 0xFF, 0x00 );
//
//                //Color key pixels
//                for( int i = 0; i < pixelCount; ++i )
//                {
//                    if( pixels[ i ] == colorKey )
//                    {
//                        pixels[ i ] = transparent;
//                    }
//                }
//
//                //Unlock texture to update
//                SDL_UnlockTexture( newTexture );
//                mPixels = NULL;
//            }
//
//            //Get rid of old formatted surface
//            SDL_FreeSurface( formattedSurface );
//        }

        //Get rid of old loaded surface
//        SDL_FreeSurface( surface );
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return mTexture != NULL;
}
#endif

bool LTexture::createBlank( int width, int height, SDL_TextureAccess access )
{
    //Create uninitialized texture
    mTexture = SDL_CreateTexture( _render, SDL_PIXELFORMAT_RGBA8888, access, width, height );
    if( mTexture == NULL )
    {
        printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
    }
    else
    {
        mWidth = width;
        mHeight = height;
    }

    return mTexture != NULL;
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
        mPixels = NULL;
        mPitch = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture rgb
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx( _render, mTexture, clip, &renderQuad, angle, center, flip );
}

void LTexture::setAsRenderTarget()
{
    //Make self render target
    SDL_SetRenderTarget( _render, mTexture );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

bool LTexture::lockTexture()
{
    bool success = true;

    //Texture is already locked
    if( mPixels != NULL )
    {
        printf( "Texture is already locked!\n" );
        success = false;
    }
        //Lock texture
    else
    {
        if( SDL_LockTexture( mTexture, NULL, &mPixels, &mPitch ) != 0 )
        {
            printf( "Unable to lock texture! %s\n", SDL_GetError() );
            success = false;
        }
    }

    return success;
}

bool LTexture::unlockTexture()
{
    bool success = true;

    //Texture is not locked
    if( mPixels == NULL )
    {
        printf( "Texture is not locked!\n" );
        success = false;
    }
        //Unlock texture
    else
    {
        SDL_UnlockTexture( mTexture );
        mPixels = NULL;
        mPitch = 0;
    }

    return success;
}

SDL_Texture* LTexture::getTexture() {
    return mTexture ;
}

void* LTexture::getPixels()
{
    return mPixels;
}

void LTexture::copyPixels( void* pixels )
{
    //Texture is locked
    if( mPixels != NULL )
    {
        //Copy to locked pixels
        memcpy( mPixels, pixels, mPitch * mHeight );
    }
}

int LTexture::getPitch()
{
    return mPitch;
}

Uint32 LTexture::getPixel32( unsigned int x, unsigned int y )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32*)mPixels;

    //Get the pixel requested
    return pixels[ ( y * ( mPitch / 4 ) ) + x ];
}