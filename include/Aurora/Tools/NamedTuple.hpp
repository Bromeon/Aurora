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
/// @brief Tuples with named members

#ifndef AURORA_NAMEDTUPLE_HPP
#define AURORA_NAMEDTUPLE_HPP

#include <Aurora/Tools/Preprocessor.hpp>

#include <tuple>


/// @addtogroup Tools
/// @{

// Note: Inconsistent syntax "T const&" instead of "const T&" because T is not a real type, only text replacement
// Thus, the former leads to problems in cases like pointers

// Constructor parameter
#define AURORA_DETAIL_PARAM_TV(Type, var)		Type const& var
#define AURORA_DETAIL_PARAM_P(pair)				AURORA_DETAIL_PARAM_TV pair
#define AURORA_DETAIL_PARAM(pair, index)		AURORA_PP_COMMA_IF(index) AURORA_DETAIL_PARAM_P(pair) 

// Constructor initialization list
#define AURORA_DETAIL_INIT_TV(Type, var)		var(var)
#define AURORA_DETAIL_INIT_P(pair)				AURORA_DETAIL_INIT_TV pair
#define AURORA_DETAIL_INIT(pair, index)			AURORA_PP_COMMA_IF(index) AURORA_DETAIL_INIT_P(pair)

// Member variable declaration
#define AURORA_DETAIL_MEMDECL_TV(Type, var)		Type var;
#define AURORA_DETAIL_MEMDECL_P(pair)			AURORA_DETAIL_MEMDECL_TV pair
#define AURORA_DETAIL_MEMDECL(pair, index)		AURORA_DETAIL_MEMDECL_P(pair)

// Variable passed as argument for std::make_tuple()
#define AURORA_DETAIL_VAR_TV(Type, var)			var
#define AURORA_DETAIL_VAR_P(pair)				AURORA_DETAIL_VAR_TV pair
#define AURORA_DETAIL_VAR(pair, index)			AURORA_PP_COMMA_IF(index) AURORA_DETAIL_VAR_P(pair) 

// Type passed as template argument for std::tuple
#define AURORA_DETAIL_TYPE_TV(Type, var)		Type
#define AURORA_DETAIL_TYPE_P(pair)				AURORA_DETAIL_TYPE_TV pair
#define AURORA_DETAIL_TYPE(pair, index)			AURORA_PP_COMMA_IF(index) AURORA_DETAIL_TYPE_P(pair) 


#define AURORA_DETAIL_TOSTDTUPLE(size, typeVarPairs)										\
std::tuple<AURORA_PP_FOREACH(AURORA_DETAIL_TYPE, size, typeVarPairs)> toStdTuple() const	\
{																							\
	return std::make_tuple(AURORA_PP_FOREACH(AURORA_DETAIL_VAR, size, typeVarPairs));		\
}																							\

/// @brief Named tuple definition
/// @details Defines a struct type with a specified list of public members and a corresponding constructor.
/// @param TupleName Name of the struct
/// @param size Number of member variables
/// @param typeVarPairs Parenthesized sequence of (Type, variable) pairs, such as ((int, i), (double, d))
/// @hideinitializer
#define AURORA_NAMED_TUPLE(TupleName, size, typeVarPairs)								\
struct TupleName																		\
{																						\
	explicit TupleName(AURORA_PP_FOREACH(AURORA_DETAIL_PARAM, size, typeVarPairs))		\
	: AURORA_PP_FOREACH(AURORA_DETAIL_INIT, size, typeVarPairs)							\
	{																					\
	}																					\
																						\
	AURORA_PP_FOREACH(AURORA_DETAIL_MEMDECL, size, typeVarPairs)						\
	AURORA_DETAIL_TOSTDTUPLE(size, typeVarPairs)										\
};																						\

/// @}
	
#endif // AURORA_NAMEDTUPLE_HPP
