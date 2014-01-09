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


// Apply macro repeated times, counting from 0 to n
#define AURORA_PP_ENUMERATE_0(macro)
#define AURORA_PP_ENUMERATE_1(macro)			macro(1)
#define AURORA_PP_ENUMERATE_2(macro)			AURORA_PP_ENUMERATE_1(macro) macro(2)
#define AURORA_PP_ENUMERATE_3(macro)			AURORA_PP_ENUMERATE_2(macro) macro(3)
#define AURORA_PP_ENUMERATE_4(macro)			AURORA_PP_ENUMERATE_3(macro) macro(4)
#define AURORA_PP_ENUMERATE_5(macro)			AURORA_PP_ENUMERATE_4(macro) macro(5)
#define AURORA_PP_ENUMERATE(n, macro)			AURORA_PP_ENUMERATE_ ## n(macro)


// Apply macro repeated times, counting from 0 to n and using a comma separator
#define AURORA_PP_ENUMERATE_COMMA_0(macro)
#define AURORA_PP_ENUMERATE_COMMA_1(macro)		macro(1)
#define AURORA_PP_ENUMERATE_COMMA_2(macro)		AURORA_PP_ENUMERATE_COMMA_1(macro), macro(2)
#define AURORA_PP_ENUMERATE_COMMA_3(macro)		AURORA_PP_ENUMERATE_COMMA_2(macro), macro(3)
#define AURORA_PP_ENUMERATE_COMMA_4(macro)		AURORA_PP_ENUMERATE_COMMA_3(macro), macro(4)
#define AURORA_PP_ENUMERATE_COMMA_5(macro)		AURORA_PP_ENUMERATE_COMMA_4(macro), macro(5)
#define AURORA_PP_ENUMERATE_COMMA(n, macro)		AURORA_PP_ENUMERATE_COMMA_ ## n(macro)


// Put a comma, if the argument is not zero
#define AURORA_PP_COMMA_UNLESS_ZERO_0()
#define AURORA_PP_COMMA_UNLESS_ZERO_1()	,
#define AURORA_PP_COMMA_UNLESS_ZERO_2()	,
#define AURORA_PP_COMMA_UNLESS_ZERO_3()	,
#define AURORA_PP_COMMA_UNLESS_ZERO_4()	,
#define AURORA_PP_COMMA_UNLESS_ZERO_5()	,
#define AURORA_PP_COMMA_UNLESS_ZERO(n)	AURORA_PP_COMMA_UNLESS_ZERO_ ## n()

#endif // AURORA_PREPROCESSOR_HPP
