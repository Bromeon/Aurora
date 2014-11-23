/////////////////////////////////////////////////////////////////////////////////
//
// Aurora C++ Library
// Copyright (c) 2012-2014 Jan Haller
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

/// @file
/// @brief Metaprogramming helpers for variadic templates

#ifndef AURORA_VARIADIC_HPP
#define AURORA_VARIADIC_HPP

#include <Aurora/Config.hpp>

#ifdef AURORA_HAS_VARIADIC_TEMPLATES

namespace aurora
{
namespace detail
{

	// Reverse of NthType: get index of type T in typelist Types
	template <typename T, typename... Types>
	struct IndexOfType;

	template <typename T, typename Head, typename... Tail>
	struct IndexOfType<T, Head, Tail...>
	{
		static const std::size_t value = 1 + IndexOfType<T, Tail...>::value;
	};

	template <typename T, typename... Tail>
	struct IndexOfType<T, T, Tail...>
	{
		static const std::size_t value = 0;
	};


	// Metafunction to apply a class template 'Function' to each type of variadic pack typelist
	template <template <typename> class Function, typename... Ts>
	struct ForEachType;

	template <template <typename> class Function, typename T, typename... Ts>
	struct ForEachType<Function, T, Ts...>
	{
		static void apply()
		{
			Function<T>::apply();
			ForEachType<Function, Ts...>::apply();
		}
	};

	template <template <typename> class Function>
	struct ForEachType<Function>
	{
		static void apply()
		{
		}
	};


	// Metafunction to apply a class template 'Function' to each value of variadic pack
	template <template <typename> class Function, typename... Ts>
	struct ForEachValue;

	template <template <typename> class Function, typename T, typename... Ts>
	struct ForEachValue<Function, T, Ts...>
	{
		static void apply(T&& t, Ts&&... ts)
		{
			Function<T>::apply(std::forward<T>(t));
			ForEachValue<Function, Ts...>::apply(std::forward<Ts>(ts)...);
		}
	};

	template <template <typename> class Function>
	struct ForEachValue<Function>
	{
		static void apply()
		{
		}
	};

} // namespace detail


/// @addtogroup Meta
/// @{


/// @brief Apply function for each type in variadic parameter pack
/// @tparam Function Class template with a static member function <b>void apply()</b>.
/// @tparam Ts Typelist to iterate through.
/// @details For each type @a T in @a Ts, the expression <b>Function<T>::apply()</b> is evaluated. Example:
/// @code
/// template <typename T>
/// struct Example
/// {
///     static void apply()
///     {
///         std::cout << typeid(T).name() << ' ';
///     }
/// };
///
/// foreach<Example, int, float>(); // output: "int float"
/// @endcode
template <template <typename T> class Function, typename... Ts>
void foreach()
{
	detail::ForEachType<Function, Ts...>::apply();
}

/// @brief Apply function for each value in variadic parameter pack
/// @tparam Function Class template with a static member function <b>void apply(T& value)</b>. The parameter needs to be compatible
///   to type @a T, however pass by value or by (const) (lvalue|rvalue) reference is allowed.
/// @tparam Ts Argument types -- can be deduced from the function arguments.
/// @param args Arguments to iterate through.
/// @details For each argument @a arg in @a args, the expression <b>Function<T>::apply(arg)</b> is evaluated. Example:
/// @code
/// template <typename T>
/// struct Example
/// {
///     static void apply(const T& arg)
///     {
///         std::cout << typeid(T).name() << '=' << arg << ' ';
///     }
/// };
///
/// foreach<Example>(4, 3.25f); // output: "int=4 float=3.25"
/// @endcode
template <template <typename T> class Function, typename... Ts>
void foreach(Ts&&... args)
{
	detail::ForEachValue<Function, Ts...>::apply(std::forward<Ts>(args)...);
}

/// @}

} // namespace aurora

#endif // AURORA_HAS_VARIADIC_TEMPLATES
#endif // AURORA_VARIADIC_HPP
