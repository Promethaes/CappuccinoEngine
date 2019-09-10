#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/IO.h"
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Cappuccino/Camera.h"
#include "..\..\include\Cappuccino\ShaderProgram.h"
namespace Cappuccino {

	Shader::Shader(std::string VERTSHADERSOURCE, std::string FRAGSHADERSOURCE)
	{
		_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		_vertexShaderSource = VERTSHADERSOURCE;
		_fragmentShaderSource = FRAGSHADERSOURCE;


		createProgram();
	}

	void Shader::createShader(const std::string& VERTSHADERSOURCE, std::string FRAGSHADERSOURCE)
	{
		_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		_vertexShaderSource = VERTSHADERSOURCE;
		_fragmentShaderSource = FRAGSHADERSOURCE;


		createProgram();
	}

	void Shader::createProgram()
	{
		//make a fucking string i guess
		std::string shaderSource = " ";


		//fucking string stream needs a string so we pass in a string
		loadFileAsString(_vertexShaderSource, shaderSource);


		//openGL needs a c string so we pretend this c++ string is a fucking c string
		compileShader(_vertexShader, shaderSource.c_str());


		//do it again for the fragment shader
		loadFileAsString(_fragmentShaderSource, shaderSource);
		compileShader(_fragmentShader, shaderSource.c_str());


		//create the shader program
		_shaderProgram = glCreateProgram();


		//attatch and link the shader program to the shaders
		glAttachShader(_shaderProgram, _vertexShader);
		glAttachShader(_shaderProgram, _fragmentShader);
		glLinkProgram(_shaderProgram);


		int success;
		char infoLog[512];

		//set the integer value of the shader program based on its link status, and store it into success
		glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);

		//if we did a dumb...aka if it failed to link
		if (!success) {
			//log the information into a c string and then print it out
			glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
			std::cout << infoLog << "\n";
		}

		//make opengl use the program we made
		glUseProgram(_shaderProgram);


		//delete the memory allocated to the gpu by the sahders so we dont get memory leaks
		glDeleteShader(_vertexShader);
		glDeleteShader(_fragmentShader);

	}

	glm::mat4 Shader::loadModelMatrix(const std::optional<glm::vec3>& translation, const std::optional<float>& scaleBy, const std::optional<glm::vec3>& rotateBy, const std::optional<float>& rotateAngle)
	{
		glm::mat4 model = glm::mat4(1.0f);

		if (translation.has_value())
			model = glm::translate(model, translation.value());
		if (scaleBy.has_value())
			model = glm::scale(model, glm::vec3(scaleBy.value()));
		if (rotateBy.has_value())
			model = glm::rotate(model, rotateAngle.value(), rotateBy.value());

		unsigned int modelLoc = glGetUniformLocation(_shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		return model;
	}

	void Shader::loadViewMatrix(Camera& defaultCamera)
	{
		glm::mat4 view;
		view = defaultCamera.whereAreWeLooking();

		unsigned int viewLoc = glGetUniformLocation(_shaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	}

	void Shader::loadProjectionMatrix(float width, float height)
	{
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

	}

	void Shader::compileShader(unsigned shader, const char* shaderSource)
	{
		//point opengl to the shader's source, then compile that shader
		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);

		int  success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << shader << "failed to compile : " << infoLog << "\n";
		}
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(_shaderProgram, name.c_str()), 1, &value[0]);
	}
	void Shader::setVec4(const std::string& name, const glm::vec3& value) const
	{
		glUniform4fv(glGetUniformLocation(_shaderProgram, name.c_str()), 1, &value[0]);

	}
	void Shader::setVec3(const std::string& name, const float x, const float y, const float z) const {
		glUniform3fv(glGetUniformLocation(_shaderProgram, name.c_str()), 1, &(glm::vec3(x, y, z))[0]);
	}
	void Shader::setFloat(const std::string& name, const float& value) const {
		glUniform1f(glGetUniformLocation(_shaderProgram, name.c_str()), value);
	}
	void Shader::setInt(const std::string& name, const int& value) const {
		glUniform1i(glGetUniformLocation(_shaderProgram, name.c_str()), value);
	}
}
