#include "Model.h"

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

Model::Model(Mesh* mesh, Texture* texture, std::vector<Shader*>& shaders)
{
	this->mesh = mesh;
	this->texture = texture;
	this->shaders = shaders;
}

Model::~Model()
{
	delete mesh;
	delete texture;

	for (const Shader* shader : shaders)
	{
		delete shader;
	}
}
