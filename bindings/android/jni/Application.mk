# only build armeabi-v7a, if required add 'armeabi' or 'x86'
APP_ABI := armeabi-v7a
APP_CPPFLAGS += -fexceptions -frtti -std=c++11
APP_OPTIM := release
APP_STL := c++_shared
APP_MODULES := verovio-android
