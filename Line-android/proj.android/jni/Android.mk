LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES :=\
	hellocpp/main.cpp \
   ../../Classes/AppDelegate.cpp \
   ../../Classes/HelloWorldScene.cpp\
   ../../Classes/WaitingScene.cpp\
   ../../Classes/LoadingBar.cpp\
   ../../Classes/Generate.cpp\
   ../../Classes/MoveControl.cpp\
   ../../Classes/ShowControl.cpp\
   ../../Classes/BackControl.cpp\
   ../../Classes/CurlTest.cpp\
   ../../Classes/enet/callbacks.c\
   ../../Classes/enet/compress.c\
   ../../Classes/enet/host.c\
   ../../Classes/enet/list.c\
   ../../Classes/enet/packet.c\
   ../../Classes/enet/peer.c\
   ../../Classes/enet/protocol.c\
   ../../Classes/enet/unix.c\
   ../../Classes/enet/win32.c
				   				   
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes                   

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)
