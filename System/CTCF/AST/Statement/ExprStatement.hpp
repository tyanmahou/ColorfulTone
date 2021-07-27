#pragma once
#include <memory>
#include "../IExpression.hpp"
#include "../IStatement.hpp"
namespace ctcf::AST
{
	struct ExprStatement : IStatement
	{
		std::shared_ptr<IExpression> expression;
	};
}