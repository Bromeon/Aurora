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


namespace aurora
{

/// @addtogroup Tools
/// @{

// Note: Inconsistent syntax "T const&" instead of "const T&" because T is not a real type, only text replacement
// Thus, the former leads to problems in cases like pointers

/// @brief Named tuple with 2 members
/// @details Defines a struct called @a Tuple, with two members as specified in the macro and a constructor
///  taking an argument for each member.
/// @hideinitializer
#define AURORA_NAMED_TUPLE2(Tuple, Type1, member1, Type2, member2)	\
struct Tuple														\
{																	\
	explicit Tuple(Type1 const& arg1, Type2 const& arg2)			\
	: member1(arg1)													\
	, member2(arg2)													\
	{																\
	}																\
																	\
	Type1 member1;													\
	Type2 member2;													\
};
	
/// @brief Named tuple with 3 members
/// @details Defines a struct called @a Tuple, with three members as specified in the macro and a constructor
///  taking an argument for each member.
/// @hideinitializer
#define AURORA_NAMED_TUPLE3(Tuple, Type1, member1, Type2, member2, Type3, member3)	\
struct Tuple																		\
{																					\
	explicit Tuple(Type1 const& arg1, Type2 const& arg2, Type3 const& arg3)			\
	: member1(arg1)																	\
	, member2(arg2)																	\
	, member3(arg3)																	\
	{																				\
	}																				\
																					\
	Type1 member1;																	\
	Type2 member2;																	\
	Type3 member3;																	\
};

/// @brief Named tuple with 4 members
/// @details Defines a struct called @a Tuple, with four members as specified in the macro and a constructor
///  taking an argument for each member.
/// @hideinitializer
#define AURORA_NAMED_TUPLE4(Tuple, Type1, member1, Type2, member2, Type3, member3, Type4, member4)	\
struct Tuple																						\
{																									\
	explicit Tuple(Type1 const& arg1, Type2 const& arg2, Type3 const& arg3, Type4 const& arg4)		\
	: member1(arg1)																					\
	, member2(arg2)																					\
	, member3(arg3)																					\
	, member4(arg4)																					\
	{																								\
	}																								\
																									\
	Type1 member1;																					\
	Type2 member2;																					\
	Type3 member3;																					\
	Type4 member4;																					\
};

/// @}

} // namespace aurora

#endif // AURORA_NAMEDTUPLE_HPP
