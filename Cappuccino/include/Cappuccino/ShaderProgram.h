#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <optional>
#include <string>

namespace Cappuccino {
	class Camera;

	enum class ShaderType : GLuint {
		VERTEX = 0,
		FRAGMENT,
		GEOMETRY
	};

	/* Desc.: Searches, compiles, and creates shaders and links them into a program that is usable by OpenGL. */
	class Shader {
	public:

		Shader();

		/*
		 * Purp.: Creates the shader program using shader files specified by the constructor parameters
		 * Req.:
		 *    vertShaderPath: The path to the vertex shader
		 *    fragShaderPath: the file path to the fragment shader
		 *    geomShaderPath: the file path to the geometry shader (empty by default)
		 */
		Shader(const std::string& name, const std::string& vertShaderPath, const std::string& fragShaderPath, const std::optional<std::string>& geomShaderPath = std::nullopt);

		/*
		Make a shader with raw strings instead of from a file
		cString bool is a dummy param so c++ doesn't fuck me over
		*/
		explicit Shader(bool cString, const char* vertShader, const char* fragShader, const char* geoShader = nullptr);

		/*
		 * Purp.: Activates the shader for use
		 * Req.: No parameters
		 * Returns: void
		 */
		void use() const;

		void createShader();

		/*
		 * Purp.: Changes the directory in which the shader program looks for the shader source files
		 * Req.:
		 *     directory: the directory in which to look for, use "default" to look in default directory ( %CappuccinoPath%\Assets\Shaders\ )
		 * Returns: Nothing
		 */
		static void setDefaultPath(const std::string& directory);

		/*
		 * Purp.: Looks for and sets uniform variables in a specific shader
		 * Req.:
		 *     name: the name of the shader where the variable is
		 *     value: the value to set the uniform variable to
		 * Returns: void
		 */
		void setUniform(const std::string& name, bool value) const;
		void setUniform(const std::string& name, GLint value) const;
		void setUniform(const std::string& name, GLfloat value) const;
		void setUniform(const std::string& name, const glm::vec2& value) const;
		void setUniform(const std::string& name, GLfloat x, GLfloat y) const;
		void setUniform(const std::string& name, const glm::vec3& value) const;
		void setUniform(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const;
		void setUniform(const std::string& name, const glm::vec4& value) const;
		void setUniform(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
		void setUniform(const std::string& name, const glm::mat3& value) const;
		void setUniform(const std::string& name, const glm::mat4& value) const;

		GLuint getID() const;
		glm::mat4 loadModelMatrix(const glm::mat4& modelMatrix);
		/**
		 * @deprecated Don't try to do the transformations from this function
		 */
		glm::mat4 loadModelMatrix(const std::optional<glm::vec3>& translation, const std::optional<float>& scaleBy, const std::optional<glm::vec3>& rotateBy, const std::optional<float>& rotateAngle);
		void loadViewMatrix(Camera& defaultCamera);
		void loadProjectionMatrix(float width, float height);
		void loadOrthoProjectionMatrix(float width, float height);

		const std::string& getName() const { return _name; }

	private:

		// Function to load files as strings (used for shader source code)
		static bool loadFileAsString(const std::string& file, std::string& output);

		// Function to compile shaders in the constructor
		static void compileShader(const char* shaderPath, const ShaderType& type, GLuint& shader);

		//compile shader input c string, not from file. f is a dummy param
		static void compileShader(const char* input, const ShaderType& type, GLuint& shader, int f);

		// Function to link shaders together after compilation
		void createProgram(unsigned vertex, unsigned fragment, const std::optional<unsigned>& geometry = std::nullopt);

		// Checker for uniform locations
		int getUniformLocation(const std::string& uniformName) const;

		GLuint _programID;
		static std::string _shaderDirectory;

		std::string _name;
		std::string _vertexShaderPath;
		std::string _fragmentShaderPath;
		std::string _geometryShaderPath;

		static unsigned _shaderName;
	};
}