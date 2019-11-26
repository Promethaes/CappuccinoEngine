#pragma once

#include "Cappuccino/Mesh.h"
#include "Cappuccino/RigidBody.h"
#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/Texture.h"
#include "Cappuccino/Transform.h"

#include <string>
#include <vector>
#include <optional>

namespace Cappuccino {

	/*
	Purp: state machine, override virtual functions to get most out of this
	*/
	class GameObject;
	class State {
	public:

		//some example states...
		/*enum States {
			JUMP,
			SHOOT,
			HURT
		};*/

		virtual void update(float dt, GameObject* go) {};
		virtual void onEnter(float dt, GameObject* go) {};
		virtual void onExit(float dt, GameObject* go) {};

	};

	class GameObject {
	public:
		GameObject(const Shader& SHADER, const std::vector<Texture*>& textures, const std::vector<Mesh*>& meshs, const std::optional<float>& mass = std::nullopt);
		virtual ~GameObject();
		/*
		Purp: wrapper class to call child update
		Req: delta time, to update things properly
		*/
		void baseUpdate(float dt);

		void setShader(const Shader& SHADER) {
			_shader = SHADER;
			_shader.createShader();
		}

		static std::vector<GameObject*> gameObjects;

		static Texture* defaultEmission;
		static Texture* defaultNormal;
		static Texture* defaultHeight;

		std::string id;


		//these functions might need to be deleted... see f16 file
		void setPosition(const glm::vec3& newPos);

		void rotateX(const float rotateBy);
		void rotateY(const float rotateBy);
		void rotateZ(const float rotateBy);

		void scaleX(const float sizeScalar);
		void scaleY(const float sizeScalar);
		void scaleZ(const float sizeScalar);


		Transform _transform;
		RigidBody _rigidBody;
		/*
		Purp: To check collision between gameobject
		Pre: another gameobject
		Post: a boolean true if colliding
		*/
		bool checkCollision(GameObject* other);

		/*
		Purp: To check collision between a gameobject and hitbox
		Pre: a hitbox
		Post: a boolean true if colliding
		*/
		bool checkCollision(const HitBox& other, const glm::vec3& pos);
		
		void setActive(bool yn) { _isActive = yn; }
		bool isActive() const { return _isActive; }
		void setVisible(bool yn) { _isVisible = yn; }
		bool isVisible() const { return _isVisible; }
	protected:
		/*
		Purp: draw the game object
		*/
		void draw();

		//a temporary state variable to store a state before the checkChangeState can be called
		bool stateChangeFlag = false;

		std::vector<Texture*> _textures;
		std::vector<Mesh*> _meshes;
		Shader _shader;
		
	private:
		/*
		Purp: to check collision with all gameobjects
		Pre: None
		Post: None
		*/
		void collision();
		/*
		Purp: pure virtual function that calls the appropriate child update definition
		Req: delta time to update properly
		*/
		virtual void childUpdate(float dt) = 0;

		bool _loadedTextures = false;
		bool _loadedMesh = false;
		bool _isActive = false;
		bool _isVisible = true;
		


		/*
		Purp: load the textures, make sure this is only called once
		*/
		void loadTextures();
		/*
		load the meshs
		*/
		void loadMesh();
	};
}
