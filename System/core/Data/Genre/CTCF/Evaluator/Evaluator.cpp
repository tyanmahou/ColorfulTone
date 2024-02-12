#include <core/Data/Genre/CTCF/Evaluator/Evaluator.hpp>

#include <core/Data/Genre/CTCF/AST/Root.hpp>
#include <core/Data/Genre/CTCF/AST/Statement/ExprStatement.hpp>
#include <core/Data/Genre/CTCF/AST/Expression/IdentifierValue.hpp>
#include <core/Data/Genre/CTCF/AST/Expression/Value.hpp>
#include <core/Data/Genre/CTCF/AST/Expression/UnaryExpr.hpp>
#include <core/Data/Genre/CTCF/AST/Expression/BinaryExpr.hpp>

#include <core/Data/MusicData/MusicData.hpp>
#include <core/Data/Score/ResultRank.hpp>
#include <core/Data/Score/SpecialResult.hpp>
#include <Siv3D.hpp>

namespace
{
    using namespace ct;
#define RETURN_COMPARE(OP) \
	if(op == U""#OP) { \
		return a OP b; \
	}

    template<class _T>
    bool Compare(const _T& a, const _T& b, const String& op)
    {
        RETURN_COMPARE(== );
        RETURN_COMPARE(!= );
        RETURN_COMPARE(< );
        RETURN_COMPARE(<= );
        RETURN_COMPARE(> );
        RETURN_COMPARE(>= );
        return false;
    }

    template<>
    bool Compare(const String& a, const String& b, const String& op)
    {
        RETURN_COMPARE(== );
        RETURN_COMPARE(!= );
        RETURN_COMPARE(< );
        RETURN_COMPARE(<= );
        RETURN_COMPARE(> );
        RETURN_COMPARE(>= );
        if (op == U"in") {
            return a.includes(b);
        }
        return false;
    }
    template<>
    bool Compare(const SpecialResult& a, const SpecialResult& b, const String& op)
    {
        RETURN_COMPARE(!= );
        RETURN_COMPARE(== );
        return false;
    }
#undef RETURN_COMPARE
}
namespace
{
    using namespace ct;
    using namespace ct::ctcf;
    using namespace ct::ctcf::AST;

    class EvalImpl
    {
    public:
        EvalImpl(const NotesData& notes):
            m_notes(notes)
        {}
        bool eval(INode* node)
        {
            // 文
            if (auto* root = dynamic_cast<Root*>(node)) {
                return eval(root->statement.get());
            }
            if (auto* statement = dynamic_cast<ExprStatement*>(node)) {
                return eval(statement->expression.get());
            }

            // 式
            if (auto* expr = dynamic_cast<IdentifierValue*>(node)) {
                return evalIdentiferValue(expr);
            }
            if (auto* expr = dynamic_cast<Value*>(node)) {
                return evalValue(expr);
            }
            if (auto* expr = dynamic_cast<UnaryExpr*>(node)) {
                return evalUnaryExpr(expr);
            }
            if (auto* expr = dynamic_cast<BinaryExpr*>(node)) {
                return evalBinaryExpr(expr);
            }
            return false;
        }
        bool evalUnaryExpr(UnaryExpr* node)
        {
            if (node->op == U"!") {
                return !this->eval(node->right.get());
            }
            return this->eval(node->right.get());
        }
        bool compare(IdentifierValue* lValue, const String& op, Value* rValue)
        {
            auto kind = lValue->kind;
            const auto& value = rValue->token.token;
            const auto* const music = m_notes.getMusic();
            if (kind == IdentifierValueKind::Bpm) {
                auto bpm = Parse<double>(value);
                return ::Compare(music->getMinBPM(), bpm, op) || ::Compare(music->getMaxBPM(), bpm, op);
            }
            if (kind == IdentifierValueKind::MinBpm) {
                return ::Compare(music->getMinBPM(), Parse<double>(value), op);
            }
            if (kind == IdentifierValueKind::MaxBpm) {
                return ::Compare(music->getMaxBPM(), Parse<double>(value), op);
            }
            if (kind == IdentifierValueKind::Artist) {
                return ::Compare(music->getArtistName(), value, op);
            }
            if (kind == IdentifierValueKind::Authority) {
                const auto& authority = music->getAuthority();
                if (authority.has_value() && !authority.value().isEmpty()) {
                    return ::Compare(authority.value(), value, op);
                }
                return false;
            }
            if (kind == IdentifierValueKind::MusicName) {
                return ::Compare(music->getMusicName(), value, op);
            }
            if (kind == IdentifierValueKind::Genre) {
                return ::Compare(music->getGenreName(), value, op);
            }
            if (kind == IdentifierValueKind::Level) {
                int32 level = Parse<int32>(value);
                return ::Compare(m_notes.getLevel(), level, op);
            }
            if (kind == IdentifierValueKind::LevelName) {
                return ::Compare(m_notes.getLevelName(), value, op);
            }
            if (kind == IdentifierValueKind::Star) {
                return ::Compare(ToStr(m_notes.getStarLv()), value, op);
            }
            if (kind == IdentifierValueKind::Note) {
                return ::Compare(m_notes.getNotesArtistName(), value, op);
            }
            if (kind == IdentifierValueKind::TotalNote) {
                uint32 total = Parse<uint32>(value);
                return ::Compare(m_notes.getTotalNotes(), total, op);
            }
            if (kind == IdentifierValueKind::ClearRate) {
                auto rateOpt = ParseOpt<float>(value);
                if (!rateOpt) {
                    float rate = ResultRank::ToRate(value);
                    float rankBaseRate = ResultRank::ToRate(ResultRank::GetRankTextureName(m_notes.getScore().clearRate));
                    return ::Compare(rankBaseRate, rate, op);
                } else {
                    float rate = rateOpt.value();
                    return ::Compare(m_notes.getScore().clearRate, rate, op);
                }
            }
            return false;
        }
        bool evalBinaryExpr(BinaryExpr* node)
        {
            if (node->op == U"&&") {
                return this->eval(node->left.get()) && this->eval(node->right.get());
            }
            if (node->op == U"||") {
                return this->eval(node->left.get()) || this->eval(node->right.get());
            }
            auto valueL = dynamic_cast<IdentifierValue*>(node->left.get());
            auto valueR = dynamic_cast<Value*>(node->right.get());
            if (!valueL || !valueR) {
                return false;
            }
            return {
                this->compare(
                valueL,
                node->op,
                valueR
                )
            };
        }
        bool evalIdentiferValue(IdentifierValue* node)
        {
            auto kind = node->kind;
            if (kind == IdentifierValueKind::Clear) {
                return m_notes.getScore().isClear;
            }
            if (kind == IdentifierValueKind::AP || kind == IdentifierValueKind::FC) {
                SpecialResult s = SpecialResult::None;
                if (kind == IdentifierValueKind::AP) {
                    s = SpecialResult::All_Perfect;
                } else if (kind == IdentifierValueKind::FC) {
                    s = SpecialResult::Full_Combo;
                }
                return m_notes.getScore().specialResult >= s;
            }
            if (kind == IdentifierValueKind::Favorite) {
                return m_notes.getMusic()->isFavorite();
            }
            if (kind == IdentifierValueKind::Official) {
                return m_notes.getMusic()->isOfficial();
            }
            return false;
        }
        bool evalValue(Value* node)
        {
            return s3d::ParseOr<bool>(node->token.token, false);
        }
    private:
        const NotesData& m_notes;
    };

}
namespace ct::ctcf
{
    bool Evaluator::eval(AST::INode* node)
    {
        return EvalImpl(m_notes).eval(node);
    }
}