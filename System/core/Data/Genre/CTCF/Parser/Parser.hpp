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
        Parser(const Lexer& lexer) :
            m_lexer(lexer),
            m_tokens(lexer.getTokens())
        {
        }
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
            while (!invalid(it)) {
                AstItr prevIt = it;
                auto statement = this->parseExprStatement(it);
                if (prevIt == it && !invalid(it)) {
                    // 無限ループ防止
                    ++it;
                }
                if (statement && statement->expression) {
                    if (statement->type == U"FILTER") {
                        root->filter = statement;
                    } else if (statement->type == U"SELECT") {
                        root->selector = statement;
                    }
                }
            }
            if ((!root->selector || !root->selector->expression)) {
                auto selector = std::make_shared<ExprStatement>();
                selector->type = U"SELECT";
                selector->mode = U"ALL";
                selector->expression = root->filter ? root->filter->expression : nullptr;
                root->selector = selector;
            }
            if (!root->filter && root->selector) {
                auto filter = std::make_shared<ExprStatement>();
                filter->type = U"FILTER";
                filter->mode = U"ANY";
                filter->expression = std::make_shared<Value>(Token(U"true"));
                root->filter = filter;
            }
            return root;
        }
        std::shared_ptr<ExprStatement> parseExprStatement(AstItr& it)
        {
            if (invalid(it)) return nullptr;
            bool a = false;
            auto statement = std::make_shared<ExprStatement>();
            if (it->type == TokenType::LBracket) {
                if (!next(it))return nullptr; // [
                statement->type = it->token;
                if (!next(it))return nullptr;
                if (it->type != TokenType::Colon) {
                    Print << U"ctfolder parse error: not found ':'";
                    return nullptr;
                }
                if (!next(it))return nullptr; // :
                statement->mode = it->token;
                if (!next(it))return nullptr;
                if (it->type != TokenType::RBracket) {
                    Print << U"ctfolder parse error: not found ']'";
                    return nullptr;
                }
                next(it); // ]
                a = true;
            } else {
                statement->type = U"FILTER";
                statement->mode = m_lexer.getOption(U"EVAL").value_or(U"ANY");
            }
            statement->expression = this->parseExpr(it, Precedence::Lowest);
            return statement;
        }
        using ParseUnaryFunc = std::shared_ptr<IExpression>(Parser::*)(AstItr&);
        ParseUnaryFunc findPrefixOp(TokenType tokenType)
        {
            static std::unordered_map<TokenType, decltype(&Parser::parseUnaryExpr)> prefixOp{
                {TokenType::Not, &Parser::parseUnaryExpr},
                {TokenType::ParenL, &Parser::parseParenExpr},

                {TokenType::Number, &Parser::parseValue},
                {TokenType::String, &Parser::parseValue},
                {TokenType::IdentifierValue, &Parser::parseIdentifierValue},
            };
            auto it = prefixOp.find(tokenType);
            if (it == prefixOp.end()) {
                return nullptr;
            }
            return it->second;
        }
        using ParseBinaryFunc = std::shared_ptr<IExpression>(Parser::*)(AstItr&, std::shared_ptr<IExpression>);
        ParseBinaryFunc findBinaryOp(TokenType tokenType)
        {
            static std::unordered_map<TokenType, decltype(&Parser::parseBinaryExpr)> binaryOp{
                {TokenType::And, &Parser::parseBinaryExpr},
                {TokenType::Or, &Parser::parseBinaryExpr},
                {TokenType::Op, &Parser::parseBinaryExpr},
            };
            auto it = binaryOp.find(tokenType);
            if (it == binaryOp.end()) {
                return nullptr;
            }
            return it->second;
        }
        bool needParseLeft(AstItr& it, Precedence precedence,  bool rightToLeft = false)
        {
            if (invalid(it)) {
                return false;
            }
            return rightToLeft
                ? (precedence >= ToPrecedence(it->type))
                : (precedence > ToPrecedence(it->type))
                ;
        }
        std::shared_ptr<IExpression> parseExpr(AstItr& it, Precedence precedence, bool rightToLeft = false)
        {
            if (invalid(it)) {
                return nullptr;
            }
            ParseUnaryFunc prefixOp = findPrefixOp(it->type);
            if (!prefixOp) {
                return nullptr;
            }
            std::shared_ptr<IExpression> left = (this->*prefixOp)(it);


            while (needParseLeft(it, precedence, rightToLeft)) {
                ParseBinaryFunc binaryOp = findBinaryOp(it->type);
                if (!binaryOp) {
                    return left;
                }
                left = (this->*binaryOp)(it, left);
            }
            return left;
        }
        std::shared_ptr<IExpression> parseUnaryExpr(AstItr& it)
        {
            const auto& op = (it++)->token;
            auto right = this->parseExpr(it, Precedence::Not, true);
            return std::make_shared<UnaryExpr>(op, right);
        }
        std::shared_ptr<IExpression> parseIdentifierValue(AstItr& it)
        {
            Token token = *(it++);
            return std::make_shared<IdentifierValue>(token);
        }
        std::shared_ptr<IExpression> parseValue(AstItr& it)
        {
            Token token = *(it++);
            return std::make_shared<Value>(token);
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
            // (
            auto expr = this->parseExpr(++it, Precedence::Lowest);

            // )
            if (invalid(it) || (it++)->type != TokenType::ParenR) {
                Print << U"ctfolder parse error: not found ')'";
            }
            return expr;
        }
        bool next(AstItr& it)
        {
            if (!invalid(it)) {
                ++it;
            }
            return it != m_tokens.end();
        }
        bool invalid(AstItr it) const
        {
            return it == m_tokens.end();
        }
    private:
        const Lexer& m_lexer;
        const Array<Token>& m_tokens;
        std::shared_ptr<AST::Root> m_root = nullptr;
    };
}