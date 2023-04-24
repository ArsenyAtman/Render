#pragma once

#include <string>

struct ApplicationSettings;

class ShaderCompiler
{
public:

	ShaderCompiler(const ApplicationSettings* settings);
	virtual ~ShaderCompiler();

private:

	void compileShader(const std::string& pathToCompiler, const std::string& pathToShader, const std::string& compileTo);

	std::string temporaryBatFile;
};
