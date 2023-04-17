#include "Texture.h"

Texture::Texture(uint32_t width, uint32_t height, uint32_t countOfChannels, void* pixels)
{
	this->width = width;
	this->height = height;
	this->countOfChannels = countOfChannels;
	this->pixels = pixels;

	size = width * height * 4;
}

Texture::~Texture()
{
	delete pixels;
}