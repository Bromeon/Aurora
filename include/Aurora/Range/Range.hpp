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
/// @brief Class template aurora::Range

#ifndef AURORA_RANGE_HPP
#define AURORA_RANGE_HPP

#include <Aurora/Range/Detail/RangeImpl.hpp>
#include <Aurora/SmartPtr/CopiedPtr.hpp>

#include <type_traits>


namespace aurora
{

/// @addtogroup Range
/// @{

/// @brief Type-erased range
/// @details Type-erased range class with forward iterator category. The iterator type is abstracted; only the element type
///  needs to be specified.
/// @tparam T Element type. Can be const-qualified to prevent changes of the elements.
/// @tparam C Iterator category. Must be one of the categories in namespace aurora::Traversal.
template <typename T, typename C>
class Range : public detail::RangeBase<T, C, C>
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Element type (template parameter T without const qualifier)
		/// 
		typedef typename std::remove_const<T>::type ElementType;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Construct from iterator interval [begin, end[
		///
		template <typename Itr>
		explicit Range(Itr begin, Itr end)
		: mRange(new detail::IteratorRange<T, C, C, Itr>(begin, end))
		{
			typedef typename std::iterator_traits<Itr>::iterator_category Category;
			static_assert( std::is_convertible<Category, C>::value, "Category of passed iterators too low." );
		}

		/// @brief Move constructor
		/// 
		Range(Range&& source)
		: mRange(std::move(source.mRange))
		{
		}

		/// @brief Construct from different range
		/// @details Can be used to add const qualifiers or to weaken the traversal category.
		template <typename T2, typename C2>
		Range(Range<T2, C2> origin)
		: mRange(std::move(origin.mRange))
		{
			static_assert( std::is_convertible<T2, T>::value, "Range element types incompatible." );
			static_assert( std::is_convertible<C2, C>::value, "Iterator categories incompatible." );
		}

		/// @brief Move assignment operator
		/// 
		Range& operator= (Range&& source)
		{
			mRange = std::move(source.mRange);
			return *this;
		}

		/// @brief Construct from different range (to add const)
		/// @details Can be used to add const qualifiers or to weaken the traversal category
		template <typename T2, typename C2>
		Range& operator= (Range<T2, C2> origin)
		{
			static_assert( std::is_convertible<T2, T>::value, "Range element types incompatible." );
			static_assert( std::is_convertible<C2, C>::value, "Iterator categories incompatible." );

			mRange = std::move(origin.mRange);
			return *this;
		}


// Documentation, since Doxygen has problems with specialized base class templates
#ifdef AURORA_DOXYGEN_SECTION

		/// @brief Returns the first element in the range.
		/// @details Requirements: Forward category.
		T& front() const;

		/// @brief Removes the first element in the range.
		/// @details Requirements: Forward category.
		void popFront();

		/// @brief Returns the last element in the range.
		/// @details Requirements: Bidirectional category.
		T& back() const;

		/// @brief Removes the last element in the range.
		/// @details Requirements: Bidirectional category.
		void popBack();

		/// @brief Checks if the range is empty.
		/// @details Requirements: Forward category.
		bool empty() const;

#endif // AURORA_DOXYGEN_SECTION


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		CopiedPtr<detail::AbstractRange<ElementType, C>>	mRange;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Friends
	template <typename T2, typename C2>
	friend class Range;

	friend struct detail::RangeAccess;
};

/// @}

} // namespace aurora

#endif // AURORA_RANGE_HPP
