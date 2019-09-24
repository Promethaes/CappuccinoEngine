#pragma once

#include <Cappuccino/Transform.h>
#include <string>
namespace Cappuccino {
	class Mesh {
	public:
		Mesh(const std::string& path) { this->_path = path; };

		/*
		Purpose: The filepath provided is what mesh will be loaded
		Req.: path: a constant string pointer to where the mesh is located
		Returns: A boolean representing if the load succeeded or not
		*/
		bool loadMesh(const std::string& path);
		/*
		Purpose: The mesh will be loaded off of the filepath provided in the contructor
		Req.: None
		Returns: A boolean representing if the load succeeded or not
		*/
		bool loadMesh();

		/*
		Purpose: This function will wipe all the data of the object
		Reg.: None
		Returns: None
		*/
		void unload();
		/*
		Purpose: This function will draw the object
		Reg.: None
		Returns: None
		*/
		void draw();
		unsigned VBOverts = 0;
		unsigned VBOuvs = 0;
		unsigned VBOnormals = 0;
		unsigned VAO = 0;

		unsigned getFaces() const { return _numFaces; }
		unsigned getVerts() const { return _numVerts; }

		Transform transform;
	private:
		unsigned _numFaces = 0;
		unsigned _numVerts = 0;
		std::string _path;
	};

}