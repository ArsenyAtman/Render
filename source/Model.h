#pragma once

#include <vector>

class Mesh;
class Texture;
class Shader;

class Model
{
public:

	Model(Mesh* mesh, Texture* texture, std::vector<Shader*>& shaders);
	virtual ~Model();

	const Mesh* getMesh() const { return mesh; }
	const Texture* getTexture() const { return texture; }
	const std::vector<Shader*>& getShaders() const { return shaders; }

private:

	Mesh* mesh = nullptr;
	Texture* texture = nullptr;
	std::vector<Shader*> shaders;

};
