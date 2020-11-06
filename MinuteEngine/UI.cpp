/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "UI.h"

#include "imgui.h"

namespace ME {
	bool UI::showCameraWindow = false;
	bool UI::loadCameraWindow = false;
	bool UI::showSceneWindow = false;
	bool UI::loadSceneWindow = false;
	bool UI::showLightWindow = false;
	bool UI::loadLightWindow = false;
	int UI::objectID = -1;
	int UI::propertyID = -1;
	int UI::lightID = -1;

	void UI::draw(Camera& camera, Scene& scene) {
		drawMenuBar();

		if (showCameraWindow)
			draw(camera);
		if (showSceneWindow)
			draw(scene);
		if (showLightWindow)
			draw(scene.getLightManager());
	}
	void UI::drawMenuBar() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Camera"))
			{
				if (ImGui::MenuItem("Config")) { 
					showCameraWindow = true; 
					loadCameraWindow = true;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Config")) { 
					showSceneWindow = true;
					loadSceneWindow = true;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Light")) {
				showLightWindow = true;
				loadLightWindow = true;
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
	void UI::draw(Camera& camera) {
		if (!ImGui::Begin("Camera", &showCameraWindow)) {
			ImGui::End();
			return;
		}
		ImGui::Text("Frame Time : %f (msec)", camera.getFrameTime() * 1000);

		static float inputColor[3];
		static float inputSpeed;
		static float inputNPlane;
		static float inputFPlane;
		static bool inputOrthoMode;

		if (loadCameraWindow) {
			inputColor[0] = camera.getBG().r;
			inputColor[1] = camera.getBG().g;
			inputColor[2] = camera.getBG().b;
			inputSpeed = camera.getSpeed();
			inputNPlane = camera.getNPlane();
			inputFPlane = camera.getFPlane();
			inputOrthoMode = camera.getOrthoMode();
			loadCameraWindow = false;
		}

		ImGui::ColorEdit3("Background", inputColor);
		ImGui::InputFloat("Speed", &inputSpeed);
		ImGui::InputFloat("Near Plane", &inputNPlane);
		ImGui::InputFloat("Far Plane", &inputFPlane);
		ImGui::Checkbox("Ortho Mode", &inputOrthoMode);

		if (ImGui::Button("Apply")) {
			camera.setBG(Color::create(inputColor[0], inputColor[1], inputColor[2]));
			camera.setSpeed(inputSpeed);
			camera.setNPlane(inputNPlane);
			camera.setFPlane(inputFPlane);
			camera.setOrthoMode(inputOrthoMode);
			camera.updateViewMat();
			camera.updateProjMat();
			loadCameraWindow = true;
		}

		if (ImGui::Button("Default Position")) {
			camera.setEye({ CAM_DEF_EYE });
			camera.setCenter({ CAM_DEF_CENTER });
			camera.setUp({ CAM_DEF_UP });
			camera.updateViewMat();
			loadCameraWindow = true;
		}

		ImGui::End();
	}
	void UI::draw(Scene& scene) {
		ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Scene", &showSceneWindow)) {
			objectID = -1;
			propertyID = -1;
			ImGui::End();
			return;
		}

		loadSceneWindow = false;

		// 1. Object window.
		ImGui::BeginChild("Select Object", ImVec2(100.0f, 0.0f), true);

		ImGui::Text("Objects");
		//ImGui::SameLine();
		// 1-1. Add new object.
		/*if (ImGui::Button("Add"))
			ImGui::OpenPopup("Add Object Popup");
		if (ImGui::BeginPopup("Add Object Popup"))
		{
			ImGui::PushItemWidth(100);
			static char name[64] = "Name"; ImGui::InputText("", name, 64);
			ImGui::PopItemWidth();

			if (ImGui::Button("Apply")) {
				auto object = Object::createPtr(name);
				scene.addObject(object);
			}
			ImGui::EndPopup();
		}*/

		ImGui::Separator();

		// 1-2. Show Objects.
		if (scene.getGraph().empty()) {
			ImGui::Text("None");
		}
		else {
			for (auto node : scene.getGraph()) {
				auto& object = node.second.getObject();
				if (ImGui::Selectable(object->getName().c_str())) {
					if (objectID != node.first)
						loadSceneWindow = true;
					objectID = node.first;
					propertyID = -1;
				}
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		// 2. Property window.
		ImGui::BeginChild("Select Property", ImVec2(130.0f, 0.0f), true);

		ImGui::Text("Properties");
		// 2-1. Add new property.
		/*if (objectID != -1) {
			auto& object = scene.getNode(objectID).getObject();
			ImGui::SameLine();
			if (ImGui::Button("Add"))
				ImGui::OpenPopup("Add Property Popup");
			if (ImGui::BeginPopup("Add Property Popup"))
			{
				if (ImGui::BeginMenu("Render")) {
					if (ImGui::MenuItem("Point")) {
						add_property_render_point(scene_man, target_object);
					}
					if (ImGui::MenuItem("Line")) {
						add_property_render_line(scene_man, target_object);
					}
					if (ImGui::MenuItem("Triangle")) {
						add_property_render_triangle(scene_man, target_object);
					}
					if (ImGui::MenuItem("Quad")) {
						add_property_render_quad(scene_man, target_object);
					}
					if (ImGui::MenuItem("Box")) {
						add_property_render_box(scene_man, target_object);
					}
					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}
		}*/
		ImGui::Separator();

		// 2-2. Show properties.
		if (objectID != -1) {
			auto& object = scene.getNode(objectID).getObject();

			int propSize = object->propertySize();
			if (propSize == 0) {
				ImGui::Text("None");
			}
			else {
				for (auto prop : object->getProperties()) {
					ImGui::PushID(prop.first);
					if (ImGui::Selectable(prop.second->getName().c_str())) {
						if (propertyID != prop.first)
							loadSceneWindow = true;
						propertyID = prop.first;
					}
					ImGui::PopID();
				}
			}
		}

		ImGui::EndChild();

		ImGui::SameLine();

		// 3. Property config window.
		ImGui::BeginChild("Property Config", ImVec2(0.0f, 0.0f), true);

		if (objectID != -1) {
			auto& object = scene.getNode(objectID).getObject();
			if (propertyID == -1) {
				// Object config : Dynamic update
				draw(*object);
			}
			else {
				// Property config
				auto prop = object->getProperty(propertyID);
				if (prop != nullptr)
					draw(*prop, loadSceneWindow);
				ImGui::Separator();
				if (prop != nullptr && prop->getRender() != nullptr)
					draw(*prop->getRender(), loadSceneWindow);
			}
		}
		ImGui::EndChild();
		ImGui::End();
	}
	void UI::draw(LightManager& lightManager) {
		if (!ImGui::Begin("Light", &showLightWindow)) {
			ImGui::End();
			return;
		}

		// 1. Light select box
		ImGui::BeginChild("Select Light", ImVec2(100.0f, 0.0f), true);

		ImGui::Text("Lights");
		ImGui::Separator();

		int num = lightManager.lights.size();
		for (int i = 0; i < num; i++) {
			auto& light = lightManager.lights[i];
			ImGui::PushID(i);
			std::string name = light.getName() + "(" + std::to_string(i) + ")";
			if (ImGui::Selectable(name.c_str())) {
				if (i != lightID)
					loadLightWindow = true;
				lightID = i;
			}
			ImGui::PopID();
		}
		ImGui::EndChild();
		ImGui::SameLine();

		// 2. Light config box
		ImGui::BeginChild("Light config", ImVec2(0.0f, 0.0f), true);

		if (lightID != -1) {
			Light inputLight = lightManager.lights[lightID];

			// Type
			const char* types[] = { "Directional", "Point" };
			const char* currentItem = types[inputLight.getType()];
			if (ImGui::BeginCombo("Type", currentItem)) {
				for (int i = 0; i < IM_ARRAYSIZE(types); i++) {
					bool isSelected = (currentItem == types[i]);
					if (ImGui::Selectable(types[i], isSelected)) {
						currentItem = types[i];
						inputLight.setType(i);
					}	
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			// Position
			static float pos[3];
			for (int i = 0; i < 3; i++)
				pos[i] = inputLight.getPosition()[i];
			ImGui::DragFloat3("Position", pos);
			inputLight.setPosition({ pos[0], pos[1], pos[2] });

			// Direction
			static float dir[3];
			for (int i = 0; i < 3; i++)
				dir[i] = inputLight.getDirection()[i];
			ImGui::DragFloat3("Direction", dir);
			inputLight.setDirection({ dir[0], dir[1], dir[2] });

			// Color
			static float ambient[3];
			static float diffuse[3];
			static float specular[3];
			for (int i = 0; i < 3; i++) {
				ambient[i] = inputLight.getAmbient()[i];
				diffuse[i] = inputLight.getDiffuse()[i];
				specular[i] = inputLight.getSpecular()[i];
			}
			ImGui::ColorEdit3("Ambient", ambient);
			ImGui::ColorEdit3("Diffuse", diffuse);
			ImGui::ColorEdit3("Specular", specular);
			inputLight.setAmbient(ME::Color::create(ambient[0], ambient[1], ambient[2]));
			inputLight.setDiffuse(ME::Color::create(diffuse[0], diffuse[1], diffuse[2]));
			inputLight.setSpecular(ME::Color::create(specular[0], specular[1], specular[2]));

			// Shadow
			bool useShadow = inputLight.shadow.getValid();
			ImGui::Checkbox("Use Shadow", &useShadow);
			inputLight.shadow.setValid(useShadow);

			lightManager.lights[lightID] = inputLight;
		}


		ImGui::EndChild();
		ImGui::SameLine();
		

		ImGui::End();
	}
	void UI::draw(Object& object) {
		float inputTranslation[3] = { 0.0f, 0.0f, 0.0f };
		float inputRotation[3] = { 0.0f, 0.0f, 0.0f };
		float transStep = 0.01f;
		float rotStep = 0.1f;
		auto& transform = object.getTransform();

		ImGui::SliderFloat3("Translation", inputTranslation, -0.01f, 0.01f);
		ImGui::SliderFloat3("Rotation", inputRotation, -0.01f, 0.01f);

		transform.translate({ inputTranslation[0], inputTranslation[1], inputTranslation[2] });
		transform.rotate({ 1.0f, 0.0f, 0.0f }, inputRotation[0]);
		transform.rotate({ 0.0f, 1.0f, 0.0f }, inputRotation[1]);
		transform.rotate({ 0.0f, 0.0f, 1.0f }, inputRotation[2]);
	}
	void UI::draw(Property& property, bool update) {
		property.drawUI(update);
		return;
	}
	void UI::draw(Render& render, bool update) {
		render.drawUI(update);
		return;
	}
}