#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <string>

namespace Cappuccino {
	enum class ShaderType : GLuint {
		VERTEX = 0,
		FRAGMENT,
		GEOMETRY
	};

/* Desc.: Searches, compiles, and creates shaders and links them into a program that is usable by OpenGL. */
	class Shader {
	public:

		Shader() = default;

/*
 * Purp.: Creates the shader program using shader files specified by the constructor parameters
 * Req.:
 *    vertShaderPath: The path to the vertex shader
 *    fragShaderPath: the file path to the fragment shader
 *    geoShaderPath: the file path to the geometry shader (empty by default)
 */
		Shader(const std::string& vertShaderPath, const std::string& fragShaderPath, const std::string& geoShaderPath = "");

/*
 * Purp.: Activates the shader for use
 * Req.: No parameters
 * Returns: void
 */
		void use() const;

/*
 * Purp.: Changes the directory in which the shader program looks for the shader source files
 * Req.:
 *     directory: the directory in which to look for, use "default" to look in default directory ( %CappuccinoPath%\Assets\Shaders\ )
 * Returns: Nothing
 */
		void changeShaderDirectory(const std::string& directory) const;

/*
 * Purp.: Looks for and sets uniform variables in a specific shader
 * Req.:
 *     name: the name of the shader where the variable is
 *     value: the value to set the uniform variable to
 * Returns: void
 */
		void setUniform(const std::string& name, bool value)    const;

/*
 * Purp.: Looks for and sets uniform variables in a specific shader
 * Req.:
 *     name: the name of the shader where the variable is
 *     value: the value to set the uniform variable to
 * Returns: void
 */
		void setUniform(const std::string& name, GLint value)   const;

/*
 * Purp.: Looks for and sets uniform variables in a specific shader
 * Req.:
 *     name: the name of the shader where the variable is
 *     value: the value to set the uniform variable to
 * Returns: void
 */
		void setUniform(const std::string& name, GLfloat value) const;
		
		//void setUniform(std::string& name, glm::vec3 value) const;
		//void setUniform(std::string& name, glm::vec4 value) const;
		//void setUniform(std::string& name, const GLfloat value1, const GLfloat value2, const GLfloat value3) const;
		//void setUniform(std::string& name, const GLfloat value1, const GLfloat value2, const GLfloat value3, const GLfloat value4 = NULL) const;

/*
 * Purp.: Returns the shader program ID as defined by OpenGL
 * Req.: No parameters
 * Returns: the shader's programID (GLuint)
 */
		GLuint getID() const;
		
	private:

		// Function to load files as strings (used for shader source code)
		static bool loadFileAsString(const std::string& file, std::string& output);
		// Function to compile shaders in the constructor
		static void compileShader(const std::string& shaderPath, const ShaderType& type, GLuint& shader);
		// Function to link shaders together after compilation
		void createProgram(const GLuint vertex, const GLuint fragment, const GLuint geometry);
		
		GLuint _programID;
		static std::string _shaderDirectory;
		
	};
	
}

#endif
