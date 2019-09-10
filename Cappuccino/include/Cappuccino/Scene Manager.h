#pragma once
#include <vector>
/*
The Scene Manager handles a list of all scenes, updating the current one while keeping the others in a sort of "freeze" state
*/
namespace Cappuccino {
	class Camera;
	class Scene {
	public:
		Scene(bool firstScene) = default;
		/*
		Purp: This function will update the active scene
		Reg.: dt: a float of the time between updates, defaultCamera: a pointer to the camera used
		Returns: The function does not return any data
		*/
		void baseUpdate(float dt, Camera& defaultCamera);
		virtual bool init() = 0;
		virtual bool exit() = 0;
		static std::vector<Scene*> scenes;

		bool isActive() const { return active; }
		bool isInit() const { return initialized; }
		bool ShouldExit() const { return shouldExit; }
	protected:
		bool _active = false;
		bool _initialized = false;
		bool _shouldExit = false;
		virtual void childUpdate(float dt, Camera& defaultCamera) = 0;
	};
}