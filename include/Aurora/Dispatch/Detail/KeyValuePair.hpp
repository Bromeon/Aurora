/////////////////////////////////////////////////////////////////////////////////
//
// Aurora C++ Library
// Copyright (c) 2012-2015 Jan Haller
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

#ifndef AURORA_KEYVALUEPAIR_HPP
#define AURORA_KEYVALUEPAIR_HPP


namespace aurora
{
namespace detail
{

	// Structure similar to std::pair, but only comparing the first element
	template <typename Key, typename Value>
	struct KeyValuePair
	{
		Key		key;
		Value	value;

		KeyValuePair(const Key& key, const Value& value)
		: key(key)
		, value(value)
		{
		}
	};

	// Less-than operator for sorting
	template <typename Key, typename Value>
	bool operator< (const KeyValuePair<Key, Value>& lhs, const KeyValuePair<Key, Value>& rhs)
	{
		return lhs.key < rhs.key;
	}
	
} // namespace detail
} // namespace aurora

#endif // AURORA_KEYVALUEPAIR_HPP
