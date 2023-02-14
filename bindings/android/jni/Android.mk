LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := verovio-android

ROOT_PATH := $(LOCAL_PATH)/../../..

LOCAL_CPPFLAGS := -Dandroid -frtti -DNO_DARMS_SUPPORT -DNO_PAE_SUPPORT -DNO_HUMDRUM_SUPPORT

VEROVIO_SRC := $(wildcard $(ROOT_PATH)/src/*.cpp)
MIDI_SRC := $(wildcard $(ROOT_PATH)/src/midi/*.cpp)

LOCAL_SRC_FILES := \
	$(VEROVIO_SRC:$(VEROVIO_SRC)/%=%) \
	$(MIDI_SRC:$(MIDI_SRC)/%=%) \
	$(ROOT_PATH)/src/pugi/pugixml.cpp \
	$(ROOT_PATH)/src/json/jsonxx.cc \
	$(ROOT_PATH)/libmei/dist/*.cpp \
	$(ROOT_PATH)/libmei/addons/*.cpp \
	$(ROOT_PATH)/src/crc/crc.cpp
	
LOCAL_C_INCLUDES := \
	$(ROOT_PATH)/include \
	$(ROOT_PATH)/include/vrv \
	$(ROOT_PATH)/libmei/dist \
	$(ROOT_PATH)/libmei/addons \
	$(ROOT_PATH)/include/utf8 \
	$(ROOT_PATH)/include/pugi \
	$(ROOT_PATH)/include/hum \
	$(ROOT_PATH)/include/midi \
	$(ROOT_PATH)/include/json \
	$(ROOT_PATH)/include/crc \
	$(ROOT_PATH)/include/zip

include $(BUILD_SHARED_LIBRARY)
