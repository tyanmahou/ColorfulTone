#pragma once
#include <memory>
#include <core/Data/Genre/CTCF/AST/Expression/Value.hpp>

namespace ct::ctcf::AST
{
    struct IdentifierValue : Value
    {
        IdentifierValue(const Token& _token) :
            Value(_token),
            kind(FindIdentifierValueKind(_token.token))
        {}

		IdentifierValueKind kind;
    };
}