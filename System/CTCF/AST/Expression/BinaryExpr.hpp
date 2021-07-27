#pragma once
#include <memory>
#include "../IExpression.hpp"
#include <Siv3D/String.hpp>

namespace ctcf::AST
{
    struct BinaryExpr : IExpression
    {
        BinaryExpr(
            std::shared_ptr<IExpression> _left,
            const s3d::String& _op,
            std::shared_ptr<IExpression> _right
        ) :
            left(_left),
            op(_op),
            right(_right)
        {}
        std::shared_ptr<IExpression> left;
        s3d::String op;
        std::shared_ptr<IExpression> right;
    };
}