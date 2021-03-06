#pragma once

#include<type_traits>

//メタ関数
namespace Mahou
{
	//std::is_invocable(C++17なら使える)
	template<class AlwaysVoid, class F, class... Args>
	struct is_invocable_impl :std::false_type
	{};
	template<class F, class... Args>
	struct is_invocable_impl<
		std::void_t<decltype(std::declval<F>()(std::declval<Args>()...))>, F, Args...
	> :std::true_type
	{};

	template<class F, class... Args>
	using is_invocable = is_invocable_impl<void, F, Args...>;

	template<class F, class... Args>
	constexpr bool is_invocable_v = is_invocable_impl<void, F, Args...>::value;

}// Mahou