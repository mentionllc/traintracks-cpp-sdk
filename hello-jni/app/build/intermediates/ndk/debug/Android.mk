LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := hello-jni
LOCAL_SRC_FILES := \
	/home/jeff/AndroidstudioProjects/hello-jni/app/src/main/jni/Android.mk \
	/home/jeff/AndroidstudioProjects/hello-jni/app/src/main/jni/Application.mk \
	/home/jeff/AndroidstudioProjects/hello-jni/app/src/main/jni/hello-jni.c \

LOCAL_C_INCLUDES += /home/jeff/AndroidstudioProjects/hello-jni/app/src/main/jni
LOCAL_C_INCLUDES += /home/jeff/AndroidstudioProjects/hello-jni/app/src/debug/jni

include $(BUILD_SHARED_LIBRARY)
