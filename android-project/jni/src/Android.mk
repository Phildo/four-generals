LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	src/main.cpp src/game.cpp src/graphics.cpp src/input.cpp src/ui.cpp src/network.cpp src/server.cpp src/client.cpp src/connection.cpp src/event.cpp src/scene.cpp src/intro_scene.cpp src/host_scene.cpp src/join_scene.cpp src/room_scene.cpp src/play_scene.cpp src/sprite.cpp src/model.cpp src/compass.cpp src/week.cpp src/client_model.cpp src/server_model.cpp src/general.cpp src/messenger.cpp src/string.cpp src/third/android_ifaddrs.c

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)
