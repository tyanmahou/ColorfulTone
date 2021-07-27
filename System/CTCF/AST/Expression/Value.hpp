#pragma once
#include <memory>
#include "../IExpression.hpp"
#include "../../Lexer/Token.hpp"

namespace ctcf::AST
{
    struct Value : IExpression
    {
        Value(const Token& _token) :
            token(_token)
        {}
        Token token;
    };
}