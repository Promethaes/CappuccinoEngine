#include "Cappuccino/Player.h"
#define player static_cast<Cappuccino::Player*>

namespace Cappuccino {
	void PlayerStates::DefaultState::update(float dt, GameObject* go)
	{
		//TODO
		//anyone looking at this, help me find a better way to do this...too many layers goin on here
		///if (player(go)->input.keyboard->keyPressed(Events::W) || player(go)->input.keyboard->keyPressed(Events::A)
		///	|| player(go)->input.keyboard->keyPressed(Events::S) || player(go)->input.keyboard->keyPressed(Events::D))
		///	player(go)->setStateChange(MovementState());
	}
	void PlayerStates::MovementState::update(float dt, GameObject* go)
	{  ///
		///if (player(go)->_input.keyboard->keyPressed(Events::W))
		///	go->setPosition(go->transform._transformMat[0] * 2.5f * dt);
		///if (player(go)->_input.keyboard->keyPressed(Events::S))
		///	go->setPosition(-go->transform._transformMat[0] * 2.5f * dt);
	   ///
		///if (player(go)->_input.keyboard->keyPressed(Events::A))
		///	go->setPosition(-player(go)->getCamera().getRight() * 2.5f * dt);
		///if (player(go)->_input.keyboard->keyPressed(Events::D))
		///	go->setPosition(player(go)->getCamera().getRight() * 2.5f * dt);
	   ///
	}
	Player::Player(const Shader& SHADER, std::vector<Texture*>& textures, const std::vector<Mesh*>& meshes)
		:_input(true, std::nullopt), GameObject(SHADER, textures, meshes)
	{

#if CROSSHAIRTEST
		_testMesh = new Mesh(CAPP_PATH + "Assets/Mesh/Cube2.obj");
		_testMesh->loadMesh();
#endif

#if UITEST
		_playerUI._uiComponents.push_back(new UIText("UI", _uiShader, glm::vec2(-1500.0f, 1000.0f), glm::vec3(1.0f, 0.0f, 1.0f), 1.0f));
		_playerUI._uiComponents.push_back(new UIBar(glm::vec2(10.0f,10.0f), (glm::vec3(0.0f, 0.0f, 1.0f))));
#endif


		_state = new PlayerStates::DefaultState();
	}

	Player::~Player()
	{
		delete _playerCamera;
	}

	void Player::childUpdate(float dt)
	{
		if (_input.keyboard->keyPressed(Events::Shift))
			speed = 7.0f;
		else
			speed = 3.5f;

		if (_input.keyboard->keyPressed(Events::W))
			setPosition(glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * speed * dt);
		if (_input.keyboard->keyPressed(Events::S))
			setPosition(-glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * speed * dt);

		if (_input.keyboard->keyPressed(Events::A))
			setPosition(-_playerCamera->getRight() * speed * dt);
		if (_input.keyboard->keyPressed(Events::D))
			setPosition(_playerCamera->getRight() * speed * dt);

		_playerCamera->setPosition(position);

#if CROSSHAIRTEST
		_crosshairShader.use();
		_crosshairShader.loadOrthoProjectionMatrix(800.0f / 10, 600.0f / 10);
		_crosshairShader.loadModelMatrix(glm::mat4(1.0f));
		_testMesh->draw();
#endif

#if UITEST
		_uiFloat += dt;
		_playerUI.update(dt);

#endif


	}
}