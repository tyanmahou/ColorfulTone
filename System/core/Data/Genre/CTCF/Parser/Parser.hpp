#pragma once
#include <core/Data/Genre/CTCF/Lexer/Token.hpp>

#include <core/Data/Genre/CTCF/AST/Root.hpp>
#include <core/Data/Genre/CTCF/AST/Statement/ExprStatement.hpp>
#include <core/Data/Genre/CTCF/AST/Expression/IdentifierValue.hpp>
#include <core/Data/Genre/CTCF/AST/Expression/Value.hpp>
#include <core/Data/Genre/CTCF/AST/Expression/UnaryExpr.hpp>
#include <core/Data/Genre/CTCF/AST/Expression/BinaryExpr.hpp>

#include <core/Data/Genre/CTCF/Parser/Precedence.hpp>
#include <Siv3D.hpp>

namespace ct::ctcf
{
    using namespace AST;
    class Parser
    {
        using AstItr = Array<Token>::const_iterator;
    public:
        Parser(const Array<Token>& tokens):
            m_tokens(tokens)
        {}
        std::shared_ptr<Root> root()
        {
            if (m_root) {
                return m_root;
            }
            auto it = m_tokens.begin();
            return m_root = this->parseRoot(it);
        }
    private:
        std::shared_ptr<Root> parseRoot(AstItr& it)
        {
            auto root = std::make_shared<Root>();
            root->statement = this->parseStatement(it);
            return root;
        }
        std::shared_ptr<IStatement> parseStatement(AstItr& it)
        {
            return this->parseExprStatement(it);
        }
        std::shared_ptr<ExprStatement> parseExprStatement(AstItr& it)
        {
            auto statement = std::make_shared<ExprStatement>();
            statement->expression = this->parseExpr(it, Precedence::Lowest);
            return statement;
        }
        std::shared_ptr<IExpression> parseExpr(AstItr& it, Precedence precedence)
        {
            static std::unordered_map<TokenType, decltype(&Parser::parseUnaryExpr)> prefixOp{
                {TokenType::Not, &Parser::parseUnaryExpr},
                {TokenType::ParenL, &Parser::parseParenExpr},

                {TokenType::Number, &Parser::parseValue},
                {TokenType::String, &Parser::parseValue},
                {TokenType::IdentifierValue, &Parser::parseIdentifierValue},
            };

            static std::unordered_map<TokenType, decltype(&Parser::parseBinaryExpr)> binaryOp {
                {TokenType::And, &Parser::parseBinaryExpr},
                {TokenType::Or, &Parser::parseBinaryExpr},
                {TokenType::Op, &Parser::parseBinaryExpr},
            };
            if (prefixOp.find(it->type) == prefixOp.end()) {
                return nullptr;
            }
            auto left = (this->*prefixOp[it->type])(it);
            while ((it + 1) != m_tokens.end() && precedence > ToPrecedence((it + 1)->type)) {
                auto next = it + 1;
                if (binaryOp.find(next->type) == binaryOp.end()) {
                    return left;
                }
                left = (this->*binaryOp[next->type])(++it, left);
            }

            return left;
        }

        std::shared_ptr<IExpression> parseUnaryExpr(AstItr& it)
        {
            const auto& op = (it++)->token;
            auto right = this->parseExpr(it, Precedence::Not);
            return std::make_shared<UnaryExpr>(op, right);
        }
        std::shared_ptr<IExpression> parseIdentifierValue(AstItr& it)
        {
            return std::make_shared<IdentifierValue>(*it);
        }
        std::shared_ptr<IExpression> parseValue(AstItr& it)
        {
            return std::make_shared<Value>(*it);
        }
        std::shared_ptr<IExpression> parseBinaryExpr(AstItr& it, std::shared_ptr<IExpression> left)
        {
            auto precedence = ToPrecedence(it->type);
            const auto& op = (it++)->token;
            auto right = this->parseExpr(it, precedence);
            return std::make_shared<BinaryExpr>(left, op, right);
        }

        std::shared_ptr<IExpression> parseParenExpr(AstItr& it)
        {
            auto expr = this->parseExpr(++it, Precedence::Lowest);

            if ((++it)->type != TokenType::ParenR) {
                Print << U"ctfolder parse error: not found ')'";
            }

            return expr;
        }
    private:
        const Array<Token>& m_tokens;
        std::shared_ptr<AST::Root> m_root = nullptr;
    };
}