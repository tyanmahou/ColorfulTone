#pragma once
#include <memory>
#include <core/Data/Genre/CTCF/AST/IExpression.hpp>
#include <Siv3D/String.hpp>

namespace ct::ctcf::AST
{
    struct UnaryExpr : IExpression
    {
        UnaryExpr(
            const s3d::String& _op,
            std::shared_ptr<IExpression> _right
        ) :
            op(_op),
            right(_right)
        {}
        s3d::String op;
        std::shared_ptr<IExpression> right;
    };
}