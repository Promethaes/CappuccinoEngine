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

		virtual void update(float dt, GameObject* go) {};
		virtual void onEnter(float dt, GameObject* go) {};
		virtual void onExit(float dt, GameObject* go) {};

	};

	class GameObject {
	public:
		GameObject(const Shader& SHADER, const std::vector<Texture*>& textures, const std::vector<Mesh*>& meshs);
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

		void setStateChange(const State& newState);
	protected:
		/*
		Purp: draw the game object
		*/
		void draw();

		State* _state;
		//a temporary state variable to store a state before the checkChangeState can be called
		State* _tempState;
		bool stateChangeFlag = false;
		glm::vec3 position;

		std::vector<Texture*> _textures;
		std::vector<Mesh*> _meshes;
		Shader _shader;

		//gameplay stuff
		float hp;
		float speed = 3.5f;
		
	private:
		void checkChangeState(float dt, const State& newState);

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