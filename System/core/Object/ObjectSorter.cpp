﻿#include <core/Object/ObjectSorter.hpp>
#include <core/Object/Bar/Bar.hpp>
#include <core/Object/Text/TextObject.hpp>
#include <core/Object/RepeatNote/RepeatNote.hpp>
#include <commons/Game/Game.hpp>
#include <commons/Game/GameConfig.hpp>
#include <Siv3D.hpp>

namespace ct
{
    int64 JudgeOrder(int64 diff)
    {
        if (diff < -JudgeRange(Judge::Good)) {
            // 遅ミス
            return -3;
        }
        else if (diff < -JudgeRange(Judge::Great)) {
            // 遅Good
            return -2;
        }
        else if (diff < -JudgeRange(Judge::Perfect)) {
            // 遅Great
            return -1;
        } 
        else if (diff > JudgeRange(Judge::Good)) {
            // まだ
            return 3;
        }
        else if (diff > JudgeRange(Judge::Great)) {
            // 速Good
            return 2;
        }
        else if (diff > JudgeRange(Judge::Perfect)) {
            // 速Great
            return 1;
        }
        // Perfect
        return 0;
    }
    ObjectOrder ObjectOrder::Calc([[maybe_unused]] const Bar* obj, [[maybe_unused]]s3d::int64 sample)
    {
        // 何もしないので一番優先度低くする
        return {
            .isActive = false,
            .judge = 4,
            .objType = std::numeric_limits<int64>::max(),
            .kind = 0,
            .timming = 0
        };
    }
    ObjectOrder ObjectOrder::Calc(const Note* obj, s3d::int64 sample)
    {
        int64 diff = obj->getTimingSampleOffset(sample);
        int64 judgeOrder = JudgeOrder(diff);
        if (Game::Config().m_judgeAlgoKind == JudgeAlgorithmKind::Nearest) {
            diff = Abs(diff);
            judgeOrder = Abs(judgeOrder);
        }
        return {
            .isActive = obj->isActive(),
            .judge = judgeOrder,
            .objType = 0,
            // Perfectの白は優先度低い
            .kind = judgeOrder == 0 && obj->getType() == 9 ? 1 : 0,
            .timming = diff
        };
    }
    ObjectOrder ObjectOrder::Calc(const LongNote* obj, s3d::int64 sample)
    {
        int64 diff = obj->getTimingSampleOffset(sample);
        int64 judgeOrder = JudgeOrder(diff);
        return {
            .isActive = obj->isActive(),
            .judge = judgeOrder,
            .objType = 0,
            .kind = 0,
            .timming = diff
        };
    }
    ObjectOrder ObjectOrder::Calc(const TextObject* obj, s3d::int64 sample)
    {
        int64 diff = obj->getTimingSampleOffset(sample);
        int64 judgeOrder = JudgeOrder(diff);
        return {
            .isActive = obj->isActive(),
            .judge = judgeOrder,
            .objType = std::numeric_limits<int64>::min(),
            .kind = 0,
            .timming = diff
        };
    }

    std::strong_ordering ObjectOrder::operator<=>(const ObjectOrder& other) const
    {
        if (isActive != other.isActive) {
            if (isActive) {
                return std::strong_ordering::less;
            } else {
                return std::strong_ordering::greater;
            }
        }
        if (auto comp = judge <=> other.judge; comp != 0) {
            return comp;
        }
        if (auto comp = objType <=> other.objType; comp != 0) {
            return comp;
        }
        if (auto comp = kind <=> other.kind; comp != 0) {
            return comp;
        }
        return kind <=> other.kind;
    }

    bool ObjectSorter::operator()(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const
    {
        return a->getOrder(sample) < b->getOrder(sample);
    }
}

