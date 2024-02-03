#pragma once
#include <memory>
#include <core/Data/Genre/CTCF/AST/IExpression.hpp>
#include <core/Data/Genre/CTCF/Lexer/Token.hpp>

namespace ct::ctcf::AST
{
    struct Value : IExpression
    {
        Value(const Token& _token) :
            token(_token)
        {}
        Token token;
    };
}