#pragma once
#include <memory>
#include "Value.hpp"

namespace ctcf::AST
{
    struct IdentifierValue : Value
    {
        IdentifierValue(const Token& _token) :
            Value(_token)
        {}
    };
}