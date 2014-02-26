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

namespace aurora
{

template <class B, typename R, class Traits>
DoubleDispatcher<B, R, Traits>::DoubleDispatcher(bool symmetric)
: mMap()
, mFallback()
, mSymmetric(symmetric)
{
}

template <class B, typename R, typename Traits>
template <typename Id1, typename Id2, typename Fn>
void DoubleDispatcher<B, R, Traits>::bind(Id1 identifier1, Id2 identifier2, Fn function)
{
	SingleKey key1 = Traits::keyFromId(identifier1);
	SingleKey key2 = Traits::keyFromId(identifier2);

	mMap[makeKey(key1, key2)] = Traits::template trampoline2<Id1, Id2>(function);
}

template <class B, typename R, typename Traits>
R DoubleDispatcher<B, R, Traits>::call(B arg1, B arg2) const
{
	SingleKey key1 = Traits::keyFromBase(arg1);
	SingleKey key2 = Traits::keyFromBase(arg2);

	// If no corresponding class (or base class) has been found: Invoke fallback if available, otherwise throw exception
	Key key = makeKey(key1, key2);
	auto itr = mMap.find(key);
	if (itr == mMap.end())
	{
		if (mFallback)
			return mFallback(arg1, arg2);
		else
			throw FunctionCallException(std::string("DoubleDispatcher::call() - function with parameters \"") + Traits::name(key1)
				+ "\" and \"" + Traits::name(key2) + "\" not registered");
	}

	// Call function (swap-flag equal for stored entry and passed arguments means the order was the same; otherwise swap arguments)
	if (itr->first.swapped == key.swapped)
		return itr->second(arg1, arg2);
	else
		return itr->second(arg2, arg1);
}

template <class B, typename R, class Traits>
void DoubleDispatcher<B, R, Traits>::fallback(std::function<R(B, B)> function)
{
	mFallback = std::move(function);
}

template <class B, typename R, class Traits>
typename DoubleDispatcher<B, R, Traits>::Key DoubleDispatcher<B, R, Traits>::makeKey(SingleKey key1, SingleKey key2) const
{
	// When symmetric, (key1,key2) and (key2,key1) are the same -> sort so that we always have (key1,key2)
	if (mSymmetric && hashValue(key2) < hashValue(key1))
		return Key(key2, key1, true);
	else
		return Key(key1, key2, false);
}

template <class B, typename R, class Traits>
DoubleDispatcher<B, R, Traits>::Key::Key(const SingleKey& key1, const SingleKey& key2, bool swapped)
: keyPair(key1, key2)
, swapped(swapped)
{
}

template <class B, typename R, class Traits>
bool DoubleDispatcher<B, R, Traits>::Key::operator== (const Key& rhs) const
{
	// Member 'swapped' not relevant for key lookup
	return keyPair == rhs.keyPair;
}

template <class B, typename R, class Traits>
std::size_t DoubleDispatcher<B, R, Traits>::Hasher::operator() (const Key& k) const
{
	return PairHasher()(k.keyPair);
}

} // namespace aurora
