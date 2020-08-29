/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_UI_H__
#define __ME_UI_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Camera.h"
#include "Scene.h"

namespace ME {
	class UI {
	private:
		static bool showCameraWindow;
		static bool loadCameraWindow;
		static bool showSceneWindow;
		static bool loadSceneWindow;
		static bool showLightWindow;
		static bool loadLightWindow;
		static int objectID;
		static int propertyID;
		static int lightID;
	public:
		static void draw(Camera& camera, Scene& scene);
		static void drawMenuBar();
		static void draw(Camera& camera);
		static void draw(Scene& scene);
		static void draw(LightManager& lightManager);
		static void draw(Object& object);
		static void draw(Property& property, bool update);
		static void draw(Render& render, bool update);
	};
}

#endif