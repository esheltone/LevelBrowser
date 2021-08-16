#pragma once

#include <string_view>
#include <vector>
#include "songdatacore/shared/bindings.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"

// this is just a wrapper around the bindings for songdatacore cause this looks a bit better :) 
namespace SongDataCoreUtils
{
    struct BeatStarSong;
    struct BeatStarSongDifficultyStats;
    using BeatStarCharacteristics = song_data_core::BeatStarCharacteristics;
    
    struct BeatStarSong : public song_data_core::BeatStarSong
    {
        public:
            static const BeatStarSong* GetSong(std::string_view hash);
            float maxNJS() const;
            double maxPpValue() const;
            double maxStarValue() const;
            
            const BeatStarSongDifficultyStats* GetDiff(int idx) const;
            const BeatStarSongDifficultyStats* GetDiff(const BeatStarCharacteristics* characteristic, int idx) const;
            const BeatStarSongDifficultyStats* GetDiff(const BeatStarCharacteristics* characteristic, std::string_view name) const;
            
            std::vector<const BeatStarSongDifficultyStats*> GetDiffVec() const;
            const BeatStarCharacteristics* GetChar(int char_) const;
            const BeatStarCharacteristics* GetChar(GlobalNamespace::BeatmapCharacteristicSO* gameChar) const;
            float GetHeat() const;
            float GetRating() const;
    };

    struct BeatStarSongDifficultyStats : public song_data_core::BeatStarSongDifficultyStats
    {
        std::vector<std::string> GetReqVec() const;
    };

    void Init();
    bool get_loaded();
}