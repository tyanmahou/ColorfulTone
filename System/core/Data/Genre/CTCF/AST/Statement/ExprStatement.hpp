#pragma once
#include <memory>
#include <core/Data/Genre/CTCF/AST/IStatement.hpp>
#include <core/Data/Genre/CTCF/AST/IExpression.hpp>

namespace ct::ctcf::AST
{
	struct ExprStatement : IStatement
	{
		std::shared_ptr<IExpression> expression;
	};
}