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
LOCAL_STATIC_LIBRARIES := libpool

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := cwmp
LOCAL_SRC_FILES := libcwmp/src/buffer.c libcwmp/src/cfg.c libcwmp/src/cwmp.c libcwmp/src/http.c libcwmp/src/ssl.c \
libcwmp/src/ini.c libcwmp/src/log.c libcwmp/src/md5.c libcwmp/src/memory.c libcwmp/src/session.c libcwmp/src/util.c \
libcwmp/src/event.c libcwmp/src/queue.c libcwmp/src/model.c libcwmp/src/callback.c
LOCAL_STATIC_LIBRARIES := libpool libxmlet
LOCAL_CFLAGS += -DUSE_CWMP_MEMORY_POOL -DSTB_CMCC_MODEL
LOCAL_LDLIBS := \
	-lz \
	-lm 
	
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE    := cwmpd
LOCAL_SRC_FILES := cwmpd/src/conf.c cwmpd/src/cwmpd.c cwmpd/src/httpd.c cwmpd/src/process.c cwmpd/src/thread.c \
cwmpd/src/agent.c cwmpd/src/data_model.c


LOCAL_STATIC_LIBRARIES := libcwmp
LOCAL_CFLAGS += -DUSE_CWMP_MEMORY_POOL \
	-D_GNU_SOURCE  -DSTB_CMCC_MODEL

LOCAL_LDLIBS += \
	-lz \
	-lm \
	




include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE    := cwmpdlib
LOCAL_SRC_FILES := cwmpd/src/conf.c cwmpd/src/cwmpd.c cwmpd/src/httpd.c cwmpd/src/process.c cwmpd/src/thread.c \
cwmpd/src/agent.c cwmpd/src/data_model.c


LOCAL_STATIC_LIBRARIES := libcwmp
LOCAL_CFLAGS += -DUSE_CWMP_MEMORY_POOL \
	-D_GNU_SOURCE  -DSTB_CMCC_MODEL

LOCAL_LDLIBS += \
	-lz \
	-lm \

include $(BUILD_EXECUTABLE)



include $(CLEAR_VARS)

LOCAL_MODULE    := readsys
LOCAL_SRC_FILES := Readsys.c

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_MODULE    := usb_permission_yijiakang
LOCAL_SRC_FILES := usb_permission_yijiakang.c

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_LDLIBS    := -lm -llog 
LOCAL_MODULE    := tr111
LOCAL_SRC_FILES := tr111/tr111.c com_zhangwoshenghuo_netcwmp_JniInter.c cwmpd/src/conf.c cwmpd/src/cwmpd.c cwmpd/src/httpd.c cwmpd/src/process.c cwmpd/src/thread.c \
cwmpd/src/agent.c cwmpd/src/data_model.c
LOCAL_SHARED_LIBRARIES := libcrypto libssl libcwmp

LOCAL_CFLAGS += -DUSE_CWMP_MEMORY_POOL \
	-D_GNU_SOURCE  -DSTB_CMCC_MODEL
LOCAL_LDLIBS += \
	-lz \
	-lm \

include $(BUILD_SHARED_LIBRARY)


