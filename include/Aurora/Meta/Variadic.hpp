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

} // namespace detail


/// @addtogroup Meta
/// @{


/// @}

} // namespace aurora

#endif // AURORA_HAS_VARIADIC_TEMPLATES
#endif // AURORA_VARIADIC_HPP
