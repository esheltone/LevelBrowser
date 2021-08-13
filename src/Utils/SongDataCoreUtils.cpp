#include "Utils/SongDataCoreUtils.hpp"
#include <thread>
#include "logging.hpp"

namespace SongDataCoreUtils
{
    bool loaded = false;
    void Init()
    {
        std::thread databaseThread([&](){
            INFO("Starting song data core database retrieval");
            song_data_core::Beatstar_RetrieveDatabase();
            loaded = true;
        });
        databaseThread.detach();
    }

    bool get_loaded()
    {
        return loaded;
    }
    double approximatePpValue(const BeatStarSongDifficultyStats* diff)
    {
        if(!diff || diff->stars <= 0.05 || !diff->ranked)
            return 0;

        return diff->stars * (45.0f + ((10.0f - diff->stars) / 7.0f));
    }

    const BeatStarSong* GetSong(const std::string_view& hash)
    {
        INFO("Getting beatstar song with hash %s", hash.data());
        return song_data_core::Beatstar_GetSong(hash.data());
    }

    const BeatStarSongDifficultyStats* GetDiff(const BeatStarSong* song, int diff)
    {
        return song_data_core::BeatStarSong_DiffGet(song, diff);
    }

    std::vector<const BeatStarSongDifficultyStats*> GetDiffVec(const BeatStarSong* song)
    {
        int difflen = song_data_core::BeatStarSong_DiffLen(song);
        std::vector<const BeatStarSongDifficultyStats*> vec;
        for (int i = 0; i < difflen; i++)
        {
            vec.push_back(GetDiff(song, i));
        }

        return vec;
    }
}