/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_MOUSE_H__
#define __ME_MOUSE_H__

#ifdef _MSC_VER
#pragma once
#endif

//#define MOUSE_DEF_SENSITIVITY 0.03f
#define MOUSE_DEF_SENSITIVITY 0.3f

#include "Camera.h"

namespace ME {
	class Mouse {
	private:
		float x = 0.0f;			// X coordinates on screen
		float y = 0.0f;			// Y coordinates on screen
		bool dirty = false;		// If false, there has been no mouse input

		bool LPress = false;
		bool RPress = false;
		bool MPress = false;

		float sensitivity = MOUSE_DEF_SENSITIVITY;
	public:
		static Mouse create();

		void setCoords(float x, float y);
		void setDirty(bool dirty);
		void setLPress(bool p);
		void setRPress(bool p);
		void setMPress(bool p);
		void setSensitivity(float sensitivity);

		float getX() const noexcept;
		float getY() const noexcept;
		bool getDirty() const noexcept;
		bool getLPress() const noexcept;
		bool getRPress() const noexcept;
		bool getMPress() const noexcept;
		float getSensitivity() const noexcept;

		void moveCamera(Camera& cam, float nx, float ny);
		void rotCamera(Camera& cam, float nx, float ny);
		void panCamera(Camera& cam, float nx, float ny);
	};
}

#endif