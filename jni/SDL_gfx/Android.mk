LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := SDL_gfx

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../SDL/include \

LOCAL_SRC_FILES := SDL_framerate.c \
SDL_gfxPrimitives.c \
SDL_rotozoom.c \
SDL_gfxBlitFunc.c \
SDL_imageFilter.c \

LOCAL_SHARED_LIBRARIES := SDL

#LOCAL_LDLIBS := -lz

include $(BUILD_SHARED_LIBRARY)
