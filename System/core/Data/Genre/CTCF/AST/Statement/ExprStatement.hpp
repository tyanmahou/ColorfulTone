#pragma once
#include <memory>
#include <core/Data/Genre/CTCF/AST/IStatement.hpp>
#include <core/Data/Genre/CTCF/AST/IExpression.hpp>

namespace ct::ctcf::AST
{
	struct ExprStatement : IStatement
	{
		s3d::String type;
		s3d::String mode;
		std::shared_ptr<IExpression> expression;
	};
}