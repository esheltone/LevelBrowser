
#include "Utils/SongDataCoreUtils.hpp"

#include <thread>
#include <cmath>
#include <map>
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
 
    const BeatStarSong* BeatStarSong::GetSong(std::string_view hash)
    {
        return reinterpret_cast<const BeatStarSong*>(song_data_core::Beatstar_GetSong(hash.data()));
    }

    float BeatStarSong::maxNJS() const
    {
        auto diffVec = GetDiffVec();
        float max = 0.0f;
        for (auto diff : diffVec)
        {
            float njs = diff->njs;
            if (njs > max) max = njs;
        }
        return max;
    }

    double BeatStarSong::maxPpValue() const
    {
        auto diffVec = GetDiffVec();
        double max = 0.0f;
        for (auto diff : diffVec)
        {
            double pp = diff->approximate_pp_value;
            if (pp > max) max = pp;
        }

        return max;
    }

    double BeatStarSong::maxStarValue() const
    {
        auto diffVec = GetDiffVec();
        double max = 0.0f;
        for (auto diff : diffVec) 
        {
            if (diff->stars > max) max = diff->stars;
        }
        return max;
    }

    const BeatStarSongDifficultyStats* BeatStarSong::GetDiff(int idx) const
    {
        return reinterpret_cast<const BeatStarSongDifficultyStats*>(song_data_core::BeatStarSong_DiffGet(this, idx));
    }

    const BeatStarSongDifficultyStats* BeatStarSong::GetDiff(const BeatStarCharacteristics* characteristic, int idx) const
    {
        char* key = const_cast<char*>(song_data_core::BeatStarSong_map_Characteristics_DifficultyStatsGetStrKey(this, characteristic, idx));
        return reinterpret_cast<const BeatStarSongDifficultyStats*>(song_data_core::BeatStarSong_map_Characteristics_DifficultyStatsGet(this, characteristic, key));
    }

    const BeatStarSongDifficultyStats* BeatStarSong::GetDiff(const BeatStarCharacteristics* characteristic, std::string_view name) const
    {
        int charLen = song_data_core::BeatStarSong_map_Characteristics_DifficultyStatsLen(this, characteristic);
        INFO("Characteristic %p had %d diffs", characteristic, charLen);
        for (int i = 0; i < charLen; i++)
        {
            auto diff = GetDiff(characteristic, i);
            if (!diff) continue;
            //INFO("Comparing %s to %s", diff->diff.string_data, name.data());
            if (!strcmp(diff->diff.string_data, name.data())) return diff;
        }
        return nullptr;
    }

    std::vector<const BeatStarSongDifficultyStats*> BeatStarSong::GetDiffVec() const
    {
        int difflen = song_data_core::BeatStarSong_DiffLen(this);
        std::vector<const BeatStarSongDifficultyStats*> vec;
        for (int i = 0; i < difflen; i++)
        {
            vec.push_back(GetDiff(i));
        }

        return vec;
    }

    const BeatStarCharacteristics* BeatStarSong::GetChar(int char_) const
    {
        return reinterpret_cast<const BeatStarCharacteristics*>(song_data_core::BeatStarSong_map_CharacteristicsKeyGet(this, char_));
    }
    
    BeatStarCharacteristics CharStringToEnum(std::string characteristic)
    {
        switch(characteristic.c_str()[0])
        {
            case 'S': return BeatStarCharacteristics::Standard;
            case 'O': return BeatStarCharacteristics::OneSaber;
            case 'N': return BeatStarCharacteristics::NoArrows;
            case 'D':
                if (characteristic.c_str()[6] == '9') return BeatStarCharacteristics::Degree90;
                else return BeatStarCharacteristics::Degree360;
            case 'L': 
                if (tolower(characteristic.c_str()[1]) == 'a') return BeatStarCharacteristics::Lawless;
                else return BeatStarCharacteristics::Lightshow;
            default: return BeatStarCharacteristics::Unknown;
        }
    }

    const BeatStarCharacteristics* BeatStarSong::GetChar(GlobalNamespace::BeatmapCharacteristicSO* gameChar) const
    {
        auto mine = CharStringToEnum(to_utf8(csstrtostr(gameChar->get_serializedName())));
        int chars = song_data_core::BeatStarSong_map_CharacteristicsLen(this);

        for (int i = 0; i < chars; i++)
        {
            auto their = GetChar(i);
            if (mine == *their) return their;
        }
        return nullptr;
    }

    std::map<const BeatStarSong*, float> heatmap;
    float BeatStarSong::GetHeat() const
    {
        // find song in heatmap so that we dont just recalculate every time
        auto itr = heatmap.find(this);
        if (itr != heatmap.end())
        {
            return itr->second;
        }

        std::string date(uploaded.string_data);

        // optimization so that we only do the calculation once
        static time_t time_past_epoch = time(0) - BEATSAVER_EPOCH;

        struct tm tm;
        memset(&tm, 0, sizeof(struct tm));
        strptime(uploaded.string_data, "%FT%T%z", &tm);
        time_t uploaded = mktime(&tm);

        int seconds = uploaded - time_past_epoch;

        int score = upvotes - downvotes;
        int absolute = abs(score);
        int sign = score < 0 ? -1 : score > 0 ? 1 : 0;

        double order = log10(absolute > 1.0f ? absolute : 1);
        float heat = (float)sign * order + (float)seconds / 45000;

        heatmap[this] = heat;
        return heat;
    }

    float BeatStarSong::GetRating() const
    {
        return song_data_core::BeatStarSong_rating(this);
    }

    std::vector<std::string> BeatStarSongDifficultyStats::GetReqVec() const
    {
        int len = BeatStarSongDifficultyStats_requirementsLen(this);
        std::vector<std::string> vec;
        for (int i = 0; i < len; i++)
        {
            auto stringWrapper = BeatStarSongDifficultyStats_requirementsGet(this, i);
            vec.push_back(std::string(stringWrapper->string_data));
        }

        return vec;
    }
}