LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := SDL_ttf

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../SDL/include \
                 $(LOCAL_PATH)/../freetype/include

LOCAL_SRC_FILES := SDL_ttf.c

LOCAL_SHARED_LIBRARIES := SDL

LOCAL_STATIC_LIBRARIES := freetype

LOCAL_LDLIBS := -lz

include $(BUILD_SHARED_LIBRARY)
