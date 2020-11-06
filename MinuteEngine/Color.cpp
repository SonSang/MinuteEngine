/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Color.h"
#include "glm/gtc/random.hpp"
#include <stdexcept>

namespace ME {
	Color Color::create(float r, float g, float b, float a) {
		Color c;
		c.r = r;
		c.g = g;
		c.b = b;
		c.a = a;
		return c;
	}
	Color Color::white() noexcept {
		return create(1.0f, 1.0f, 1.0f);
	}
	Color Color::black() noexcept {
		return create(0.0f, 0.0f, 0.0f);
	}
	Color Color::red() noexcept {
		return create(1.0f, 0.0f, 0.0f);
	}
	Color Color::green() noexcept {
		return create(0.0f, 1.0f, 0.0f);
	}
	Color Color::blue() noexcept {
		return create(0.0f, 0.0f, 1.0f);
	}
	Color Color::yellow() noexcept {
		return create(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	}
	Color Color::pink() noexcept {
		return create(255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f, 1.0f);
	}
	Color Color::cyan() noexcept {
		return create(0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);
	}
	Color Color::brown() noexcept {
		return create(128.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	}
	Color Color::beige() noexcept {
		return create(255.0f / 255.0f, 255.0f / 255.0f, 204.0f / 255.0f, 1.0f);
	}
	Color Color::purple() noexcept {
		return create(153.0f / 255.0f, 51.0f / 255.0f, 102.0f / 255.0f, 1.0f);
	}
	Color Color::random(bool randOpacity) noexcept {
		Color c;
		c[0] = glm::linearRand(0.0f, 1.0f);
		c[1] = glm::linearRand(0.0f, 1.0f);
		c[2] = glm::linearRand(0.0f, 1.0f);
		if (randOpacity)
			c[3] = glm::linearRand(0.0f, 1.0f);
		else
			c[3] = 1.0f;
		return c;
	}
}