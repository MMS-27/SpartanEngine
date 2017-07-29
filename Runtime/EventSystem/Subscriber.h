/*
Copyright(c) 2016-2017 Panos Karabelas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

//= INCLUDES ==============
#include <functional>
#include "../Core/Helper.h"
//=========================

namespace Directus
{
	class DLL_API Subscriber
	{
	public:
		typedef std::function<void()> functionType;

		Subscriber(int eventID, functionType&& subFunc)
		{
			m_eventID = eventID;
			m_subscribedFunction = std::forward<functionType>(subFunc);
		}

		void Call()
		{
			m_subscribedFunction();
		}

		int GetEventID() { return m_eventID; }
		std::size_t GetAddress() { return getAddress(m_subscribedFunction); }

	private:
		template<typename T, typename... U>
		size_t getAddress(std::function<T(U...)> f) 
		{
			typedef T(fnType)(U...);
			fnType ** fnPointer = f.template target<fnType*>();
			return (size_t)*fnPointer;
		}

		int m_eventID;
		functionType m_subscribedFunction;
	};
}