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

#ifndef AURORA_ABSTRACTRANGE_HPP
#define AURORA_ABSTRACTRANGE_HPP

#include <Aurora/Range/Detail/AbstractIterator.hpp>
#include <Aurora/SmartPtr/CopiedPtr.hpp>
#include <Aurora/Meta/Templates.hpp>
#include <Aurora/Meta/Tuple.hpp>

#include <utility>


namespace aurora
{
namespace detail
{

	// Type-erased base class for different range implementations
	template <typename T>
	struct AbstractRange
	{
		typedef CopiedPtr<AbstractRange> Ptr;
		typedef typename AbstractIterator<T>::Ptr AbstractIteratorPtr;

		virtual ~AbstractRange() {}

		virtual AbstractIteratorPtr begin() = 0;
		virtual AbstractIteratorPtr beforeEnd() = 0;

		virtual AbstractIteratorPtr next(AbstractIteratorPtr sink) = 0;
		virtual AbstractIteratorPtr prev(AbstractIteratorPtr sink) = 0;
	};
	
	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	// Default range implementation, consisting of an iterator pair
	template <typename T, typename Itr>
	struct IteratorRange : AbstractRange<T>
	{
		using typename AbstractRange<T>::AbstractIteratorPtr;

		IteratorRange(Itr begin, Itr end)
		: mBegin(begin)
		, mEnd(end)
		{
		}

		virtual AbstractIteratorPtr begin()
		{
			return makeSubIterator<T>(mBegin, mEnd, true);
		}

		virtual AbstractIteratorPtr beforeEnd()
		{
			return makeSubIterator<T>(mBegin, mEnd, false);
		}

		virtual AbstractIteratorPtr next(AbstractIteratorPtr sink)
		{
			assert(sink);
			return nullptr;
		}

		virtual AbstractIteratorPtr prev(AbstractIteratorPtr sink)
		{
			assert(sink);
			return nullptr;
		}

		Itr mBegin;
		Itr mEnd;
	};
	
	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	// Range consisting of multiple ranges, aggregated as std::tuple for efficiency reasons
	// TupleRange is created during aurora::chain()
	template <typename T, typename Tuple>
	struct TupleRange : AbstractRange<T>
	{
		using typename AbstractRange<T>::AbstractIteratorPtr;

		explicit TupleRange(Tuple t)
		: mTuple(std::move(t))
		{
		}

		AbstractRange<T>* getSubRange(std::size_t index)
		{
			typedef AbstractRange<T> CommonBase;
			return dynamicGet<CommonBase>(mTuple, index);
		}

		virtual AbstractIteratorPtr begin()
		{
			return indexify(tupleFront(mTuple).begin(), 0u);
		}

		virtual AbstractIteratorPtr beforeEnd()
		{
			return indexify(tupleBack(mTuple).beforeEnd(), std::tuple_size<Tuple>::value - 1u);
		}

		virtual AbstractIteratorPtr next(AbstractIteratorPtr sink)
		{
			return step(std::move(sink), +1);
		}

		virtual AbstractIteratorPtr prev(AbstractIteratorPtr sink)
		{
			return step(std::move(sink), -1);
		}

		AbstractIteratorPtr step(AbstractIteratorPtr sink, std::ptrdiff_t offset)
		{
			assert(sink);
			std::size_t nextIndex = sink->getSubrangeIndex() + offset;

			// If next subrange available, return its begin (mark iterator with its index)
			if (auto* subrange = getSubRange(nextIndex))
			{
				auto ptr = (offset > 0) ? subrange->begin() : subrange->beforeEnd();
				return indexify(ptr, nextIndex);
			}
			else
			{
				return nullptr;
			}
		}

		AbstractIteratorPtr indexify(AbstractIteratorPtr forwarded, std::size_t index)
		{
			forwarded->setSubrangeIndex(index);
			return forwarded;
		}

		Tuple mTuple;
	};
	
	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	// Maker idiom for IteratorRange
	template <typename T, typename Itr>
	IteratorRange<T, Itr> makeIteratorRange(Itr begin, Itr end)
	{
		return IteratorRange<T, Itr>(begin, end);
	}

	template <typename T, typename Itr>
	typename IteratorRange<T, Itr>::Ptr makeIteratorRangePtr(Itr begin, Itr end)
	{
		return makeCopied<IteratorRange<T, Itr>>(begin, end);
	}

	// Maker for IteratorRange; infer iterators from container 
	template <typename T, typename Container>
	auto makeContainerRange(Container&& c) -> AURORA_AUTO_RETURN
	(
		makeIteratorRange<T>(c.begin(), c.end())
	)

	// Transformer for tupleTransform(), takes any container (or range) and makes an IteratorRange of it
	template <typename T>
	struct ContainerToIteratorRange
	{
		template <typename Range>
		static auto transform(Range&& r) -> AURORA_AUTO_RETURN( makeContainerRange<T>(std::forward<Range>(r)) )
	};

	// Maker idiom for TupleRange
	template <typename T, typename Tuple>
	TupleRange<T, Tuple> makeTupleRange(Tuple&& t)
	{
		return TupleRange<T, Tuple>(std::forward<Tuple>(t));
	}

	// Maker idiom for TupleRange::Ptr
	template <typename T, typename Tuple>
	typename TupleRange<T, Tuple>::Ptr makeTupleRangePtr(Tuple&& t)
	{
		return makeCopied<TupleRange<T, Tuple>>(std::forward<Tuple>(t));
	}

} // namespace detail
} // namespace aurora

#endif // AURORA_ABSTRACTRANGE_HPP
