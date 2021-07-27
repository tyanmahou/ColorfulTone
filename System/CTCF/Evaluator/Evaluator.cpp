#include "Evaluator.hpp"
#include "../AST/Root.hpp"
#include "../AST/Statement/ExprStatement.hpp"
#include "../AST/Expression/IdentifierValue.hpp"
#include "../AST/Expression/Value.hpp"
#include "../AST/Expression/UnaryExpr.hpp"
#include "../AST/Expression/BinaryExpr.hpp"

#include"MusicData.h"
#include "ResultRank.h"

namespace
{
#define RETURN_COMPARE(OP) \
	if(op == L""#OP) { \
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
        if (op == L"in") {
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
    using namespace ctcf;
    using namespace ctcf::AST;

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
            if (node->op == L"!") {
                return !this->eval(node->right.get());
            }
            return this->eval(node->right.get());
        }
        bool compare(Value* lValue, const String& op, Value* rValue)
        {
            auto type = lValue->token.type;
            const auto& value = rValue->token.token;
            const auto* const music = m_notes.getMusic();
            if (type == TokenType::Bpm) {
                auto bpm = Parse<double>(value);
                return ::Compare(music->getMinBPM(), bpm, op) || ::Compare(music->getMaxBPM(), bpm, op);
            }
            if (type == TokenType::MinBpm) {
                return ::Compare(music->getMinBPM(), Parse<double>(value), op);
            }
            if (type == TokenType::MaxBpm) {
                return ::Compare(music->getMaxBPM(), Parse<double>(value), op);
            }
            if (type == TokenType::Artist) {
                return ::Compare(music->getArtistName(), value, op);
            }
            if (type == TokenType::Authority) {
                const auto& authority = music->getAuthority();
                if (authority.has_value() && !authority.value().isEmpty) {
                    return ::Compare(authority.value(), value, op);
                }
                return false;
            }
            if (type == TokenType::MusicName) {
                return ::Compare(music->getMusicName(), value, op);
            }
            if (type == TokenType::Genre) {
                return ::Compare(music->getGenreName(), value, op);
            }
            if (type == TokenType::Level) {
                int level = Parse<int>(value);
                return ::Compare(m_notes.getLevel(), level, op);
            }
            if (type == TokenType::LevelName) {
                return ::Compare(m_notes.getLevelName(), value, op);
            }
            if (type == TokenType::Star) {
                return ::Compare(ToStr(m_notes.getStarLv()), value, op);
            }
            if (type == TokenType::Note) {
                return ::Compare(m_notes.getNotesArtistName(), value, op);
            }
            if (type == TokenType::TotalNote) {
                int total = Parse<int>(value);
                return ::Compare(m_notes.getTotalNotes(), total, op);
            }
            if (type == TokenType::ClearRate) {
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
            if (type == TokenType::Number) {
                return ::Compare(Parse<double>(lValue->token.token), Parse<double>(value), op);
            }
            if (type == TokenType::String) {
                return ::Compare(lValue->token.token, value, op);
            }
            return false;
        }
        bool evalBinaryExpr(BinaryExpr* node)
        {
            if (node->op == L"&&") {
                return this->eval(node->left.get()) && this->eval(node->right.get());
            }
            if (node->op == L"||") {
                return this->eval(node->left.get()) || this->eval(node->right.get());
            }
            auto valueL = dynamic_cast<Value*>(node->left.get());
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
            auto type = node->token.type;
            if (type == TokenType::Clear) {
                return m_notes.getScore().isClear;
            }
            if (type == TokenType::AP || type == TokenType::FC) {
                SpecialResult s = SpecialResult::None;
                if (type == TokenType::AP) {
                    s = SpecialResult::All_Perfect;
                } else if (type == TokenType::FC) {
                    s = SpecialResult::Full_Combo;
                }
                return m_notes.getScore().specialResult >= s;
            }
            if (type == TokenType::Favorite) {
                return m_notes.getMusic()->isFavorite();
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
namespace ctcf
{
    bool Evaluator::eval(AST::INode* node)
    {
        return EvalImpl(m_notes).eval(node);
    }
}