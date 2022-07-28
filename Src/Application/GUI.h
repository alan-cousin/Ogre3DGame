#pragma once
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <glm/glm.hpp>
#include "OIS.h"


	class GUI {
	public:
		void init(const std::string& resourceDirectory);
		void destroy();

		void draw();

		void moveMouse(int x, int y);
		void moveJoystick(int x, int y);
		void downMouse(OIS::MouseButtonID id);
		void upMouse(OIS::MouseButtonID id);
		void downButton(std::string id);
		void upButton(std::string id);
		void setMouseCursor(const std::string&imageFile);
		void showMouseCursor();
		void hideMouseCursor();

		void loadScheme(const std::string& schemeFile);
		void setFont(const std::string& fontFile);
		CEGUI::Window* createWidget(const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name = "");
		static void setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4& destRectPix);

		// Getters
		static CEGUI::OgreRenderer* getRenderer() { return m_renderer; }
		const CEGUI::GUIContext* getContext() { return m_context; }
		 CEGUI::Window* getRoot() { return m_root; }
		static CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
		static CEGUI::MouseButton convertButtonFromJoystick(std::string buttonID);
	private:
		static CEGUI::OgreRenderer* m_renderer;
		CEGUI::GUIContext* m_context = nullptr;
		CEGUI::Window* m_root = nullptr;
	};
