#pragma once
#include <core/Data/Genre/CTCF/AST/IStatement.hpp>
#include <memory>

namespace ct::ctcf::AST
{
	struct Root : INode
	{
		// 1つのみstatement
		std::shared_ptr<IStatement> statement;
	};
}