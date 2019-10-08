#pragma once
#include "Cappuccino/Mesh.h"
#include "Cappuccino/Transform.h"
namespace Cappuccino {

	namespace Primitives {

		class Cube : public Mesh {
		public:
			Cube();
			void draw() override;
			Transform _transform;
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