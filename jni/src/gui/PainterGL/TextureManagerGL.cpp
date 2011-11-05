/*
Copyright (C) 2005 Matthias Braun <matze@braunis.de>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <config.h>

#include "TextureManagerGL.hpp"

#include <SDL_opengles.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "TextureGL.hpp"
   
#include <android/log.h>

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "JNI_DEBUGGING", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,   "JNI_DEBUGGING", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,    "JNI_DEBUGGING", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,    "JNI_DEBUGGING", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,   "JNI_DEBUGGING", __VA_ARGS__)

TextureManagerGL::TextureManagerGL()
{
}

TextureManagerGL::~TextureManagerGL()
{
}

static int powerOfTwo(int val) {
    int result = 1;
    while(result < val)
        result *= 2;
    return result;
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

Texture*
TextureManagerGL::create(SDL_Surface* image)
{
    int texture_w = powerOfTwo(image->w);
    int texture_h = powerOfTwo(image->h);

    //int texture_w = image->w;
    //int texture_h = image->h;
    //LOGD("[Lincity]TextureManagerGL::create() - w=%d h=%d w=%d h=%d pitch=%d\n",texture_w,texture_h,image->w,image->h,image->pitch);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    SDL_Surface* convert = SDL_CreateRGBSurface(SDL_SWSURFACE,
            texture_w, texture_h, 32,
            0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
    SDL_Surface* convert = SDL_CreateRGBSurface(SDL_SWSURFACE,
        texture_w, texture_h, 32,
        0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif

    if(convert == 0) {
        std::ostringstream msg;
        msg << "Couldn't convert SDL_Surface while creating texture"
            << " (out of memory?): " << SDL_GetError();
        throw std::runtime_error(msg.str());
    }
    SDL_SetAlpha(image, 0, 0);
     SDL_BlitSurface(image, 0, convert, 0);



    GLuint handle;
    glGenTextures(1, &handle);

    SDL_PixelFormat* format = convert->format;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, handle);

    //LOGD("[Lincity]TextureManagerGL::create() - Handle %d\n",handle);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //glPixelStorei(GL_UNPACK_ROW_LENGTH, convert->pitch/format->BytesPerPixel);
    //assert(convert->pitch == texture_w * convert->format->BytesPerPixel);

       /*
    glTexImage2D(GL_TEXTURE_2D, 0, format->BytesPerPixel,
            convert->w, convert->h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, convert->pixels);
         */
         

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA,
            convert->w, convert->h, 0, GL_RGBA,
             GL_UNSIGNED_BYTE, convert->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   /*
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
     */
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    TextureGL* texture = new TextureGL(handle);

    texture->rect = Rect2D(0, 0,
            (float) image->w/(float) texture_w,
            (float) image->h/(float) texture_h);

    /*
    texture->rect = Rect2D(0, 0,
            1.,
            1.);
            */
    texture->width = image->w;
    texture->height = image->h;

    SDL_FreeSurface(image);
    SDL_FreeSurface(convert);

    if (int a = glGetError() != 0)
    {
       LOGD("[Lincity]TextureManagerGL::create() ERROR OCCURED : %d\n",a);

       switch (a) {
        case GL_INVALID_ENUM:
        LOGD("[Lincity]TextureManagerGL::create() GL_INVALID_ENUM\n");
        break;
        case GL_INVALID_VALUE:
        LOGD("[Lincity]TextureManagerGL::create() GL_INVALID_VALUE\n");
        break;
        case GL_INVALID_OPERATION:
        LOGD("[Lincity]TextureManagerGL::create() GL_INVALID_OPERATION\n");
        break;
        case GL_STACK_OVERFLOW:
        LOGD("[Lincity]TextureManagerGL::create() GL_STACK_OVERFLOW\n");;
        break;
        case GL_STACK_UNDERFLOW:
        LOGD("[Lincity]TextureManagerGL::create() GL_STACK_UNDERFLOW\n");
        break;
        case GL_OUT_OF_MEMORY:
        LOGD("[Lincity]TextureManagerGL::create() GL_OUT_OF_MEMORY\n");
        break;
       }
    }
    return texture;
}

