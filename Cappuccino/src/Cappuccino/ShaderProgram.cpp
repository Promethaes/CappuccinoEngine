#include "Cappuccino/ShaderProgram.h"

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/ResourceManager.h"

#include <fstream>
#include <sstream>
#include <algorithm>

using string = std::string;
using ifstream = std::ifstream;
using sstream = std::stringstream;

using namespace Cappuccino;

string Shader::_shaderDirectory = "./Assets/Shaders/";

Shader::Shader() : _programID(0), _name(""), _vertexShaderPath(""), _fragmentShaderPath(""), _geometryShaderPath("") {}

Shader::Shader(const std::string& name, const string& vertShaderPath, const string& fragShaderPath, const std::optional<string>& geomShaderPath) :
	_programID(0), _name(name), _vertexShaderPath(vertShaderPath), _fragmentShaderPath(fragShaderPath), _geometryShaderPath(geomShaderPath ? geomShaderPath.value() : "") {
	createShader();
}

Shader::Shader(bool cString, const char* vertShader, const char* fragShader, const char* geoShader) {
	_programID = 0;
	_name = std::to_string(rand() % 100);
	GLuint vertShaderu = 0, fragShaderu = 0, geoShaderu = 0;

	//dummy int at the end to specify which compile shader to use
	compileShader(vertShader, ShaderType::VERTEX, vertShaderu, 0);
	compileShader(fragShader, ShaderType::FRAGMENT, fragShaderu, 0);

	std::optional<unsigned> g;
	if (geoShader != nullptr) {
		compileShader(geoShader, ShaderType::GEOMETRY, geoShaderu, 0);
		g = geoShaderu;
	}

	if (g.has_value()) {
		createProgram(vertShaderu, fragShaderu, g.value());
	}
	else {
		createProgram(vertShaderu, fragShaderu);
	}
}

void Shader::createShader() {
	GLuint vertShader = 0, fragShader = 0, geomShader = 0;

	compileShader(_vertexShaderPath.c_str(), ShaderType::VERTEX, vertShader);
	compileShader(_fragmentShaderPath.c_str(), ShaderType::FRAGMENT, fragShader);
	if (!_geometryShaderPath.empty()) {
		compileShader(_geometryShaderPath.c_str(), ShaderType::GEOMETRY, geomShader);
		createProgram(vertShader, fragShader, geomShader);
	}
	else {
		createProgram(vertShader, fragShader);
	}
}

void Shader::use() const {
	glUseProgram(_programID); }

void Shader::setDefaultPath(const string& directory) {
	string dir = directory;
	std::transform(dir.begin(), dir.end(), dir.begin(), tolower);

	if (dir == "default")
		_shaderDirectory = "./Assets/Shaders/";
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

void Shader::compileShader(const char* shaderPath, const ShaderType& type, GLuint& shader) {
	std::string shaderString;

	if (!loadFileAsString(_shaderDirectory + shaderPath, shaderString)) {
		CAPP_ASSERT(shaderString.empty(), "Failed to read shader from file!\n%s", std::string(_shaderDirectory + shaderPath).c_str());
		shaderString = "";
	}

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

	auto shaderFinal = shaderString.c_str();

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderFinal, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		CAPP_ASSERT(success, "Failed to compile shader!\n%s\n\n%s", std::string(_shaderDirectory + shaderPath).c_str(), infoLog);
	}
}

void Shader::compileShader(const char* input, const ShaderType& type, GLuint& shader, int f)
{

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

	auto shaderFinal = input;

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderFinal, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		CAPP_ASSERT(success, "Failed to compile C-string shader!\n%s", infoLog);
	}
}

void Shader::createProgram(const unsigned vertex, const unsigned fragment, const std::optional<unsigned>& geometry) {
	_programID = glCreateProgram();

	if (vertex) {
		glAttachShader(_programID, vertex);
	}
	else {
		CAPP_PRINT_WARNING("No vertex shader linked!");
	}

	if (fragment) {
		glAttachShader(_programID, fragment);
	}
	else {
		CAPP_PRINT_WARNING("No fragment shader linked!");
	}

	if (geometry) {

		if (geometry.value()) {
			glAttachShader(_programID, geometry.value());
		}
		else {
			CAPP_PRINT_WARNING("No geometry shader linked!");
		}

	}

	GLint success;

	glLinkProgram(_programID);

	glGetProgramiv(_programID, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(_programID, 512, NULL, infoLog);
		CAPP_ASSERT(success, "Failed to create the shader program!\n%s", infoLog);
	}

	if (vertex) {
		glDeleteShader(vertex);
	}
	if (fragment) {
		glDeleteShader(fragment);
	}
	if (geometry.has_value()) {
		if (geometry.value()) {
			glDeleteShader(geometry.value());
		}
	}
}

glm::mat4 Shader::loadModelMatrix(const glm::mat4& modelMatrix) {
	setUniform("model", modelMatrix);
	return modelMatrix;
}

glm::mat4 Shader::loadModelMatrix(const std::optional<glm::vec3>& translation, const std::optional<float>& scaleBy, const std::optional<glm::vec3>& rotateBy, const std::optional<float>& rotateAngle) {
	glm::mat4 model = glm::mat4(1.0f);

	if (translation.has_value())
		model = translate(model, translation.value());
	if (scaleBy.has_value())
		model = scale(model, glm::vec3(scaleBy.value()));
	if (rotateBy.has_value())
		model = rotate(model, rotateAngle.value(), rotateBy.value());

	setUniform("model", model);
	return model;
}

void Shader::loadViewMatrix(Camera& defaultCamera) {
	const glm::mat4 view = defaultCamera.whereAreWeLooking();
	setUniform("view", view);
}

void Shader::loadProjectionMatrix(float width, float height) {
	const glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
	setUniform("projection", projection);
}

void Shader::loadOrthoProjectionMatrix(float width, float height)
{
	const glm::mat4 projection = glm::ortho(-width, width, -height, height, -0.1f, 100.0f);
	setUniform("projection", projection);
}


int Shader::getUniformLocation(const std::string& uniformName) const {
	const int location = glGetUniformLocation(_programID, uniformName.c_str());
	if(location == -1) {
		CAPP_PRINT_ERROR("Could not find uniform \"%s\" in shader \"%s\" (ID: %u)", uniformName.c_str(), _name.c_str(), _programID);
	}
	return location;
}

void Shader::setUniform(const std::string& name, const bool value) const                                                      { glUniform1i(getUniformLocation(name), static_cast<GLint>(value)); }
void Shader::setUniform(const std::string& name, const GLint value) const                                                     { glUniform1i(getUniformLocation(name), value); }
void Shader::setUniform(const std::string& name, const GLfloat value) const                                                   { glUniform1f(getUniformLocation(name), value); }

void Shader::setUniform(const std::string& name, const glm::vec2& value) const                                                { glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value)); }
void Shader::setUniform(const std::string& name, const GLfloat x, const GLfloat y) const                                      { glUniform2f(getUniformLocation(name), x, y); }
void Shader::setUniform(const std::string& name, const glm::vec3& value) const                                                { glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value)); }
void Shader::setUniform(const std::string& name, const GLfloat x, const GLfloat y, const GLfloat z) const                     { glUniform3f(getUniformLocation(name), x, y, z); }
void Shader::setUniform(const std::string& name, const glm::vec4& value) const                                                { glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value)); }
void Shader::setUniform(const std::string& name, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w) const    { glUniform4f(getUniformLocation(name), x, y, z, w); }

void Shader::setUniform(const std::string& name, const glm::mat3& value) const                                                { glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)); }
void Shader::setUniform(const std::string& name, const glm::mat4& value) const                                                { glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)); }
