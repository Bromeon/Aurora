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

template <typename Signature, typename Traits>
SingleDispatcher<Signature, Traits>::SingleDispatcher()
: mMap()
{
}

template <typename Signature, typename Traits>
template <typename Id, typename Fn>
void SingleDispatcher<Signature, Traits>::bind(Id identifier, Fn function)
{
	mMap[Traits::keyFromId(identifier)] = Traits::template trampoline1<Id>(function);
}

template <typename Signature, typename Traits>
typename SingleDispatcher<Signature, Traits>::Result SingleDispatcher<Signature, Traits>::call(Parameter arg) const
{
	Key key = Traits::keyFromBase(arg);

	// If no corresponding class (or base class) has been found, throw exception
	auto itr = mMap.find(key);
	if (itr == mMap.end())
	{
		if (mFallback)
			return mFallback(arg);
		else
			throw FunctionCallException(std::string("SingleDispatcher::call() - function with parameter \"") + Traits::name(key) + "\" not registered");
	}

	// Otherwise, call dispatched function
	return itr->second(arg);
}

template <typename Signature, typename Traits>
void SingleDispatcher<Signature, Traits>::fallback(std::function<Signature> function)
{
	mFallback = std::move(function);
}

} // namespace aurora
