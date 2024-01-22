# include <Siv3D.hpp>

void Main()
{
	Array<FilePath> paths = Dialog::OpenFiles({ FileFilter{  U"HLSL", { U"hlsl" }} }, FileSystem::CurrentDirectory(), U"コンパイルファイルPS");
	for (const FilePath& path : paths)
	{
		FilePath outPath = path.replaced(U"hlsl", U"ps");
		if (Platform::Windows::CompileHLSLToFile(path, outPath, ShaderStage::Pixel, U"PS")) {
			Print << U"OK << " << path;
		} else {
			Print << U"Error << " << path;
		}
	}
	while (System::Update()) {
	}
}
