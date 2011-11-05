
/* Include the SDL main definition header */
#include "SDL_main.h"

/*******************************************************************************
                 Functions called by JNI
*******************************************************************************/
#include <jni.h>
#include <SDL.h>

#include <android/log.h>
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "JNI_DEBUGGING", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,   "JNI_DEBUGGING", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,    "JNI_DEBUGGING", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,    "JNI_DEBUGGING", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,   "JNI_DEBUGGING", __VA_ARGS__)



#define ACTION_CANCEL              0x00000003
#define ACTION_DOWN                0x00000000
#define ACTION_HOVER_ENTER         0x00000000
#define ACTION_HOVER_EXIT          0x0000000a
#define ACTION_HOVER_MOVE          0x00000007
#define ACTION_MASK                0x000000ff
#define ACTION_MOVE                0x00000002
#define ACTION_OUTSIDE             0x00000004
#define ACTION_POINTER_DOWN        0x00000005
#define ACTION_POINTER_INDEX_MASK  0x0000ff00
#define ACTION_POINTER_INDEX_SHIFT 0x00000008
#define ACTION_POINTER_UP          0x00000006
#define ACTION_SCROLL              0x00000008
#define ACTION_UP                  0x00000001

int g_android_pause = 0;
int g_android_quit = 0;

// Called before SDL_main() to initialize JNI bindings in SDL library
extern "C" void SDL_Android_Init(JNIEnv* env, jclass cls);
//extern "C" void Android_RunAudioThread();
//extern "C" JNIEnv* mAudioEnv;

// Library init
extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    return JNI_VERSION_1_4;
}

// Start up the SDL app
extern "C" void Java_org_libsdl_app_Lincity4droidActivity_nativeInit(JNIEnv* env, jclass cls,int w, int h)
{
    LOGD("[Lincity]Java_org_libsdl_app_Lincity4droidActivity_nativeInit() %d %d",w,h);
    /* This interface could expand with ABI negotiation, calbacks, etc. */
    SDL_Android_Init(env, cls);

    /* Run the application code! */
    int status;
    char *argv[2];

    char res[10];
    sprintf(res,"%dx%d",w,h);
    char s[3];
    sprintf(s,"-S");
    argv[0] = strdup("SDL_app");
    //argv[1] = NULL;
    argv[1] = strdup(s);
    argv[2] = strdup(res);
    argv[3] = NULL;
    status = SDL_main(3, argv);

    /* We exit here for consistency with other platforms. */
    exit(status);
}

// Start up the SDL app
extern "C" void Java_org_libsdl_app_Lincity4droidActivity_onNativeResize(JNIEnv* env, jclass cls, jobject obj)
{
}

extern "C" void Java_org_libsdl_app_Lincity4droidActivity_onNativeKeyDown(JNIEnv* env, jclass cls, jobject obj)
{
}

extern "C" void Java_org_libsdl_app_Lincity4droidActivity_onNativeKeyUp(JNIEnv* env, jclass cls, jobject obj)
{
}

//extern "C" void Java_org_libsdl_app_Lincity4droidActivity_onNativeTouch(JNIEnv* env, jclass cls, jobject obj)
extern "C" void Java_org_libsdl_app_Lincity4droidActivity_onNativeTouch(JNIEnv* env, jclass cls,int action, float x, float y,float p)
{
  //LOGD("[Lincity]Java_org_libsdl_app_Lincity4droidActivity_onNativeTouch() Action: %d %f %f %d %d\n",action,x,y,(Uint16)x,(Uint16)y);
  static SDL_Event event;
  switch (action)
  {
    case ACTION_UP :
         event.type = SDL_FINGERUP;
         event.tfinger.x = (Uint16)x;
         event.tfinger.y = (Uint16)y;
         if (SDL_PushEvent(&event) == -1)
         {
            LOGD("[Lincity]Java_org_libsdl_app_Lincity4droidActivity_onNativeTouch() Error : could not push event into event queue\n");
          }
         break;
    case ACTION_DOWN :
         event.type = SDL_FINGERDOWN;
         event.tfinger.x = (Uint16)x;
         event.tfinger.y = (Uint16)y;
         if (SDL_PushEvent(&event) == -1)
         {
            LOGD("[Lincity]Java_org_libsdl_app_Lincity4droidActivity_onNativeTouch() Error : could not push event into event queue\n");
         }
         break;
    case ACTION_MOVE :
         if ((event.type ==  SDL_FINGERMOTION) || (event.type = SDL_FINGERDOWN))
         {
            event.type = SDL_FINGERMOTION;
            event.tfinger.dx = event.tfinger.x - (Uint16)x;
            event.tfinger.dy = event.tfinger.y - (Uint16)y;
            event.tfinger.y = (Uint16)y;
            event.tfinger.x = (Uint16)x;
            if (SDL_PushEvent(&event) == -1)
            {
              LOGD("[Lincity]Java_org_libsdl_app_Lincity4droidActivity_onNativeTouch() Error : could not push event into event queue\n");
            }
         }
         break;
    default : break;
  }

}

extern "C" void Java_org_libsdl_app_Lincity4droidActivity_onNativeAccel(JNIEnv* env, jclass cls, jobject obj)
{
}

extern "C" void Java_org_libsdl_app_Lincity4droidActivity_onNativePause(JNIEnv* env, jclass cls, jobject obj)
{
    LOGD("[Lincity]Java_org_libsdl_app_Lincity4droidActivity_onNativePause()");
    g_android_pause = 1;
}

extern "C" void Java_org_libsdl_app_Lincity4droidActivity_onNativeResume(JNIEnv* env, jclass cls, jobject obj)
{
    LOGD("[Lincity]Java_org_libsdl_app_Lincity4droidActivity_onNativeResume()");
    g_android_pause = 0;
}

extern "C" void Java_org_libsdl_app_Lincity4droidActivity_nativeQuit(JNIEnv* env, jclass cls, jobject obj)
{
    LOGD("[Lincity]Java_org_libsdl_app_Lincity4droidActivity_nativeQuit()");
    g_android_quit = 1;
}
/*
extern "C" void Java_org_libsdl_app_Lincity4droidActivity_nativeRunAudioThread(JNIEnv* env, jclass cls, jobject obj)
{
     LOGD("[Lincity]Java_org_libsdl_app_Lincity4droidActivity_nativeRunAudioThread()");
     mAudioEnv = env;
     Android_RunAudioThread();
}
*/

/* vi: set ts=4 sw=4 expandtab: */
