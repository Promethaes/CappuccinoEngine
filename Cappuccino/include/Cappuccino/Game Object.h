#pragma once
#include "Cappuccino/Mesh.h"
#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/Texture.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Cappuccino/RigidBody.h"
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

		virtual void update(float dt) {};
		virtual void onEnter(float dt, const GameObject& go) {};
		virtual void onExit(float dt, const GameObject& go) {};

	};

	class GameObject {
	public:
		GameObject(const Shader& SHADER, const std::vector<Texture*>& textures, const std::vector<Mesh*>& meshs);

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

		std::string id;


		//these functions might need to be deleted... see f16 file
		void setPosition(const glm::vec3& newPos);

		void scaleX(const float sizeScalar);
		void scaleY(const float sizeScalar);
		void scaleZ(const float sizeScalar);

		void changeState(const State& newState);

		Transform transform;
	protected:
		/*
		Purp: draw the game object
		*/
		State* _state;
		void draw();
		glm::vec3 position;
		std::vector<Texture*> _textures;
		std::vector<Mesh*> _meshes;
		Shader _shader;
		RigidBody _rigidBody;
		//	glm::vec3 _position;
	private:
		/*
		Purp: pure virtual function that calls the appropriate child update definition
		Req: delta time to update properly
		*/
		virtual void childUpdate(float dt) = 0;

		bool _loadedTextures = false;
		bool _loadedMesh = false;
		
		

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