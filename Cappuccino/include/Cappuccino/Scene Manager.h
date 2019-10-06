#pragma once
#include <vector>
#include <string>
#include "Cappuccino/Camera.h"
/*
The Scene Manager handles a list of all scenes, updating the current one while keeping the others in a sort of "freeze" state
*/
namespace Cappuccino {
	class Camera;
	class Scene {
	public:
		Scene(bool firstScene);
		/*
		Purp: This function will update the active scene
		Reg.: dt: a float of the time between updates, defaultCamera: a pointer to the camera used
		Returns: The function does not return any data
		*/
		void baseUpdate(float dt);
		virtual bool init() = 0;
		virtual bool exit() = 0;
		static std::vector<Scene*> scenes;
		static Camera* defaultCamera;

		void sendString(const std::string& info);

		bool isActive() const { return _active; }
		bool isInit() const { return _initialized; }
		bool ShouldExit() const { return _shouldExit; }

		/*
		Purp: determine the mouse callback for the scene
		it is suggested that some control booleans are put in place
		eg. isPaused, if so then make the mouse visible and so on
		*/
		virtual void mouseFunction(double xpos, double ypos) = 0;

		void setActive(bool yn) { _active = yn; }
	protected:
		std::string info;
		bool _active = false;
		bool _initialized = false;
		bool _shouldExit = false;
		virtual void childUpdate(float dt) = 0;
	};

	class SceneManager {
		friend Scene;
	public:

		/*
		Purp: get the user to add a scene in a nice looking way
		Req: a pointer to a scene
		*/
		static void addScene(Scene* scene) {}

		static void updateScenes(float dt);

		static void changeScene(unsigned changeToIndex);

	private:
		SceneManager();
	};
}
