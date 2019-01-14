
# include <Siv3D.hpp>

#include"ColorfulTone.h"

//# define SHADERCOMPILE
# ifdef SHADERCOMPILE 
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
		CompileShaderFromFile(L"Shaders/flipPage.hlsl", L"Shaders/flipPage.ps", ShaderType::PS_4_0);
		CompileShaderFromFile(L"Shaders/drawCanvas.hlsl", L"Shaders/drawCanvas.ps", ShaderType::PS_4_0);
	}
}
#endif

void Main()
{

#ifdef SHADERCOMPILE 
	Window::Resize(800, 600);
	::ShadersCompile();
	Graphics::SetBackground(Palette::White);
	Texture tex(L"Resource/Img/Load/bg.png");

	PixelShader ps(L"Shaders/drawCanvas.ps");
	Texture d(L"Work/drawCanvas.png");
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

		Graphics2D::SetTexture(ShaderStage::Pixel, 1, d);
		Graphics2D::SetConstant(ShaderStage::Pixel, 1, cb);
		Graphics2D::BeginPS(ps);
		tex.scale(0.5).drawAt(Window::Center());
		Graphics2D::EndPS();
		//if (Input::KeySpace.pressed)
		//{
		//	d.draw();
		//}
		//auto t2d = Transformer2D(Mat3x2::Scale(0.8,Window::Center()));
		//for (int i = 9; i >=0 ; i--)
		//{
		//	Vec2 pos{ 100.0,600.0 * i / 10.0+45};
		//	Vec2 pos2{ 700.0,600.0 * i / 10.0+45};
		//	if (i % 2) {
		//		std::swap(pos, pos2);
		//	}
		//	pos.y -= 30;
		//	Line(pos, pos2).draw(100,{ Color(255.0*i / 10.0, 0, 0,255), Color(255.0*(i + 1) / 10.0,0,0,255) });
		//}
	}
#else
	ColorfulTone game;

	game.mainLoop();
#endif


}
