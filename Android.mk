LOCAL_PATH := $(call my-dir)

###########################################
# Prebuilt Opus Library
###########################################
include $(CLEAR_VARS)
LOCAL_MODULE := opus
LOCAL_SRC_FILES := vendor/opus/lib/arm64-v8a/libopus.a
include $(PREBUILT_STATIC_LIBRARY)

###########################################
# Prebuilt ShadowHook Library
###########################################
include $(CLEAR_VARS)
LOCAL_MODULE := shadowhook
LOCAL_SRC_FILES := vendor/shadowhook/arm64-v8a/libshadowhook.so
include $(PREBUILT_SHARED_LIBRARY)

###########################################
# Prebuilt BASS Library
###########################################
include $(CLEAR_VARS)
LOCAL_MODULE := bass
LOCAL_SRC_FILES := vendor/bass/libs/arm64-v8a/libbass.so
include $(PREBUILT_SHARED_LIBRARY)

###########################################
# SAMP Library
###########################################
include $(CLEAR_VARS)
LOCAL_MODULE := samp
LOCAL_CPP_FEATURES := exceptions rtti

# Source files - recursively find all .c and .cpp files
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/*.c) \
                   $(wildcard $(LOCAL_PATH)/*.cpp) \
                   $(wildcard $(LOCAL_PATH)/*/*.c) \
                   $(wildcard $(LOCAL_PATH)/*/*.cpp) \
                   $(wildcard $(LOCAL_PATH)/*/*/*.c) \
                   $(wildcard $(LOCAL_PATH)/*/*/*.cpp) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*.c) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*.cpp) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*/*.c) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*/*.cpp) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*/*/*.c) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*/*/*.cpp) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*/*/*/*.c) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*/*/*/*.cpp) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*/*/*/*/*.c) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*/*/*/*/*.cpp) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*/*/*/*/*/*.c) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*/*/*/*/*/*.cpp) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*/*/*/*/*/*/*.c) \
                   $(wildcard $(LOCAL_PATH)/*/*/*/*/*/*/*/*/*/*.cpp)

# Convert absolute paths to relative paths
LOCAL_SRC_FILES := $(LOCAL_SRC_FILES:$(LOCAL_PATH)/%=%)

# Include directories

LOCAL_C_INCLUDES += $(wildcard $(LOCAL_PATH)/vendor/)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/vendor/shadowhook/include \
                    $(LOCAL_PATH)/vendor/opus/include \
                    $(LOCAL_PATH)/util/ \
                    $(LOCAL_PATH)/vendor/armhook \
                    $(LOCAL_PATH)/vendor/imgui \
                    $(LOCAL_PATH)/vendor/ \
                    $(LOCAL_PATH)/game/RW/ \
                    $(LOCAL_PATH)/game/Core/

# Compilation flags
LOCAL_CPPFLAGS := -w -s -g -fvisibility=hidden -pthread -Wall -O3 -fexceptions -std=c++20 -DUSE_ALLOCA
LOCAL_CFLAGS := -w -s -g -fvisibility=hidden -pthread -Wall -O3 -fexceptions -DUSE_ALLOCA

# Linker flags
LOCAL_LDFLAGS := -s

# Static libraries
LOCAL_STATIC_LIBRARIES := opus

# Shared libraries
LOCAL_SHARED_LIBRARIES := shadowhook bass

# System libraries
LOCAL_LDLIBS := -llog -lEGL -lGLESv3 -lc

# Build shared library
include $(BUILD_SHARED_LIBRARY)