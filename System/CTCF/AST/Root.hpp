#pragma once
#include "IStatement.hpp"
#include <memory>

namespace ctcf::AST
{
	struct Root : INode
	{
		// 1つのみstatement
		std::shared_ptr<IStatement> statement;
	};
}