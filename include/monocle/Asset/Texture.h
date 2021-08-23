#ifndef MNCL_TEXTURE_H
#define MNCL_TEXTURE_H

#include "glad/glad.h"
#include "monocle/Support/StreamReader.h"
#include <stdio.h>

namespace mncl {
	class Texture {
	private:
		unsigned int width;
		unsigned int height;
		GLuint texId;
		
		void generate(unsigned char* data, int channels);
	public:
		GLuint getTexture() {return texId;};


		bool bind(unsigned int slot);
		Texture(const char* filename);
		Texture(FILE* file);

	};

};



#endif