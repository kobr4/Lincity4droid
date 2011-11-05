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

#include "Event.hpp"

#include <assert.h>

#include <android/log.h>
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "JNI_DEBUGGING", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,   "JNI_DEBUGGING", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,    "JNI_DEBUGGING", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,    "JNI_DEBUGGING", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,   "JNI_DEBUGGING", __VA_ARGS__)

Event::Event(SDL_Event& event)
    : inside(true)
{
    switch(event.type) {
        case SDL_KEYUP:
            type = KEYUP;
            keysym = event.key.keysym;
            break;
        case SDL_KEYDOWN:
            type = KEYDOWN;
            keysym = event.key.keysym;
            break;
        case SDL_MOUSEMOTION:
            type = MOUSEMOTION;
            mousepos = Vector2(event.motion.x, event.motion.y);
            mousemove = Vector2(event.motion.xrel, event.motion.yrel);
            break;
        case SDL_MOUSEBUTTONUP:
            type = MOUSEBUTTONUP;
            mousepos = Vector2(event.button.x, event.button.y);
            mousebutton = event.button.button;
            break;
        case SDL_MOUSEBUTTONDOWN:
            type = MOUSEBUTTONDOWN;
            mousepos = Vector2(event.button.x, event.button.y);
            mousebutton = event.button.button;
            break;
        //NMY
        case SDL_FINGERDOWN:
            type = FINGERDOWN;
            //LOGD("[Lincity]Event::Event() SDL_FINGERDOWN %d %d\n",event.tfinger.x,event.tfinger.y);
            mousepos = Vector2(event.tfinger.x, event.tfinger.y);
            mousebutton = 0;
            break;
        case SDL_FINGERUP:
            type = FINGERUP;
            //LOGD("[Lincity]Event::Event() SDL_FINGERUP\n");
            mousepos = Vector2(event.tfinger.x, event.tfinger.y);
            mousebutton = 0;
            break;
        case SDL_FINGERMOTION:
            type = FINGERMOTION;
            //LOGD("[Lincity]Event::Event() SDL_FINGERMOTION %d %d %d %d\n",event.tfinger.x,event.tfinger.y,event.tfinger.dx, event.tfinger.dy);
            mousepos = Vector2(event.tfinger.x, event.tfinger.y);
            fingermove = Vector2(event.tfinger.dx, event.tfinger.dy);
            break;
        default:
            assert(false);
    }
}

Event::Event(float _elapsedTime)
    : type(UPDATE), inside(false), elapsedTime(_elapsedTime)
{
}
