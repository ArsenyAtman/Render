#include "ModelLoader.h"

#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Settings.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"

ModelLoader::ModelLoader()
{
    // ...
}

Model* ModelLoader::loadModel(const ApplicationSettings* settings)
{
    Mesh* mesh = loadMesh(settings->meshPath);
    Texture* texture = loadTexture(settings->texturePath);

    std::vector<Shader*> shaders;
    for (const std::string& shaderPath : settings->shaderPaths)
    {
        shaders.push_back(loadShader(shaderPath));
    }

    return new Model(mesh, texture, shaders);
}

Mesh* ModelLoader::loadMesh(const std::string& meshPath)
{
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &err, meshPath.c_str()))
    {
        throw std::runtime_error(warn + err);
    }

    std::vector<Vertex> vertices;

    for (const tinyobj::shape_t& shape : shapes)
    {
        for (const tinyobj::index_t& index : shape.mesh.indices)
        {
            Vertex vertex{};

            vertex.position = {
                attributes.vertices[3 * index.vertex_index + 0],
                attributes.vertices[3 * index.vertex_index + 1],
                attributes.vertices[3 * index.vertex_index + 2]
            };

            vertex.textureCoordinate = {
                attributes.texcoords[2 * index.texcoord_index + 0],
                1.0f - attributes.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            vertices.push_back(vertex);
        }
    }

    return new Mesh(vertices);
}

Texture* ModelLoader::loadTexture(const std::string& texturePath)
{
    int textureWidth;
    int textureHeight;
    int textureChannels;

    stbi_uc* pixels = stbi_load(texturePath.c_str(), &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha);
    if (!pixels)
    {
        throw std::runtime_error("Failed to load texture!");
    }

    int size = textureWidth * textureHeight * 4;

    void* data = new char[size];
    memcpy(data, pixels, size);

    stbi_image_free(pixels);

    return new Texture(textureWidth, textureHeight, textureChannels, data);
}

Shader* ModelLoader::loadShader(const std::string& shaderPath)
{
    std::ifstream file(shaderPath.c_str(), std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open a file!");
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> code(fileSize);

    file.seekg(0);
    file.read(code.data(), fileSize);

    file.close();

    ShaderType shaderType = (shaderPath.find("vert") != std::string::npos) ? ShaderType::Vertex : ShaderType::Fragment;

    return new Shader(shaderType, code);
}
