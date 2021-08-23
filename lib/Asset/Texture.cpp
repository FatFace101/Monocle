
#include <cstdio>
#include <stdlib.h>
#include "monocle/Asset/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
namespace stb {
	#include "stb/stb_image.h"
}


using namespace mncl;


Texture::Texture(const char* filename)  {
	stb::stbi_set_flip_vertically_on_load(true);
	int channels;
	unsigned char* data = stb::stbi_load(filename, (int*)&width, (int*)&height, &channels, 0);
	generate(data, channels);
	stb::stbi_image_free(data);
}

Texture::Texture(FILE* file) {
	stb::stbi_set_flip_vertically_on_load(true);
	int channels;
	unsigned char* data = stb::stbi_load_from_file(file, (int*)&width, (int*)&height, &channels, 0);
	generate(data, channels);
	stb::stbi_image_free(data);
}


void Texture::generate(unsigned char* data, int channels) {
	if (data) {
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		GLenum format;
		printf("Loading texture with %i channels.\n", channels);
		if (channels == 3) {
			format = GL_RGB;
		}
		if (channels == 4) {
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		texId = 0;
		printf("Couldn't generate texture.");
	}
	
};


bool Texture::bind(unsigned int slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texId);
	return true;
}