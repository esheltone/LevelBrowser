& ./build.ps1
& adb push libs/arm64-v8a/libsongbrowser.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libsongbrowser.so
& Start-Sleep -Milliseconds 500
& adb shell am force-stop com.beatgames.beatsaber
& adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
& ./log.ps1