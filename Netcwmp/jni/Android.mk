LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := pool
LOCAL_SRC_FILES := libpoo.a 

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := xmlet


LOCAL_SRC_FILES := libxmlet.a 


LOCAL_SHARED_LIBRARIES := libpool

include $(RREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := cwmp

LOCAL_SRC_FILES := libcwmp.a 

LOCAL_SHARED_LIBRARIES := libpool libxmlet
	

include $(RREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := cwmpd
LOCAL_SRC_FILES := cwmpd/src/conf.c cwmpd/src/cwmpd.c cwmpd/src/httpd.c cwmpd/src/process.c cwmpd/src/thread.c \
cwmpd/src/agent.c cwmpd/src/data_model.c


LOCAL_SHARED_LIBRARIES := libpool libxmlet libcwmp
LOCAL_CFLAGS += -DUSE_CWMP_MEMORY_POOL \
	-D_GNU_SOURCE

LOCAL_LDLIBS := \
	-lz \
	-lm \




include $(BUILD_EXECUTABLE)









