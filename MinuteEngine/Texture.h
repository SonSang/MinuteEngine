/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_TEXTURE_H__
#define __ME_TEXTURE_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Shader.h"
#include <string>

namespace ME {
	class Texture2D {
	public:
		uint id;			// Specifies ID of this texture : [ glBindTexture(type, { id }) ]
	public:
		static Texture2D create(const std::string& path = "");
		void loadImage(const std::string& path);
	};

	class TextureCube {
	public:
		uint id;			// Specifies ID of this texture : [ glBindTexture(type, { id }) ]
	public:
		static TextureCube create(const std::string path[6]);
		void loadImage(const std::string path[6]);
	};
}

#endif