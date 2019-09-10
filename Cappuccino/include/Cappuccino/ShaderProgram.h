#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <optional>
#include <string>
namespace Cappuccino {
	class Camera;
	class Shader {
	public:

		Shader() = default;
		Shader(std::string VERTSHADERSOURCE, std::string FRAGSHADERSOURCE);

		//only call this function if you can't call the constructor for some reason
		void createShader(const std::string& VERTSHADERSOURCE, std::string FRAGSHADERSOURCE);
		//get the program 
		unsigned getId() const { return _shaderProgram; }
		void setVec3(const std::string&name, const float x, const float y, const float z) const;
		void setVec3(const std::string &name, const glm::vec3 &value) const;
		void setVec4(const std::string& name, const glm::vec3& value) const;
		void setFloat(const std::string &name, const float& value) const;
		void setInt(const std::string& name, const int& value)const;

		void createProgram();
		//not sure if i should be making these functions but we're gonna try it anyways

		glm::mat4 loadModelMatrix(const std::optional<glm::vec3>& translation, const std::optional<float>& scaleBy, const std::optional<glm::vec3>& rotateBy, const std::optional<float>& rotateAngle);
		void loadViewMatrix(Camera& defaultCamera);
		void loadProjectionMatrix(float width, float height);
	private:
		//compiles the shader that you pass in
		void compileShader(unsigned shader, const char* tempString);


		unsigned _shaderProgram;
		unsigned _vertexShader;
		std::string _vertexShaderSource;
		std::string _fragmentShaderSource;
		unsigned _fragmentShader;
	};
}
