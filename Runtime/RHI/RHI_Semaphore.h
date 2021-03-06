/*
Copyright(c) 2016-2020 Panos Karabelas

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

//= INCLUDES ======================
#include "../Core/Spartan_Object.h"
#include "RHI_Definition.h"
//=================================

namespace Spartan
{
    class RHI_Semaphore : public Spartan_Object
    {
    public:
        RHI_Semaphore(RHI_Device* rhi_device, const char* name = nullptr);
        ~RHI_Semaphore();

        void* GetResource() { return m_resource; }

        RHI_Semaphore_State GetState()                  const { return m_state; }
        void SetState(const RHI_Semaphore_State state)        { m_state = state; }
    private:
        void* m_resource = nullptr;
        RHI_Semaphore_State m_state = RHI_Semaphore_State::Idle;
        RHI_Device* m_rhi_device = nullptr;
    };
}
