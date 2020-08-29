/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Texture.h"
#include "Utils.h"

#include <GL/glew.h>
#include <SDL_opengl.h>

#define STB_IMAGE_IMPLEMENTATION // https://learnopengl.com/Getting-started/Textures
#include "stbi/stb_image.h"

#include <stdexcept>

namespace ME {
	// Texture2D
	Texture2D Texture2D::create(const std::string& path) {
		Texture2D texture;
		glGenTextures(1, &texture.id);
		if (!path.empty()) 
			texture.loadImage(path);
		return texture;
	}
	void Texture2D::loadImage(const std::string& path) {
		int width, height, nrChannels;
		uchar* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		auto size = sizeof(data);

		if (!data)
			throw(std::runtime_error("Texture image load failed"));
		/*if (width > GL_MAX_TEXTURE_SIZE || height > GL_MAX_TEXTURE_SIZE)
			throw(std::runtime_error("Texture image is too big"));*/

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);

		/* Warning : Parameters must be set! If not, only black color will be displayed */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// @arg0 : Texture type
		// @arg1 : Mipmap level
		// @arg2 : Format of which we want to store the texture
		// @arg3,4 : Width and height of image
		// @arg5 : Have to be 0 ( Legacy )
		// @arg6 : Format of source image
		// @arg7 : Image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		
		stbi_image_free(data);
	}

	// TextureCube
	TextureCube TextureCube::create(const std::string path[6]) {
		TextureCube texture;
		glGenTextures(1, &texture.id);
		texture.loadImage(path);
		return texture;
	}
	void TextureCube::loadImage(const std::string path[6]) {
		glEnable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		for (int i = 0; i < 6; i++) {
			int width, height, nrChannels;
			uchar* data = stbi_load(path[i].c_str(), &width, &height, &nrChannels, 0);
			auto size = sizeof(data);

			if (!data)
				throw(std::runtime_error("Texture image load failed"));
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		/* Warning : Parameters must be set! If not, only black color will be displayed */
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}