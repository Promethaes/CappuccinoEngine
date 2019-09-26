#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Camera.h"
#include <fstream>
#include <sstream>
#include <algorithm>
using string = std::string;
using ifstream = std::ifstream;
using sstream = std::stringstream;
namespace Cappuccino {
	string Shader::_shaderDirectory = CAPP_PATH + R"(\Assets\Shaders\)";

	Shader::Shader() : _programID(0), _vertexShaderPath(""), _fragmentShaderPath(""), _geometryShaderPath("") {}

	Shader::Shader(const string& vertShaderPath, const string& fragShaderPath, const string& geoShaderPath) {
		_programID = 0;

		_vertexShaderPath = vertShaderPath;
		_fragmentShaderPath = fragShaderPath;
		_geometryShaderPath = geoShaderPath;

		createShader();
	}

	void Shader::createShader() {
		_programID = 0;
		GLuint vertShader = 0, fragShader = 0, geoShader = 0;

		CAPP_PRINT_N("----------COMPILING SHADERS----------");

		CAPP_PRINT_N("------VERTEX SHADER------");
		compileShader(_vertexShaderPath, ShaderType::VERTEX, vertShader);
		CAPP_PRINT_N("\n-----FRAGMENT SHADER-----");
		compileShader(_fragmentShaderPath, ShaderType::FRAGMENT, fragShader);
		if (!_geometryShaderPath.empty()) {
			CAPP_PRINT_N("\n-----GEOMETRY SHADER-----");
			compileShader(_geometryShaderPath, ShaderType::GEOMETRY, geoShader);
		}

		CAPP_PRINT_N("\n----------LINKING SHADERS AND CREATING SHADER PROGRAM----------");
		createProgram(vertShader, fragShader, geoShader);
	}

	void Shader::use() const { glUseProgram(_programID); }

	void Shader::changeShaderDirectory(const string& directory) const {
		string dir = directory;
		std::transform(dir.begin(), dir.end(), dir.begin(), ::tolower);

		if (dir == "default")
			_shaderDirectory = CAPP_PATH + R"(\Assets\Shaders\)";
		else
			_shaderDirectory = directory;
	}

	GLuint Shader::getID() const { return _programID; }

	// This function is brought to us by courtesy of Emilian.cpp
	bool Shader::loadFileAsString(const std::string& file, std::string& output) {
		const ifstream inStream(file.data());
		sstream fileContent;

		if (!inStream.good()) {
			return false;
		}

		fileContent << inStream.rdbuf();
		output = fileContent.str();

		return true;
	}

	void Shader::compileShader(const string& shaderPath, const ShaderType& type, GLuint& shader) {
		string shaderString;
		const GLchar* shaderSource;

		CAPP_PRINT_N("Reading source code...");
		if (!loadFileAsString(_shaderDirectory + shaderPath, shaderString)) {
			CAPP_PRINT_ERROR("Failed to read shader from file!");
			shaderString = "";
		}

		shaderSource = shaderString.c_str();

		CAPP_PRINT_N("Compiling shader...");
		GLint success;
		GLuint shaderType = NULL;

		switch (type) {
		case ShaderType::VERTEX:
			shaderType = GL_VERTEX_SHADER;
			break;

		case ShaderType::FRAGMENT:
			shaderType = GL_FRAGMENT_SHADER;
			break;

		case ShaderType::GEOMETRY:
			shaderType = GL_GEOMETRY_SHADER;
			break;
		}

		shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);

			CAPP_PRINT_ERROR("Failed to compile shader!");
			CAPP_PRINT_ERROR(infoLog);
		}
	}

	void Shader::createProgram(const GLuint vertex, const GLuint fragment, const GLuint geometry) {
		_programID = glCreateProgram();

		CAPP_PRINT_N("Linking shaders...");
		if (vertex) {
			glAttachShader(_programID, vertex);
		}
		else {
			CAPP_PRINT_ERROR("No vertex shader linked!");
		}

		if (fragment) {
			glAttachShader(_programID, fragment);
		}
		else {
			CAPP_PRINT_ERROR("No fragment shader linked!");
		}

		if (geometry) {
			glAttachShader(_programID, geometry);
		}
		else {
			CAPP_PRINT_ERROR("No geometry shader linked!");
		}

		CAPP_PRINT_N("Creating program...");
		GLint success;

		glLinkProgram(_programID);

		glGetProgramiv(_programID, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infoLog[512];
			glGetProgramInfoLog(_programID, 512, NULL, infoLog);
			CAPP_PRINT_ERROR("Failed to create the shader program!");
			CAPP_PRINT_ERROR(infoLog);
		}

		CAPP_PRINT_N("Deleting shaders...");
		if (vertex) {
			glDeleteShader(vertex);
		}
		if (fragment) {
			glDeleteShader(fragment);
		}
		if (geometry) {
			glDeleteShader(geometry);
		}
	}
	glm::mat4 Shader::loadModelMatrix(const glm::mat4& modelMatrix)
	{
		unsigned int modelLoc = glGetUniformLocation(_programID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		return modelMatrix;
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

		unsigned int modelLoc = glGetUniformLocation(_programID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		return model;
	}

	void Shader::loadViewMatrix(Camera& defaultCamera)
	{
		glm::mat4 view;
		view = defaultCamera.whereAreWeLooking();

		unsigned int viewLoc = glGetUniformLocation(_programID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	}

	void Shader::loadProjectionMatrix(float width, float height)
	{
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(_programID, "projection"), 1, GL_FALSE, &projection[0][0]);
	}

	void Cappuccino::Shader::setUniform(const std::string& name, const bool value) const {
		glUniform1i(
			glGetUniformLocation(_programID, name.c_str()), static_cast<GLint>(value)
		);
	}
	void Cappuccino::Shader::setUniform(const std::string& name, const GLint value) const {
		glUniform1i(
			glGetUniformLocation(_programID, name.c_str()), value
		);
	}
	void Cappuccino::Shader::setUniform(const std::string& name, const GLfloat value) const {
		glUniform1f(
			glGetUniformLocation(_programID, name.c_str()), value
		);
	}
	void Shader::setUniform(const std::string& name, const float x, const float y, const float z) const {
		glUniform3fv(glGetUniformLocation(_programID, name.c_str()), 1, &(glm::vec3(x, y, z))[0]);
	}
	void Shader::setUniform(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(_programID, name.c_str()), 1, &value[0]);
	}
	void Shader::setUniform(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(_programID, name.c_str()), 1, &value[0]);
	}
}