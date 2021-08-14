#pragma once

#include <string_view>
#include <vector>
#include "songdatacore/shared/bindings.hpp"

// this is just a wrapper around the bindings for songdatacore cause this looks a bit better :) 
namespace SongDataCoreUtils
{
    using BeatStarSong = song_data_core::BeatStarSong;
    using BeatStarSongDifficultyStats = song_data_core::BeatStarSongDifficultyStats;

    void Init();
    bool get_loaded();

    float GetHeat(const BeatStarSong* song);
    float GetRating(const BeatStarSong* song);

    double approximatePpValue(const BeatStarSongDifficultyStats* diff);
    double approximatePpValue(const BeatStarSong* song);

    double maxPpValue(const BeatStarSong* song);
    double maxStarValue(const BeatStarSong* song);

    const BeatStarSong* GetSong(const std::string_view& hash);
    const BeatStarSongDifficultyStats* GetDiff(const BeatStarSong* song, int diff);

    std::vector<const BeatStarSongDifficultyStats*> GetDiffVec(const BeatStarSong* song);
    std::vector<std::string> GetReqVec(const BeatStarSongDifficultyStats* diff);
}