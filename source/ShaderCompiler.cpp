#include "ShaderCompiler.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "Settings.h"

ShaderCompiler::ShaderCompiler(const ApplicationSettings* settings)
{
	this->temporaryBatFile = settings->temporaryBatFile;

	for (size_t i = 0; i < settings->shadersToCompile.size(); ++i)
	{
		compileShader(settings->pathToCompiler, settings->shadersToCompile[i], settings->compileShadersTo[i]);
	}
}

ShaderCompiler::~ShaderCompiler()
{
	std::filesystem::remove(temporaryBatFile);
}

void ShaderCompiler::compileShader(const std::string& pathToCompiler, const std::string& pathToShader, const std::string& compileTo)
{
	std::ofstream file;
	file.open(temporaryBatFile, std::ios::out);

	file << "\""<< pathToCompiler << "\" \"%~dp0" << pathToShader << "\" -o \"%~dp0" << compileTo << "\"\n";

	file.close();

	system(temporaryBatFile.c_str());
}
