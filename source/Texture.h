#pragma once

#include <cstdint>

class Texture
{
public:

	Texture(uint32_t width, uint32_t height, uint32_t countOfChannels, void* data);
	virtual ~Texture();

	uint32_t getSize() const { return size; }

	uint32_t getWidth() const { return width; }
	uint32_t getHeight() const { return height; }
	uint32_t getCountOfChannels() const { return countOfChannels; }
	void* getData() const { return pixels; }

private:

	uint32_t size;

	uint32_t width;
	uint32_t height;
	uint32_t countOfChannels;
	void* pixels;
};
