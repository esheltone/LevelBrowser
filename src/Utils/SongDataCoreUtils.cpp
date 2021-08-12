#include "Utils/SongDataCoreUtils.hpp"
#include <thread>

namespace SongDataCoreUtils
{
    bool loaded = false;
    void Init()
    {
        std::thread databaseThread([&](){
            song_data_core::Beatstar_RetrieveDatabase();
            loaded = true;
        });
        databaseThread.detach();
    }

    bool get_loaded()
    {
        return loaded;
    }

    const BeatStarSong* GetSong(const std::string_view& hash)
    {
        return song_data_core::Beatstar_GetSong(hash.data());
    }

    const BeatStarSongDifficultyStats* GetDiff(const BeatStarSong* song, int diff)
    {
        return song_data_core::BeatStarSong_DiffGet(song, diff);
    }
}