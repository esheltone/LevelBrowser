# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


LOCAL_PATH := $(call my-dir)
TARGET_ARCH_ABI := $(APP_ABI)

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Creating prebuilt for dependency: modloader - version: 1.2.3
include $(CLEAR_VARS)
LOCAL_MODULE := modloader
LOCAL_EXPORT_C_INCLUDES := extern/includes/modloader
LOCAL_SRC_FILES := extern/libs/libmodloader.so
LOCAL_CPP_FEATURES += rtti exceptions
include $(PREBUILT_SHARED_LIBRARY)
# Creating prebuilt for dependency: beatsaber-hook - version: 3.8.1
include $(CLEAR_VARS)
LOCAL_MODULE := beatsaber-hook_3_8_1
LOCAL_EXPORT_C_INCLUDES := extern/includes/beatsaber-hook
LOCAL_SRC_FILES := extern/libs/libbeatsaber-hook_3_8_1.so
include $(PREBUILT_SHARED_LIBRARY)
# Creating prebuilt for dependency: custom-types - version: 0.15.9
include $(CLEAR_VARS)
LOCAL_MODULE := custom-types
LOCAL_EXPORT_C_INCLUDES := extern/includes/custom-types
LOCAL_SRC_FILES := extern/libs/libcustom-types.so
include $(PREBUILT_SHARED_LIBRARY)
# Creating prebuilt for dependency: questui - version: 0.11.1
include $(CLEAR_VARS)
LOCAL_MODULE := questui
LOCAL_EXPORT_C_INCLUDES := extern/includes/questui
LOCAL_SRC_FILES := extern/libs/libquestui.so
include $(PREBUILT_SHARED_LIBRARY)
# Creating prebuilt for dependency: songloader - version: 0.7.1
include $(CLEAR_VARS)
LOCAL_MODULE := songloader
LOCAL_EXPORT_C_INCLUDES := extern/includes/songloader
LOCAL_SRC_FILES := extern/libs/libsongloader.so
include $(PREBUILT_SHARED_LIBRARY)
# Creating prebuilt for dependency: songdatacore - version: 0.4.1
include $(CLEAR_VARS)
LOCAL_MODULE := android-libsong_data_core_rust
LOCAL_EXPORT_C_INCLUDES := extern/includes/songdatacore
LOCAL_SRC_FILES := extern/libs/libandroid-libsong_data_core_rust.so
include $(PREBUILT_SHARED_LIBRARY)
# Creating prebuilt for dependency: codegen - version: 0.22.0
include $(CLEAR_VARS)
LOCAL_MODULE := codegen
LOCAL_EXPORT_C_INCLUDES := extern/includes/codegen
LOCAL_SRC_FILES := extern/libs/libcodegen.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := songbrowser
LOCAL_SRC_FILES += $(call rwildcard,src/**,*.cpp)
LOCAL_SRC_FILES += $(call rwildcard,extern/includes/beatsaber-hook/src/inline-hook,*.cpp)
LOCAL_SRC_FILES += $(call rwildcard,extern/includes/beatsaber-hook/src/inline-hook,*.c)
LOCAL_SHARED_LIBRARIES += modloader
LOCAL_SHARED_LIBRARIES += beatsaber-hook_3_8_1
LOCAL_SHARED_LIBRARIES += custom-types
LOCAL_SHARED_LIBRARIES += questui
LOCAL_SHARED_LIBRARIES += songloader
LOCAL_SHARED_LIBRARIES += android-libsong_data_core_rust
LOCAL_SHARED_LIBRARIES += codegen
LOCAL_LDLIBS += -llog
LOCAL_CFLAGS += -DID='"SongBrowser"' -DVERSION='"1.0.2"' -I'./shared' -I'./extern' -Wno-inaccessible-base -O2
LOCAL_C_INCLUDES += ./include ./src ./extern/includes ./extern/includes/codegen/include ./extern/includes/libil2cpp/il2cpp/libil2cpp ./shared
LOCAL_CPP_FEATURES += rtti exceptions
include $(BUILD_SHARED_LIBRARY)
