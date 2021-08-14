#include "Utils/SongDataCoreUtils.hpp"
#include <thread>
#include <cmath>
#include "logging.hpp"
#include "System/DateTime.hpp"

#define BEATSAVER_EPOCH 1525132800

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

    double approximatePpValue(const BeatStarSong* song)
    {
        auto diffVec = GetDiffVec(song);
        double pp = 0.0f;
        for (auto diff : diffVec) pp += approximatePpValue(diff);
        return pp; 
    }

    double maxPpValue(const BeatStarSong* song)
    {
        auto diffVec = GetDiffVec(song);
        double max = 0.0f;
        for (auto diff : diffVec) 
        {
            double pp = approximatePpValue(diff);
            if (pp > max) max = pp;
        }

        return max;
    }

    double maxStarValue(const BeatStarSong* song)
    {
        auto diffVec = GetDiffVec(song);
        double max = 0.0f;
        for (auto diff : diffVec) 
        {
            if (diff->stars > max) max = diff->stars;
        }
        return max;
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

    std::vector<std::string> GetReqVec(const BeatStarSongDifficultyStats* diff)
    {
        int len = BeatStarSongDifficultyStats_requirementsLen(diff);
        std::vector<std::string> vec;
        for (int i = 0; i < len; i++)
        {
            auto stringWrapper = BeatStarSongDifficultyStats_requirementsGet(diff, i);
            vec.push_back(std::string(stringWrapper->string_data));
        }

        return vec;
    }

    float GetRating(const BeatStarSong* song)
    {
        return song_data_core::BeatStarSong_rating(song);
    }

    float GetHeat(const BeatStarSong* song)
    {
        #warning not implemented
        INFO("uploaded: %s", song->uploaded.string_data);

        static System::DateTime EPOCH = System::DateTime(1970, 1, 1);
        //(this.uploaded.getTime() - epoch.getTime()) / 1000 - BEATSAVER_EPOCH
        int seconds =
          (EPOCH.get_Second() / 1000 - BEATSAVER_EPOCH);

        int score = song->upvotes - song->downvotes;
        int absolute = abs(score);
        int sign = score < 0 ? -1 : score > 0 ? 1 : 0;

        double order = log10(absolute > 1.0f ? absolute : 1);
        float heat = (float)sign * order + (float)seconds / 45000;

        return heat;

    }
}