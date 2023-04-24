#pragma once

#include <string>

struct ApplicationSettings;
class Model;
class Mesh;
class Texture;
class Shader;

class ModelLoader
{
	
public:

	ModelLoader();

	Model* loadModel(const ApplicationSettings* settings);

	Mesh* loadMesh(const std::string& meshPath);
	Texture* loadTexture(const std::string& texturePath);
	Shader* loadShader(const std::string& shaderPath);
};
