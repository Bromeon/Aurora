/////////////////////////////////////////////////////////////////////////////////
//
// Aurora C++ Library
// Copyright (c) 2012 Jan Haller
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

#ifndef AURORA_METAPROGRAMMING_HPP
#define AURORA_METAPROGRAMMING_HPP

#include <Aurora/Config.hpp>

#include <type_traits>


namespace aur
{
namespace detail
{

	// Selects a type depending on a boolean condition.
	// If Condition is true, the result member Type evaluates to TrueType, otherwise to FalseType.
	template <bool Condition, typename TrueType, typename FalseType>
	struct Conditional
	{
		typedef TrueType Type;
	};

	template <typename TrueType, typename FalseType>
	struct Conditional<false, TrueType, FalseType>
	{
		typedef FalseType Type;
	};


	// Removes reference and pointer attribute from type
	template <typename T>
	struct RemoveIndirection
	{
		typedef typename std::remove_pointer<
			typename std::remove_reference<T>::type
		>::type Type;
	};


	// Removes reference, pointer and const attribute from type
	template <typename T>
	struct Decay
	{
		typedef typename std::remove_const<
			typename RemoveIndirection<T>::Type
		>::type Type;
	};


	// Type wrapper for overloaded functions
	template <typename T>
	struct Type2Type
	{
	};

	// Integer wrapper for overloaded functions
	template <int N>
	struct Int2Type
	{
	};


	// SFINAE Enable If
	template <bool Condition>
	struct EnableIf;

	template <>
	struct EnableIf<true>
	{
		typedef void Type;
	};

	#define AURORA_ENABLE_IF(...)  typename ::aur::detail::EnableIf<__VA_ARGS__>::Type* = nullptr
	#define AURORA_DISABLE_IF(...) AURORA_ENABLE_IF(!(__VA_ARGS__))


	// Adjusts New such that it has the same const, pointer, reference attributes as Origin
	template <typename Origin, typename New>
	struct Replicate
	{
		typedef typename Decay<New>::Type														New0;

		typedef typename Conditional<
			std::is_const<typename RemoveIndirection<Origin>::Type>::value,
			const New0,
			New0>::Type																			New1;

		typedef typename Conditional<std::is_pointer<Origin>::value, New1*, New1>::Type	New2;
		typedef typename Conditional<std::is_reference<Origin>::value, New2&, New2>::Type	New3;
		typedef typename Conditional<std::is_const<Origin>::value, const New3, New3>::Type	Type;
	};

	// Human-readable form
	#define AURORA_REPLICATE(Origin, New) typename aur::detail::Replicate<Origin, New>::Type
	
} // namespace detail
} // namespace aur

#endif // AURORA_METAPROGRAMMING_HPP
