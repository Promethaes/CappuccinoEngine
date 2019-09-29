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
		_state = new PlayerStates::DefaultState();
	}

	Player::~Player()
	{
		delete _playerCamera;
	}

	void Player::childUpdate(float dt)
	{
		if (_input.keyboard->keyPressed(Events::W))
			setPosition(glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * 2.5f * dt);
		if (_input.keyboard->keyPressed(Events::S))
			setPosition(-glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * 2.5f * dt);

		if (_input.keyboard->keyPressed(Events::A))
			setPosition(-_playerCamera->getRight() * 2.5f * dt);
		if (_input.keyboard->keyPressed(Events::D))
			setPosition(_playerCamera->getRight() * 2.5f * dt);

		_playerCamera->setPosition(position);
	}


}

