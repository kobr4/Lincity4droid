LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := physfs


LOCAL_CFLAGS := -DDEBUG -DPHYSFS_NO_CDROM_SUPPORT

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/.. \

LOCAL_SRC_FILES := physfs.c \
physfs_byteorder.c \
physfs_unicode.c \
platform/unix.c \
platform/posix.c \
archivers/dir.c

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
