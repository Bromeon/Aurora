/////////////////////////////////////////////////////////////////////////////////
//
// Aurora C++ Library
// Copyright (c) 2012-2013 Jan Haller
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

	// If no corresponding class (or base class) has been found, throw exception
	auto itr = mMap.find(makeKey(key1, key2));
	if (itr == mMap.end())
		throw FunctionCallException(std::string("DoubleDispatcher::call() - function with parameters \"") + Traits::name(key1)
		+ "\" and \"" + Traits::name(key2) + "\" not registered");

	// Otherwise, call dispatched function
	return itr->second(arg1, arg2);
}

template <class B, typename R, class Traits>
typename DoubleDispatcher<B, R, Traits>::Key DoubleDispatcher<B, R, Traits>::makeKey(SingleKey key1, SingleKey key2) const
{
	// When symmetric, (key1,key2) and (key2,key1) are the same -> sort so that we always have (key1,key2)
	if (mSymmetric && hashValue(key2) < hashValue(key1))
		std::swap(key1, key2);

	return Key(key1, key2);
}

} // namespace aurora
