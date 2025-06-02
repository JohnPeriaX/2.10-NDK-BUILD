# Application configuration
APP_ABI := arm64-v8a
APP_PLATFORM := android-21
APP_STL := c++_shared
#APP_STL := c++_static

# Build configuration
APP_OPTIM := release
APP_STRIP_MODE := --strip-unneeded

# C++ features
APP_CPPFLAGS := -std=c++20 -fexceptions -frtti

# Enable multithreaded build
APP_ALLOW_MISSING_DEPS := true

# Modules to build
APP_MODULES := samp