/*
Copyright(c) 2016-2018 Panos Karabelas

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

//= INCLUDES ================================
#include "LightShader.h"
#include "../../Scene/Components/Transform.h"
#include "../../Core/Settings.h"
#include "../../RHI/RHI_Implementation.h"
#include "../../RHI/RHI_Shader.h"
#include "../../RHI/RHI_ConstantBuffer.h"
#include "../../Scene/Actor.h"
//===========================================

//= NAMESPACES ================
using namespace std;
using namespace Directus::Math;
//=============================

namespace Directus
{
	LightShader::LightShader()
	{
		m_rhiDevice = nullptr;
	}

	void LightShader::Compile(const string& filePath, shared_ptr<RHI_Device> rhiDevice, Context* context)
	{
		m_rhiDevice = rhiDevice;

		// Load the vertex and the pixel shader
		m_shader = make_shared<RHI_Shader>(m_rhiDevice);
		m_shader->Compile_VertexPixel_Async(filePath, Input_PositionTextureTBN, context);

		// Create constant buffer
		m_cbuffer = make_shared<RHI_ConstantBuffer>(m_rhiDevice);
		m_cbuffer->Create(sizeof(LightBuffer), 0, Buffer_Global);
	}

	void LightShader::UpdateConstantBuffer(
		const Matrix& mWorld,
		const Matrix& mView,
		const Matrix& mBaseView,
		const Matrix& mPerspectiveProjection,
		const Matrix& mOrthographicProjection,
		const vector<weak_ptr<Actor>>& lights,
		Camera* camera
	)
	{
		if (!IsCompiled())
		{
			LOG_ERROR("Failed to compile deferred shader.");
			return;
		}

		if (!camera || lights.empty())
			return;

		// Get a pointer to the data in the constant buffer.
		auto buffer = (LightBuffer*)m_cbuffer->Map();

		// Get some stuff
		Matrix worlBaseViewProjection	= mWorld * mBaseView * mOrthographicProjection;
		Matrix viewProjection			= mView * mPerspectiveProjection;
		Vector3 camPos					= camera->GetTransform()->GetPosition();

		buffer->cameraPosition	= Vector4(camPos.x, camPos.y, camPos.z, 1.0f);
		buffer->m_wvp			= worlBaseViewProjection;
		buffer->m_vpInv			= viewProjection.Inverted();

		// Reset any light buffer values because the shader will still use them
		buffer->dirLightColor = Vector4::Zero;
		buffer->dirLightDirection = Vector4::Zero;
		buffer->dirLightIntensity = Vector4::Zero;
		for (int i = 0; i < maxLights; i++)
		{
			buffer->pointLightPosition[i]		= Vector4::Zero;
			buffer->pointLightColor[i]			= Vector4::Zero;
			buffer->pointLightIntenRange[i]		= Vector4::Zero;
			buffer->spotLightPosition[i]		= Vector4::Zero;
			buffer->spotLightColor[i]			= Vector4::Zero;
			buffer->spotLightIntenRangeAngle[i] = Vector4::Zero;
		}

		// Fill with directional lights
		for (const auto& lightWeak : lights)
		{
			auto light		= lightWeak.lock();
			auto component	= light->GetComponent<Light>().lock();

			if (component->GetLightType() != LightType_Directional)
				continue;

			Vector3 direction = component->GetDirection();

			buffer->dirLightColor = component->GetColor();
			buffer->dirLightIntensity = Vector4(component->GetIntensity());
			buffer->dirLightDirection = Vector4(direction.x, direction.y, direction.z, 0.0f);
		}

		// Fill with point lights
		int pointIndex = 0;
		for (const auto& lightWeak : lights)
		{
			auto light		= lightWeak.lock();
			auto component	= light->GetComponent<Light>().lock();

			if (component->GetLightType() != LightType_Point)
				continue;

			Vector3 pos = light->GetTransform_PtrRaw()->GetPosition();

			buffer->pointLightPosition[pointIndex]		= Vector4(pos.x, pos.y, pos.z, 1.0f);
			buffer->pointLightColor[pointIndex]			= component->GetColor();
			buffer->pointLightIntenRange[pointIndex]	= Vector4(component->GetIntensity(), component->GetRange(), 0.0f, 0.0f);

			pointIndex++;
		}

		// Fill with spot lights
		int spotIndex = 0;
		for (const auto& lightWeak : lights)
		{
			auto light		= lightWeak.lock();
			auto component	= light->GetComponent<Light>().lock();

			if (component->GetLightType() != LightType_Spot)
				continue;

			Vector3 direction	= component->GetDirection();
			Vector3 pos			= light->GetTransform_PtrRaw()->GetPosition();

			buffer->spotLightColor[spotIndex]			= component->GetColor();
			buffer->spotLightPosition[spotIndex]		= Vector4(pos.x, pos.y, pos.z, 1.0f);
			buffer->spotLightDirection[spotIndex]		= Vector4(direction.x, direction.y, direction.z, 0.0f);
			buffer->spotLightIntenRangeAngle[spotIndex] = Vector4(component->GetIntensity(), component->GetRange(), component->GetAngle(), 0.0f);

			spotIndex++;
		}

		buffer->pointLightCount = (float)pointIndex;
		buffer->spotLightCount	= (float)spotIndex;
		buffer->nearPlane		= camera->GetNearPlane();
		buffer->farPlane		= camera->GetFarPlane();
		buffer->viewport		= Settings::Get().GetResolution();
		buffer->padding			= Vector2::Zero;

		// Unmap buffer
		m_cbuffer->Unmap();
	}

	bool LightShader::IsCompiled()
	{
		return m_shader ? m_shader->IsCompiled() : false;
	}
}
