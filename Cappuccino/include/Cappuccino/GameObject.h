#pragma once

#include "Cappuccino/Mesh.h"
#include "Cappuccino/RigidBody.h"
#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/Texture.h"
#include "Cappuccino/Transform.h"
#include "Cappuccino/AnimationSystem.h"

#include <string>
#include <vector>
#include <optional>

namespace Cappuccino {

	class GameObject {
	public:
		GameObject(const Shader& SHADER, const std::vector<Texture*>& textures, const std::vector<Mesh*>& meshs, const std::optional<float>& mass = std::nullopt, unsigned viewportNum = 0);
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

		std::string id;

		Transform _transform;
		RigidBody _rigidBody;
		/*
		Purp: To check collision between GameObject
		Pre: another GameObject
		Post: a boolean true if colliding
		*/
		bool checkCollision(GameObject* other);
		/*
		Purp: To check if you will collide with another GameObject
		Pre: another GameObject
		Post: a boolean true if they will collide
		*/
		bool willCollide(GameObject* other, const glm::vec3& direction, float dt);

		/*
		Purp: To check collision between a GameObject and hitbox
		Pre: a hitbox
		Post: a boolean true if colliding
		*/
		bool checkCollision(const HitBox& other, const glm::vec3& pos);
		/*
		Purp: To check if you will collide with another hitbox
		Pre: a hitbox
		Post: a boolean true if they will collide
		*/
		bool willCollide(const HitBox& other, const glm::vec3& direction, const glm::vec3& pos, float dt);
		/*
		Purp: Check if a ray is intersecting with cube hitboxes
		Req: A ray
		returns: whether or not it is intersecting
		*/
		bool intersecting(const Ray& ray);
		/*
		Purp: to see what gameobject the ray hits first
		Pre: a ray
		Post: what GameObject it hits first
		*/
		GameObject* getFirstIntersect(const Ray& ray);

		/*
		Purp: to see what gameobject the ray hits first
		Pre: a ray, a vector of ids to ignore or only look for, a bool(true for blacklist, false for whitelist)
		Post: what GameObject it hits first
		*/
		GameObject* getFirstIntersect(const Ray& ray, const std::vector<std::string>& ids, bool blackList);

		void setActive(const bool yn) { _isActive = yn; }
		bool isActive() const { return _isActive; }
		void setVisible(const bool yn) { _isVisible = yn; }
		bool isVisible() const { return _isVisible; }
		void setPaused(bool yn) { _isPaused = yn; }
		bool isPaused() const { return _isPaused; }
		void setClose(bool yn) { _shouldClose = yn; }
		bool getClose() const { return _shouldClose; }
		void draw();

		//draw function used for the gBuffer
		void gBufferDraw(Shader* gBufferShader);

		//returns which viewport the object should be drawn in
		unsigned getViewportNum() { return _viewportNum; }
		Animator _animator;
	protected:

		unsigned _viewportNum;

		std::vector<Texture*> _textures;
		std::vector<Mesh*> _meshes;
		Shader _shader;

	private:
		/*
		Purp: to check collision with all gameobjects
		Pre: None
		Post: None
		*/
		void collision(float dt);
		/*
		Purp: pure virtual function that calls the appropriate child update definition
		Req: delta time to update properly
		*/
		virtual void childUpdate(float dt) = 0;

		bool _loadedTextures = false;
		bool _loadedMesh = false;
		bool _isActive = false;
		bool _isVisible = true;
		bool _isPaused = false;
		bool _shouldClose = false;



		/*
		Purp: load the textures, make sure this is only called once
		*/
		void loadTextures();
		/*
		load the meshes
		*/
		void loadMesh();
	};
}
