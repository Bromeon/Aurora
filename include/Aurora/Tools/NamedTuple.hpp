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
#include <Aurora/Tools/Hash.hpp>

#include <tuple>


/// @addtogroup Tools
/// @{

// Note: Inconsistent syntax "T const&" instead of "const T&" because T is not a real type, only text replacement
// Thus, the former leads to problems in cases like pointers

// Constructor parameter
#define AURORA_DETAIL_PARAM_TV(Type, var)			Type const& var
#define AURORA_DETAIL_PARAM_P(pair)					AURORA_DETAIL_PARAM_TV pair
#define AURORA_DETAIL_PARAM(pair, index)			AURORA_PP_COMMA_IF(index) AURORA_DETAIL_PARAM_P(pair)

// Constructor initialization list: Value initialization / copy construction
#define AURORA_DETAIL_INIT_TV(Type, var)			var(var)
#define AURORA_DETAIL_INIT_P(pair)					AURORA_DETAIL_INIT_TV pair
#define AURORA_DETAIL_INIT(pair, index)				AURORA_PP_COMMA_IF(index) AURORA_DETAIL_INIT_P(pair)

// Constructor initialization list: Default construction
#define AURORA_DETAIL_INITDEFAULT_TV(Type, var)		var()
#define AURORA_DETAIL_INITDEFAULT_P(pair)			AURORA_DETAIL_INITDEFAULT_TV pair
#define AURORA_DETAIL_INITDEFAULT(pair, index)		AURORA_PP_COMMA_IF(index) AURORA_DETAIL_INITDEFAULT_P(pair)

// Member variable declaration
#define AURORA_DETAIL_MEMDECL_TV(Type, var)			Type var;
#define AURORA_DETAIL_MEMDECL_P(pair)				AURORA_DETAIL_MEMDECL_TV pair
#define AURORA_DETAIL_MEMDECL(pair, index)			AURORA_DETAIL_MEMDECL_P(pair)

// Variable passed as argument for std::make_tuple()
#define AURORA_DETAIL_VAR_TV(Type, var)				var
#define AURORA_DETAIL_VAR_P(pair)					AURORA_DETAIL_VAR_TV pair
#define AURORA_DETAIL_VAR(pair, index)				AURORA_PP_COMMA_IF(index) AURORA_DETAIL_VAR_P(pair)

// Type passed as template argument for std::tuple
#define AURORA_DETAIL_TYPE_TV(Type, var)			Type
#define AURORA_DETAIL_TYPE_P(pair)					AURORA_DETAIL_TYPE_TV pair
#define AURORA_DETAIL_TYPE(pair, index)				AURORA_PP_COMMA_IF(index) AURORA_DETAIL_TYPE_P(pair)

// Combine hash h with hash of member variable
#define AURORA_DETAIL_HASHCOMBINE_TV(Type, var)		aurora::hashCombine(h, t.var);
#define AURORA_DETAIL_HASHCOMBINE_P(pair)			AURORA_DETAIL_HASHCOMBINE_TV pair
#define AURORA_DETAIL_HASHCOMBINE(pair, index)		AURORA_DETAIL_HASHCOMBINE_P(pair)

// High-order macro to apply an extension
#define AURORA_DETAIL_EXTENSION(macro, index, args)	macro args

// Conversion function from named tuple to std::tuple
#define AURORA_DETAIL_TOSTDTUPLE(size, typeVarPairs)										\
std::tuple<AURORA_PP_FOREACH(AURORA_DETAIL_TYPE, size, typeVarPairs)> toStdTuple() const	\
{																							\
	return std::make_tuple(AURORA_PP_FOREACH(AURORA_DETAIL_VAR, size, typeVarPairs));		\
}																							\


/// @brief Comparison operator == for named tuples
/// @details Supplies the named tuple with an operator== in the surrounding namespace, which compares member-wise.
///  Do not invoke this macro directly. It is passed to AURORA_NAMED_TUPLE_EXT.
/// @hideinitializer
#define AURORA_NT_EQUAL(TupleName, size, typeVarPairs)										\
friend bool operator== (const TupleName& lhs, const TupleName& rhs)							\
{																							\
	return lhs.toStdTuple() == rhs.toStdTuple();											\
}																							\

/// @brief Comparison operator < for named tuples
/// @details Supplies the named tuple with an operator< in the surrounding namespace, which compares lexicographically.
///  Do not invoke this macro directly. It is passed to AURORA_NAMED_TUPLE_EXT.
/// @hideinitializer
#define AURORA_NT_LESS(TupleName, size, typeVarPairs)										\
friend bool operator< (const TupleName& lhs, const TupleName& rhs)							\
{																							\
	return lhs.toStdTuple() < rhs.toStdTuple();												\
}																							\

/// @brief Hash functor for named tuples
/// @details Supplies the named tuple with a member typedef @a Hasher inside the tuple class.
///  Do not invoke this macro directly. It is passed to AURORA_NAMED_TUPLE_EXT.
/// @hideinitializer
#define AURORA_NT_HASHER(TupleName, size, typeVarPairs)										\
struct Hasher																				\
{																							\
	std::size_t operator() (const TupleName& t)	const										\
	{																						\
		std::size_t h = 0;																	\
		AURORA_PP_FOREACH(AURORA_DETAIL_HASHCOMBINE, size, typeVarPairs)					\
		return h;																			\
	}																						\
};																							\

/// @brief Default constructor for named tuples
/// @details Supplies the named tuple with a default constructor calls each members' default constructor.
///  Do not invoke this macro directly. It is passed to AURORA_NAMED_TUPLE_EXT.
/// @hideinitializer
#define AURORA_NT_DEFAULT_CTOR(TupleName, size, typeVarPairs)												\
AURORA_PP_IF(size, /* there is already a 0-argument ctor if size==0 */										\
	TupleName()																								\
	: AURORA_PP_FOREACH(AURORA_DETAIL_INITDEFAULT, size, typeVarPairs)										\
	{																										\
	}																										\
, AURORA_PP_UNUSED)																							\

/// @brief Named tuple definition
/// @details Defines a struct type with a specified list of public members and a corresponding constructor.
/// @param TupleName Name of the struct
/// @param size Number of member variables
/// @param typeVarPairs Parenthesized sequence of (Type, variable) pairs, such as ((int, i), (double, d))
/// @hideinitializer
#define AURORA_NAMED_TUPLE(TupleName, size, typeVarPairs) AURORA_NAMED_TUPLE_EXT(TupleName, size, typeVarPairs, 0, ())

/// @brief Named tuple definition with extended functionality
/// @details Defines a struct type with a specified list of public members and a corresponding constructor.
/// @param TupleName Name of the struct
/// @param size Number of member variables
/// @param typeVarPairs Parenthesized sequence of (Type, variable) pairs, such as ((int, i), (double, d))
/// @param extSize Number of extensions
/// @param extensions Parenthesized sequence of extensions, such as (AURORA_NT_EQUAL, AURORA_NT_LESS)
/// @hideinitializer
#define AURORA_NAMED_TUPLE_EXT(TupleName, size, typeVarPairs, extSize, extensions)							\
struct TupleName																							\
{																											\
	/* Member variables	*/																					\
	AURORA_PP_FOREACH(AURORA_DETAIL_MEMDECL, size, typeVarPairs)											\
																											\
	/* Constructor */																						\
	explicit TupleName(AURORA_PP_FOREACH(AURORA_DETAIL_PARAM, size, typeVarPairs))							\
	AURORA_PP_IF(size, :, AURORA_PP_UNUSED) AURORA_PP_FOREACH(AURORA_DETAIL_INIT, size, typeVarPairs)		\
	{																										\
	}																										\
																											\
	/* toStdTuple() conversion function */																	\
	AURORA_DETAIL_TOSTDTUPLE(size, typeVarPairs)															\
																											\
	/* All extensions */																					\
	AURORA_PP_FOREACH_DATA(AURORA_DETAIL_EXTENSION, extSize, extensions, (TupleName, size, typeVarPairs))	\
};																											\

/// @}
	
#endif // AURORA_NAMEDTUPLE_HPP
