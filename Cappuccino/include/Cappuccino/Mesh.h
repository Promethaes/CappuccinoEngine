#pragma once

#include <string>
#include <vector>

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

		void reload(const std::vector<float>& VERTS,const std::vector<float>& NORMS,const std::vector<float>& TANGS);

		/*
		Purpose: This function will wipe all the data of the object
		*/
		void unload();

		/*
		Purpose: This function will draw the object
		*/
		virtual void draw();

		/*
		 * Purp.: Changes the directory in which the engine looks for the meshes
		 * Req.:
		 *     directory: the directory in which to look for, use "default" to look in default directory ( %CappuccinoPath%\Assets\Meshes\ )
		 * Returns: Nothing
		 */
		static void setDefaultPath(const std::string& directory);

		
		unsigned _VAO = 0;
		unsigned _VBO = 0;

		unsigned getFaces() const { return _numFaces; }
		unsigned getVerts() const { return _numVerts; }

		bool loaded = false;
		std::vector<float> master;
		std::vector<float> verts;
		std::vector<float> texts;
		std::vector<float> norms;
		std::vector<float> tangs;

	protected:

		static std::string _meshDirectory;
		
		unsigned _numFaces = 0;
		unsigned _numVerts = 0;
		std::string _path;

	};
}