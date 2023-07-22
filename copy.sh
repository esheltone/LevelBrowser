#!/bin/bash

NDKPath=`cat ndkpath.txt`

buildScript="$NDKPath/build/ndk-build"

./$buildScript NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk -j8
adb push libs/arm64-v8a/libsongbrowser.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libsongbrowser.so
adb push extern/libs/libandroid-libsong_data_core_rust.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libandroid-libsong_data_core_rust.so
adb push ExtraFiles/Icons/Speed.png /sdcard/ModData/com.beatgames.beatsaber/Mods/SongBrowser/Icons/Speed.png
adb push ExtraFiles/Icons/Graph.png /sdcard/ModData/com.beatgames.beatsaber/Mods/SongBrowser/Icons/Graph.png
adb push ExtraFiles/Icons/X.png /sdcard/ModData/com.beatgames.beatsaber/Mods/SongBrowser/Icons/X.png
adb push ExtraFiles/Icons/StarFull.png /sdcard/ModData/com.beatgames.beatsaber/Mods/SongBrowser/Icons/StarFull.png
adb push ExtraFiles/Icons/DeleteIcon.png /sdcard/ModData/com.beatgames.beatsaber/Mods/SongBrowser/Icons/DeleteIcon.png
adb push ExtraFiles/Icons/DoubleArrow.png /sdcard/ModData/com.beatgames.beatsaber/Mods/SongBrowser/Icons/DoubleArrow.png
adb push ExtraFiles/Icons/RandomIcon.png /sdcard/ModData/com.beatgames.beatsaber/Mods/SongBrowser/Icons/RandomIcon.png
adb push ExtraFiles/Icons/NoteStartOffset.png /sdcard/ModData/com.beatgames.beatsaber/Mods/SongBrowser/Icons/NoteStartOffset.png
adb push ExtraFiles/Icons/PlaylistIcon.png /sdcard/ModData/com.beatgames.beatsaber/Mods/SongBrowser/Icons/PlaylistIcon.png
adb shell am force-stop com.beatgames.beatsaber
adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity

adb logcat -c && adb logcat > test.log