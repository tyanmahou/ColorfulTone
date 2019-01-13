
# include <Siv3D.hpp>

#include"ColorfulTone.h"

//#define SHADERCOMPILE
#ifdef SHADERCOMPILE 
# include <d3d11.h>
# include <d3dcompiler.h>
#pragma comment( lib, "d3dcompiler.lib" )
namespace
{
	bool CompileShaderFromFile(const FilePath& from, const FilePath& to, ShaderType shaderType=ShaderType::PS_4_0,
		const String& entryPoint=L"PS", int32 flags = ShaderCompileFlag::Default)
	{

		ID3DBlob* pBlobOut = nullptr;
		ID3DBlob* pErrorBlob = nullptr;
		const char* target = (shaderType == ShaderType::VS_4_0 ? "vs_4_0" : "ps_4_0");
		const HRESULT hr = D3DCompileFromFile(from.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.narrow().c_str(), target, flags, 0, &pBlobOut, &pErrorBlob);

		if (pErrorBlob)
		{
			Println(L"Error 3");

			::OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

			pErrorBlob->Release();
		}

		if (FAILED(hr))
		{
			Println(L"Error 4");

			if (pBlobOut)
			{
				Println(L"Error 5");

				pBlobOut->Release();
			}

			return false;
		}

		BinaryWriter(to).write(pBlobOut->GetBufferPointer(), pBlobOut->GetBufferSize());

		pBlobOut->Release();

		Println(L"OK");

		return true;
	}



	void ShadersCompile(bool allCompile = false)
	{
		CompileShaderFromFile(L"Shaders/mainBG.hlsl", L"Shaders/mainBG.ps", ShaderType::PS_4_0);
		CompileShaderFromFile(L"Shaders/ResultBG.hlsl", L"Shaders/ResultBG.ps", ShaderType::PS_4_0);
		CompileShaderFromFile(L"Shaders/startTrans.hlsl", L"Shaders/startTrans.ps", ShaderType::PS_4_0);
		CompileShaderFromFile(L"Shaders/radialBlur.hlsl", L"Shaders/radialBlur.ps", ShaderType::PS_4_0);
		CompileShaderFromFile(L"Shaders/pageTransaction.hlsl", L"Shaders/pageTransaction.ps", ShaderType::PS_4_0);
	}
}
#endif

void Main()
{

#ifdef SHADERCOMPILE 
	Window::Resize(800, 600);
	::ShadersCompile();
	Texture tex(L"Resource/Img/Load/bg.png");

	PixelShader ps(L"Shaders/pageTransaction.ps");

	ConstantBuffer<Float4> cb;
	cb->x = 0;
	while (System::Update())
	{
		if (Input::KeyUp.pressed)
		{
			cb->x += 0.01;
		}
		if (Input::KeyDown.pressed)
		{
			cb->x -= 0.01;
		}
		if (Input::KeySpace.clicked)
		{
			Println(cb->x);
		}
		Graphics2D::SetConstant(ShaderStage::Pixel, 1, cb);
		Graphics2D::BeginPS(ps);
		tex.draw();
		Graphics2D::EndPS();
	}
#else
	ColorfulTone game;

	game.mainLoop();
#endif


}
