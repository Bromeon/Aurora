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
/// @brief Utilities for preprocessor metaprogramming

// Minimalistic implementation inspired by Boost.Preprocessor
// Tricks used:
// - Map numbers/values to other values: Concatenate macro with value; result is a predefined macro which is then substituted
// - Lazy evaluation: Forward macros to others that effectively do the same (often 2 layers of indirection)
// - Unpack tuples: Call macro without parenthesis around argument
// - Count VA arguments: Forward them to another VA macro with enumerated argument list
// - Detect empty arguments: Exploit property of function-style macros that are not substituted without an argument list
// - Get rid of garbage passed as part of argument: Forward to VA macro that consumes all arguments
//
// Notes: 
// - Preprocessor metaprogramming is a whole different approach than anything else from C++; nothing is intuitive
// - AURORA_PP_AT() can be implemented without O(n^2) helper macros, e.g. in <boost/preprocessor/array/elem.hpp>
// - Passing __VA_ARGS__ to non-variadic macros does not work reliably
//   (see http://www.boost.org/doc/libs/1_55_0/libs/preprocessor/doc/topics/variadic_macros.html).
// - Some macros behave differently when deeply nested; it may be difficult to combine them
// - Many macros behave differently on VC++ and g++, just in case it gets boring


#ifndef AURORA_PREPROCESSOR_HPP
#define AURORA_PREPROCESSOR_HPP

#include <Aurora/Config.hpp>

// Concatenation
#define AURORA_PP_CAT_IMPL(a, b)		a ## b
#define AURORA_PP_CAT(a, b)				AURORA_PP_CAT_IMPL(a, b)
#define AURORA_PP_CAT3(a, b, c)			AURORA_PP_CAT(AURORA_PP_CAT(a, b), c)
#define AURORA_PP_CAT4(a, b, c, d)		AURORA_PP_CAT3(AURORA_PP_CAT(a, b), c, d)
#define AURORA_PP_CAT5(a, b, c, d, e)	AURORA_PP_CAT4(AURORA_PP_CAT(a, b), c, d, e)


// Convert to string literal
#define AURORA_PP_STRINGIZE_IMPL(a)		#a
#define AURORA_PP_STRINGIZE(a)			AURORA_PP_STRINGIZE_IMPL(a)


// Object and function macro that evaluate to empty space
#define AURORA_PP_NOTHING
#define AURORA_PP_VA_CONSUME(...)


// Macro that evaluates to its argument without changing it
#define AURORA_PP_ID_IMPL(expr) expr
#define AURORA_PP_ID(expr) AURORA_PP_ID_IMPL(expr)


// Conditional evaluation - when using function style macros for true/falseCase, put the argument list after PP_IF(...), i.e. PP_IF(...)(args)
#define AURORA_PP_IF_0(trueCase, falseCase)					falseCase
#define AURORA_PP_IF_1(trueCase, falseCase)					trueCase
#define AURORA_PP_IF_2(trueCase, falseCase)					trueCase
#define AURORA_PP_IF_3(trueCase, falseCase)					trueCase
#define AURORA_PP_IF_4(trueCase, falseCase)					trueCase
#define AURORA_PP_IF_5(trueCase, falseCase)					trueCase
#define AURORA_PP_IF_IMPL2(condition, trueCase, falseCase)	AURORA_PP_IF_ ## condition(trueCase, falseCase)
#define AURORA_PP_IF_IMPL(condition, trueCase, falseCase)	AURORA_PP_IF_IMPL2(condition, trueCase, falseCase)
#define AURORA_PP_IF(condition, trueCase, falseCase)		AURORA_PP_IF_IMPL(condition, trueCase, falseCase)


// Apply macro repeated times, counting from 0 to n
#define AURORA_PP_ENUMERATE_0(macro)
#define AURORA_PP_ENUMERATE_1(macro)			                             macro(1)
#define AURORA_PP_ENUMERATE_2(macro)			AURORA_PP_ENUMERATE_1(macro) macro(2)
#define AURORA_PP_ENUMERATE_3(macro)			AURORA_PP_ENUMERATE_2(macro) macro(3)
#define AURORA_PP_ENUMERATE_4(macro)			AURORA_PP_ENUMERATE_3(macro) macro(4)
#define AURORA_PP_ENUMERATE_5(macro)			AURORA_PP_ENUMERATE_4(macro) macro(5)
#define AURORA_PP_ENUMERATE(n, macro)			AURORA_PP_ENUMERATE_ ## n(macro)


// Apply macro repeated times, counting from 0 to n and using a comma separator
#define AURORA_PP_ENUMERATE_COMMA_0(macro)
#define AURORA_PP_ENUMERATE_COMMA_1(macro)		                                    macro(1)
#define AURORA_PP_ENUMERATE_COMMA_2(macro)		AURORA_PP_ENUMERATE_COMMA_1(macro), macro(2)
#define AURORA_PP_ENUMERATE_COMMA_3(macro)		AURORA_PP_ENUMERATE_COMMA_2(macro), macro(3)
#define AURORA_PP_ENUMERATE_COMMA_4(macro)		AURORA_PP_ENUMERATE_COMMA_3(macro), macro(4)
#define AURORA_PP_ENUMERATE_COMMA_5(macro)		AURORA_PP_ENUMERATE_COMMA_4(macro), macro(5)
#define AURORA_PP_ENUMERATE_COMMA(n, macro)		AURORA_PP_ENUMERATE_COMMA_ ## n(macro)


// Put a comma, if the argument is not zero
#define AURORA_PP_COMMA_IF_0()
#define AURORA_PP_COMMA_IF_1()	,
#define AURORA_PP_COMMA_IF_2()	,
#define AURORA_PP_COMMA_IF_3()	,
#define AURORA_PP_COMMA_IF_4()	,
#define AURORA_PP_COMMA_IF_5()	,
#define AURORA_PP_COMMA_IF(n)	AURORA_PP_COMMA_IF_ ## n()


// Index access to tuple
#define AURORA_PP_AT_S1_N0(v0)					v0
#define AURORA_PP_AT_S2_N0(v0, v1)				v0
#define AURORA_PP_AT_S2_N1(v0, v1)				v1
#define AURORA_PP_AT_S3_N0(v0, v1, v2)			v0
#define AURORA_PP_AT_S3_N1(v0, v1, v2)			v1
#define AURORA_PP_AT_S3_N2(v0, v1, v2)			v2
#define AURORA_PP_AT_S4_N0(v0, v1, v2, v3)		v0
#define AURORA_PP_AT_S4_N1(v0, v1, v2, v3)		v1
#define AURORA_PP_AT_S4_N2(v0, v1, v2, v3)		v2
#define AURORA_PP_AT_S4_N3(v0, v1, v2, v3)		v3
#define AURORA_PP_AT_S5_N0(v0, v1, v2, v3, v4)	v0
#define AURORA_PP_AT_S5_N1(v0, v1, v2, v3, v4)	v1
#define AURORA_PP_AT_S5_N2(v0, v1, v2, v3, v4)	v2
#define AURORA_PP_AT_S5_N3(v0, v1, v2, v3, v4)	v3
#define AURORA_PP_AT_S5_N4(v0, v1, v2, v3, v4)	v4

#define AURORA_PP_AT(size, n, tuple) AURORA_PP_CAT4(AURORA_PP_AT_S, size, _N, n) tuple


// Iterate over tuple, apply each macro once. macro is MACRO(value, index)
#define AURORA_PP_FOREACH_0(macro, size, tuple)
#define AURORA_PP_FOREACH_1(macro, size, tuple)		                                        macro(AURORA_PP_AT(size, 0, tuple), 0)
#define AURORA_PP_FOREACH_2(macro, size, tuple)		AURORA_PP_FOREACH_1(macro, size, tuple) macro(AURORA_PP_AT(size, 1, tuple), 1)
#define AURORA_PP_FOREACH_3(macro, size, tuple)		AURORA_PP_FOREACH_2(macro, size, tuple) macro(AURORA_PP_AT(size, 2, tuple), 2)
#define AURORA_PP_FOREACH_4(macro, size, tuple)		AURORA_PP_FOREACH_3(macro, size, tuple) macro(AURORA_PP_AT(size, 3, tuple), 3)
#define AURORA_PP_FOREACH_5(macro, size, tuple)		AURORA_PP_FOREACH_4(macro, size, tuple) macro(AURORA_PP_AT(size, 4, tuple), 4)

#define AURORA_PP_FOREACH_SIZED(macro, size, tuple)	AURORA_PP_CAT(AURORA_PP_FOREACH_, size) (macro, size, tuple)
#define AURORA_PP_FOREACH(macro, tuple)				AURORA_PP_FOREACH_SIZED(macro, AURORA_PP_SIZE(tuple), tuple)


// Iterate over tuple, passing additional data to each macro. macro is MACRO(value, index, data)
#define AURORA_PP_FOREACH_DATA_0(macro, size, tuple, data)
#define AURORA_PP_FOREACH_DATA_1(macro, size, tuple, data)		                                                   macro(AURORA_PP_AT(size, 0, tuple), 0, data)
#define AURORA_PP_FOREACH_DATA_2(macro, size, tuple, data)		AURORA_PP_FOREACH_DATA_1(macro, size, tuple, data) macro(AURORA_PP_AT(size, 1, tuple), 1, data)
#define AURORA_PP_FOREACH_DATA_3(macro, size, tuple, data)		AURORA_PP_FOREACH_DATA_2(macro, size, tuple, data) macro(AURORA_PP_AT(size, 2, tuple), 2, data)
#define AURORA_PP_FOREACH_DATA_4(macro, size, tuple, data)		AURORA_PP_FOREACH_DATA_3(macro, size, tuple, data) macro(AURORA_PP_AT(size, 3, tuple), 3, data)
#define AURORA_PP_FOREACH_DATA_5(macro, size, tuple, data)		AURORA_PP_FOREACH_DATA_4(macro, size, tuple, data) macro(AURORA_PP_AT(size, 4, tuple), 4, data)

#define AURORA_PP_FOREACH_DATA_SIZED(macro, size, tuple, data)	AURORA_PP_CAT(AURORA_PP_FOREACH_DATA_, size) (macro, size, tuple, data)
#define AURORA_PP_FOREACH_DATA(macro, tuple, data)				AURORA_PP_FOREACH_DATA_SIZED(macro, AURORA_PP_SIZE(tuple), tuple, data)


// Check whether a variadic argument list starts with (
// Warning: From now on, you are entering the deep abyss of the preprocessor. The following macros depend on black magic; their functioning is probabilistic.
#define AURORA_PP_AURORA_PP_NO				0 AURORA_PP_VA_CONSUME(
#define AURORA_PP_AURORA_PP_YES				1 AURORA_PP_VA_CONSUME(
#define AURORA_PP_VA_CAT2(a, b, ...)		AURORA_PP_CAT(a, b)
#define AURORA_PP_VA_YESNO_TO_10(a, ...)	AURORA_PP_VA_CAT2(AURORA_PP_, a) )
#define AURORA_PP_NO(...)					AURORA_PP_YES

#define AURORA_PP_VA_HAS_PARENTHESIS(...)	AURORA_PP_ID(AURORA_PP_VA_YESNO_TO_10(AURORA_PP_NO __VA_ARGS__))


// VA size inference (argument lists with at least one argument)
#define AURORA_PP_VA_POSITIVE_SIZE_IMPL(e0, e1, e2, e3, e4, e5, size, ...) size
#define AURORA_PP_VA_POSITIVE_SIZE(...) AURORA_PP_CAT(AURORA_PP_VA_POSITIVE_SIZE_IMPL(__VA_ARGS__, 6, 5, 4, 3, 2, 1,), AURORA_PP_NOTHING)


// VA size inference, including empty argument lists
#define AURORA_PP_VA_SIZE_1 1
#define AURORA_PP_VA_SIZE_2 2
#define AURORA_PP_VA_SIZE_3 3
#define AURORA_PP_VA_SIZE_4 4
#define AURORA_PP_VA_SIZE_5 5
#define AURORA_PP_VA_SIZE_6 0

#define AURORA_PP_VA_5COMMAS(...)		,,,,,
#define AURORA_PP_VA_SIZE_IMPL(...)		AURORA_PP_CAT(AURORA_PP_VA_SIZE_, AURORA_PP_VA_POSITIVE_SIZE(__VA_ARGS__))
#define AURORA_PP_VA_SIZE(...)			AURORA_PP_VA_SIZE_IMPL(AURORA_PP_VA_5COMMAS __VA_ARGS__ ())


// Tuple size
#define AURORA_PP_POSITIVE_SIZE(tuple)	AURORA_PP_VA_POSITIVE_SIZE tuple

#define AURORA_PP_SIZE_IMPL1(tuple)		AURORA_PP_IF(AURORA_PP_VA_HAS_PARENTHESIS tuple, AURORA_PP_VA_POSITIVE_SIZE, AURORA_PP_VA_SIZE) tuple
#define AURORA_PP_SIZE_IMPL(tuple)		AURORA_PP_SIZE_IMPL1(tuple)

#ifdef _MSC_VER
	#define AURORA_PP_SIZE(tuple)		AURORA_PP_CAT(AURORA_PP_SIZE_IMPL(tuple),)	// works with nested tuples, but not deep inside other macros (especially IF)
	#define AURORA_PP_FLAT_SIZE(tuple)	AURORA_PP_CAT(AURORA_PP_VA_SIZE tuple,)		// doesn't work with nested tuples
#else
	#define AURORA_PP_SIZE(tuple)		AURORA_PP_SIZE_IMPL(tuple)
	#define AURORA_PP_FLAT_SIZE(tuple)	AURORA_PP_CAT(AURORA_PP_VA_SIZE tuple,)
#endif


#endif // AURORA_PREPROCESSOR_HPP
