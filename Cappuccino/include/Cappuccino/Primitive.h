#pragma once
#include "Cappuccino/Mesh.h"
#include "Cappuccino/Transform.h"
#include "Cappuccino/HitBox.h"
#include "Cappuccino/RigidBody.h"
namespace Cappuccino {

	namespace Primitives {

		class Cube : public Mesh {
		public:
			Cube();
			void draw() override;
			Transform _transform;

			RigidBody _body;
		};
		class Sphere : public Mesh {
		public:
			Sphere();
			Transform _transform;
		};
		class Cylinder : public Mesh {
		public:
			Cylinder();
			void draw() override;
			Transform _transform;
		};

	}
}