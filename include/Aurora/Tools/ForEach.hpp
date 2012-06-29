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

/// @file
/// @brief Macros to simplify container iteration

#ifndef AURORA_FOREACH_HPP
#define AURORA_FOREACH_HPP

/// @addtogroup Tools
/// @{

/// @brief Macro to iterate forward on a mutable sequence.
/// @details Code example:
/// @code
/// std::vector<int> v = createVector();
/// AURORA_ITR_FOREACH(itr, v)
/// {
///     *itr += 2;
/// }
/// @endcode
/// @hideinitializer
#define AURORA_ITR_FOREACH(iterator, container)		\
	for (auto iterator = (container).begin(),		\
	auroraForeachEnd = (container).end();			\
	iterator != auroraForeachEnd; ++iterator)

/// @brief Macro to iterate forward on a constant sequence.
/// @details Code example:
/// @code
/// const std::vector<int> v = createVector();
/// AURORA_CITR_FOREACH(itr, v)
/// {
///     std::cout << *itr << std::endl;
/// }
/// @endcode
/// @hideinitializer
#define AURORA_CITR_FOREACH(iterator, container)	\
	for (auto iterator = (container).cbegin(),		\
	auroraForeachEnd = (container).cend();			\
	iterator != auroraForeachEnd; ++iterator)

/// @brief Macro that emulates C++11 range-based for loop.
/// @details Code example:
/// @code
/// std::vector<int> v = createVector();
/// AURORA_FOREACH(int& i, v)
/// {
///     i += 2;
/// }
/// @endcode
/// @hideinitializer
#define AURORA_FOREACH(declaration, container)																\
	if (bool auroraBroken = false) {} else																	\
	for (auto auroraItr = container.begin(); auroraItr != container.end() && !auroraBroken; ++auroraItr)	\
	if (bool auroraPassed = false) {} else																	\
	if (auroraBroken = true, false) {} else																	\
	for (declaration = *auroraItr; !auroraPassed; auroraPassed = true, auroraBroken = false)


/// @}

#endif // AURORA_FOREACH_HPP
