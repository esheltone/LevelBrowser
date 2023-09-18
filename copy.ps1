& $PSScriptRoot/build.ps1
if ($?) {
    adb push build/liblevelbrowser.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/liblevelbrowser.so
    adb push ExtraFiles/Icons/Speed.png /sdcard/ModData/com.beatgames.beatsaber/Mods/LevelBrowser/Icons/Speed.png
    adb push ExtraFiles/Icons/Graph.png /sdcard/ModData/com.beatgames.beatsaber/Mods/LevelBrowser/Icons/Graph.png
    adb push ExtraFiles/Icons/X.png /sdcard/ModData/com.beatgames.beatsaber/Mods/LevelBrowser/Icons/X.png
    adb push ExtraFiles/Icons/StarFull.png /sdcard/ModData/com.beatgames.beatsaber/Mods/LevelBrowser/Icons/StarFull.png
    adb push ExtraFiles/Icons/DeleteIcon.png /sdcard/ModData/com.beatgames.beatsaber/Mods/LevelBrowser/Icons/DeleteIcon.png
    adb push ExtraFiles/Icons/DoubleArrow.png /sdcard/ModData/com.beatgames.beatsaber/Mods/LevelBrowser/Icons/DoubleArrow.png
    adb push ExtraFiles/Icons/RandomIcon.png /sdcard/ModData/com.beatgames.beatsaber/Mods/LevelBrowser/Icons/RandomIcon.png
    adb push ExtraFiles/Icons/NoteStartOffset.png /sdcard/ModData/com.beatgames.beatsaber/Mods/LevelBrowser/Icons/NoteStartOffset.png
    adb push ExtraFiles/Icons/PlaylistIcon.png /sdcard/ModData/com.beatgames.beatsaber/Mods/LevelBrowser/Icons/PlaylistIcon.png
    if ($?) {
        adb shell am force-stop com.beatgames.beatsaber
        adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
        if ($args[0] -eq "--log") {
            $timestamp = Get-Date -Format "MM-dd HH:mm:ss.fff"
            adb logcat -c
            adb logcat -T "$timestamp" main-modloader:W QuestHook[Tracks`|v0.1.0]:* QuestHook[UtilsLogger`|v1.0.12]:* AndroidRuntime:E *:S
        }
    }
}
