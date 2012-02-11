#include "SDL.h"
#ifdef __APPLE__
    #include "SDL_image/SDL_image.h"
#else
    #include "SDL_image.h"
#endif
#include "minigl.h"
#include <string>


/**
 *
 *	Function: mglGetTexture
 *	This helper function loads a texture file in .bmp format 
 *	into the buffer data. The output paramters width and height
 *	give the size of the texture. Note: This function allocates
 *  memory for you. Don't forget to free it.
 *
 *	@param filename - Name of file to open
 *  @param data - Buffer to be allocated and filled with texture data.
 * 	@param width - output parameter for the width of the texture
 *	@param height - output parameter for height of the texture
 */

using std::string;

inline void mglGetTexture(string filename, MGLpixel*& data, MGLsize* width, MGLsize* height){
	
	SDL_Surface* tex;
	tex = IMG_Load(filename.c_str());

	SDL_PixelFormat newFormat = *tex->format;
	newFormat.BytesPerPixel = sizeof(MGLpixel);
	newFormat.BitsPerPixel = 8*sizeof(MGLpixel);
	
	SDL_Surface* conv =  SDL_ConvertSurface(tex,&newFormat,SDL_SWSURFACE);
	SDL_FreeSurface(tex);

	
	*width = conv->w;
	*height= conv->h;

	data = new MGLpixel[conv->w * conv->h];


	for(int r = 0; r < conv->h; r++){
		for(int c = 0; c < conv->w; c++){
			int index = (conv->h-1 -r)*conv->w + c;
			MGLpixel pixel = *(((MGLpixel*) conv->pixels)+r*conv->w+c);
			MGL_SET_RED( data[index], ((pixel & newFormat.Rmask) >> newFormat.Rshift) <<newFormat.Rloss);
			MGL_SET_GREEN( data[index], ((pixel & newFormat.Gmask) >> newFormat.Gshift) <<newFormat.Gloss);
			MGL_SET_BLUE( data[index], ((pixel & newFormat.Bmask) >> newFormat.Bshift) <<newFormat.Bloss);
			//Set Alpha bits
			data[index] = data[index] |  0xff000000; 
		}
	}

	SDL_FreeSurface(conv);
}
