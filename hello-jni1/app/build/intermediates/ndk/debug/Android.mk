LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := hello-jni
LOCAL_SRC_FILES := \
	/home/jeff/AndroidstudioProjects/hello-jni1/app/src/main/jni/Android.mk \
	/home/jeff/AndroidstudioProjects/hello-jni1/app/src/main/jni/Application.mk \
	/home/jeff/AndroidstudioProjects/hello-jni1/app/src/main/jni/hello-jni.c \
	/home/jeff/AndroidstudioProjects/hello-jni1/app/src/main/jni/production/hello-jni/hello-jni.apk \
	/home/jeff/AndroidstudioProjects/hello-jni1/app/src/main/jni/production/hello-jni/com/example/hellojni/R.class \
	/home/jeff/AndroidstudioProjects/hello-jni1/app/src/main/jni/production/hello-jni/com/example/hellojni/R$attr.class \
	/home/jeff/AndroidstudioProjects/hello-jni1/app/src/main/jni/production/hello-jni/com/example/hellojni/R$string.class \
	/home/jeff/AndroidstudioProjects/hello-jni1/app/src/main/jni/production/hello-jni/com/example/hellojni/BuildConfig.class \

LOCAL_C_INCLUDES += /home/jeff/AndroidstudioProjects/hello-jni1/app/src/main/jni
LOCAL_C_INCLUDES += /home/jeff/AndroidstudioProjects/hello-jni1/app/src/debug/jni

include $(BUILD_SHARED_LIBRARY)
