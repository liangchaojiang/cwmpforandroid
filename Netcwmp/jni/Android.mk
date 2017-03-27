LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := pool
LOCAL_SRC_FILES := libpool/src/pool.c 

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := xmlet
LOCAL_SRC_FILES := libxmlet/src/attr.c libxmlet/src/node.c libxmlet/src/element.c libxmlet/src/parser.c \
libxmlet/src/buffer.c libxmlet/src/document.c libxmlet/src/list.c libxmlet/src/namemap.c libxmlet/src/xmlet.c

LOCAL_CFLAGS += -DUSE_CWMP_MEMORY_POOL
LOCAL_LDLIBS := \
	-lz \
	-lm 
LOCAL_SHARED_LIBRARIES := libpool

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := cwmp
LOCAL_SRC_FILES := libcwmp/src/buffer.c libcwmp/src/cfg.c libcwmp/src/cwmp.c libcwmp/src/http.c libcwmp/src/ssl.c \
libcwmp/src/ini.c libcwmp/src/log.c libcwmp/src/md5.c libcwmp/src/memory.c libcwmp/src/session.c libcwmp/src/util.c \
libcwmp/src/event.c libcwmp/src/queue.c libcwmp/src/model.c libcwmp/src/callback.c
LOCAL_SHARED_LIBRARIES := libpool libxmlet
LOCAL_CFLAGS += -DUSE_CWMP_MEMORY_POOL
LOCAL_LDLIBS := \
	-lz \
	-lm 
	
include $(BUILD_STATIC_LIBRARY)


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









