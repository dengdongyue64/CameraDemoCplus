LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= CameraDemo.cpp
LOCAL_MODULE:= CameraTest

LOCAL_MODULE_TAGS := tests
LOCAL_C_INCLUDES += \
        frameworks/base/libs \
        frameworks/base/include \
        system/core/include \
        hardware/libhardware/include \
        frameworks/base/include/media/stagefright/openmax \
	frameworks/av/media/libmediaplayerservice \
	frameworks/av/include/media/stagefright/timedtext \
	frameworks/native/include/media/hardware \
	frameworks/native/include/media/openmax \
	frameworks/native/include   \
	external/flac/include \
	external/tremolo \
	external/openssl/include \
        frameworks/base/media/libmediaplayerservice

LOCAL_CFLAGS :=

LOCAL_SHARED_LIBRARIES += \
		libbinder \
                libcutils \
                libutils \
                libui \
                libcamera_client \
                libgui

LOCAL_SHARED_LIBRARIES += \
        libstagefright libmedia liblog libstagefright_foundation \
        libskia libstdc++ libc libdl libmediaplayerservice libcamera_client

#ifeq ($(USE_PIXTREE_STAGEFRIGHT), true)
#LOCAL_SHARED_LIBRARIES += libpxtrstagefright
#endif

# Disable it because the ISurface interface may change, and before we have a
# chance to fix this test, we don't want to break normal builds.
include $(BUILD_EXECUTABLE)
