#pragma once
#include "Cappuccino/Game Object.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/CappInput.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/UI.h"

namespace Cappuccino {
	class Player : public GameObject {
	public:
		Player(const Shader& SHADER, std::vector<Texture*>& textures, const std::vector<Mesh*>& meshes);
		~Player();

		void childUpdate(float dt) override;

		const CappInput _input;

		Camera* getCamera() const { return _playerCamera; }


#if CROSSHAIRTEST
		Shader _crosshairShader{ "screenSpace.vert","screenSpace.frag" };
		Mesh* _testMesh;
		Transform _testMeshTransform;
#endif

#if UITEST
		UserInterface _playerUI;
		Shader _uiShader{ "font.vert","font.frag" };
		float _uiFloat = 0;
#endif

	protected:
		Camera* _playerCamera = new Camera();
	};

	//leaving this in for discussion purposes
	namespace PlayerStates {
		class DefaultState : public State {
		public:
			void update(float dt, GameObject* go) override;
		};
		class MovementState : public State {
		public:
			void update(float dt, GameObject* go) override;
		};
	}
}