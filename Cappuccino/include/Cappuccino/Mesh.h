#pragma once

#include <Cappuccino/Transform.h>
#include <string>
namespace Cappuccino {
	class Mesh {
	public:
		Mesh(const std::string& path);

		
		/*
		Purpose: The mesh will be loaded off of the filepath provided in the contructor
		Req.: None
		Returns: A boolean representing if the load succeeded or not
		*/
		bool loadMesh();

		/*
		Purpose: This function will wipe all the data of the object
		*/
		void unload();
		/*
		Purpose: This function will draw the object
		*/
		virtual void draw();
		unsigned _VAO = 0;
		unsigned _VBO = 0;

		unsigned getFaces() const { return _numFaces; }
		unsigned getVerts() const { return _numVerts; }

		bool loaded = false;
	protected:
		unsigned _numFaces = 0;
		unsigned _numVerts = 0;
		std::string _path;
	};
}