/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_COLOR_H__
#define __ME_COLOR_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "glm/vec4.hpp"

namespace ME {
	class Color : public glm::vec4 {
	public:
		static Color create(float r, float g, float b, float a = 1.0f);
		static Color white() noexcept;
		static Color black() noexcept;
		static Color red() noexcept;
		static Color green() noexcept;
		static Color blue() noexcept;
		static Color random(bool randOpacity = false) noexcept;
	};
}

#endif