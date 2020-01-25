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

Shader::Shader() : _programID(0), _vertexShaderPath(""), _fragmentShaderPath(""), _geometryShaderPath("") {}

Shader::Shader(const string& vertShaderPath, const string& fragShaderPath, const string& geoShaderPath) {
	_programID = 0;

	_vertexShaderPath = vertShaderPath;
	_fragmentShaderPath = fragShaderPath;
	_geometryShaderPath = geoShaderPath;

	createShader();

	ResourceManager::_allShaders.push_back(this);
}

Cappuccino::Shader::Shader(char* vertShader, char* fragShader, char* geoShader)
{
	_programID = 0;

	GLuint vertShaderu = 0, fragShaderu = 0, geoShaderu = 0;

	//dummy int at the end to specify which compile shader to use
	compileShader(vertShader, ShaderType::VERTEX, vertShaderu,0);
	compileShader(fragShader, ShaderType::FRAGMENT, fragShaderu,0);
	compileShader(geoShader, ShaderType::FRAGMENT, geoShaderu,0);

	createProgram(vertShaderu, fragShaderu, geoShaderu);

	ResourceManager::_allShaders.push_back(this);
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

void Shader::use() const { CAPP_GL_CALL(glUseProgram(_programID)); }

void Shader::setDefaultPath(const string& directory) {
	string dir = directory;
	std::transform(dir.begin(), dir.end(), dir.begin(), ::tolower);

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
		CAPP_PRINT_ERROR("Failed to read shader from file!");
		CAPP_PRINT_ERROR("%s", std::string(_shaderDirectory + shaderPath).c_str());
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

	CAPP_GL_CALL(shader = glCreateShader(shaderType));
	CAPP_GL_CALL(glShaderSource(shader, 1, &shaderFinal, NULL));
	CAPP_GL_CALL(glCompileShader(shader));

	CAPP_GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (!success) {
		GLchar infoLog[512];
		CAPP_GL_CALL(glGetShaderInfoLog(shader, 512, NULL, infoLog));

		CAPP_PRINT_ERROR("Failed to compile shader!");
		CAPP_PRINT_ERROR("%s", std::string(_shaderDirectory + shaderPath).c_str());
		CAPP_PRINT_ERROR("%s", infoLog);
	}
}

void Cappuccino::Shader::compileShader(char* input, const ShaderType& type, GLuint& shader, int f)
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

	CAPP_GL_CALL(shader = glCreateShader(shaderType));
	CAPP_GL_CALL(glShaderSource(shader, 1, &shaderFinal, NULL));
	CAPP_GL_CALL(glCompileShader(shader));

	CAPP_GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (!success) {
		GLchar infoLog[512];
		CAPP_GL_CALL(glGetShaderInfoLog(shader, 512, NULL, infoLog));

		CAPP_PRINT_ERROR("Failed to compile c string shader!");
		CAPP_PRINT_ERROR("%s", infoLog);
	}
}

void Shader::createProgram(const GLuint vertex, const GLuint fragment, std::optional<GLuint> geometry) {
	CAPP_GL_CALL(_programID = glCreateProgram());

	if (vertex) {
		CAPP_GL_CALL(glAttachShader(_programID, vertex));
	}
	else {
		CAPP_PRINT_WARNING("No vertex shader linked!");
	}

	if (fragment) {
		CAPP_GL_CALL(glAttachShader(_programID, fragment));
	}
	else {
		CAPP_PRINT_WARNING("No fragment shader linked!");
	}

	if(geometry.has_value()) {
		
		if (geometry.value()) {
			CAPP_GL_CALL(glAttachShader(_programID, geometry.value()));
		}
		else {
			CAPP_PRINT_WARNING("No geometry shader linked!");
		}
		
	}

	GLint success;

	CAPP_GL_CALL(glLinkProgram(_programID));

	CAPP_GL_CALL(glGetProgramiv(_programID, GL_LINK_STATUS, &success));
	if (!success) {
		GLchar infoLog[512];
		CAPP_GL_CALL(glGetProgramInfoLog(_programID, 512, NULL, infoLog));
		CAPP_PRINT_ERROR("Failed to create the shader program!");
		CAPP_PRINT_ERROR("%s", infoLog);
	}

	if (vertex) {
		CAPP_GL_CALL(glDeleteShader(vertex));
	}
	if (fragment) {
		CAPP_GL_CALL(glDeleteShader(fragment));
	}
	if(geometry.has_value()) {
		if (geometry.value()) {
			CAPP_GL_CALL(glDeleteShader(geometry.value()));
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
		model = glm::translate(model, translation.value());
	if (scaleBy.has_value())
		model = glm::scale(model, glm::vec3(scaleBy.value()));
	if (rotateBy.has_value())
		model = glm::rotate(model, rotateAngle.value(), rotateBy.value());

	setUniform("model", model);
	return model;
}

void Shader::loadViewMatrix(Camera& defaultCamera) {
	const glm::mat4 view = defaultCamera.whereAreWeLooking();
	setUniform("view", view);
}

void Shader::loadProjectionMatrix(float width, float height) {
	const glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	setUniform("projection", projection);
}

void Shader::loadOrthoProjectionMatrix(float width, float height)
{
	const glm::mat4 projection = glm::ortho(-width, width, -height, height, -0.1f, 100.0f);
	setUniform("projection", projection);
}

void Shader::setUniform(const std::string& name, const bool value) const {
	CAPP_GL_CALL(GLint location = glGetUniformLocation(_programID, name.c_str()));
		
	if(location != -1) {
		CAPP_GL_CALL(glUniform1i(location, static_cast<GLint>(value)));
	}
	else {
		CAPP_PRINT_ERROR("Could not find uniform %s in shader #%u", name.c_str(), _programID);
	}
}

void Shader::setUniform(const std::string& name, const GLint value) const {
	CAPP_GL_CALL(GLint location = glGetUniformLocation(_programID, name.c_str()));
		
	if(location != -1) {
		CAPP_GL_CALL(glUniform1i(location, value));
	}
	else {
		CAPP_PRINT_ERROR("Could not find uniform %s in shader #%u", name.c_str(), _programID);
	}
}

void Shader::setUniform(const std::string& name, const GLfloat value) const {
	CAPP_GL_CALL(GLint location = glGetUniformLocation(_programID, name.c_str()));

	if(location != -1) {
		CAPP_GL_CALL(glUniform1f(location, value));
	}
	else {
		CAPP_PRINT_ERROR("Could not find uniform %s in shader #%u", name.c_str(), _programID);
	}
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) const {
	CAPP_GL_CALL(GLint location = glGetUniformLocation(_programID, name.c_str()));

	if(location != -1) {
		CAPP_GL_CALL(glUniform3fv(location, 1, &value[0]));
	}
	else {
		CAPP_PRINT_ERROR("Could not find uniform %s in shader #%u", name.c_str(), _programID);
	}
}

void Shader::setUniform(const std::string& name, const GLfloat x, const GLfloat y, const GLfloat z) const {
	CAPP_GL_CALL(GLint location = glGetUniformLocation(_programID, name.c_str()));

	if(location != -1) {
		CAPP_GL_CALL(glUniform3fv(location, 1, &glm::vec3(x, y, z)[0]));
	}
	else {
		CAPP_PRINT_ERROR("Could not find uniform %s in shader #%u", name.c_str(), _programID);
	}
}

void Shader::setUniform(const std::string& name, const glm::vec4& value) const {
	CAPP_GL_CALL(GLint location = glGetUniformLocation(_programID, name.c_str()));

	if(location != -1) {
		CAPP_GL_CALL(glUniform4fv(location, 1, &value[0]));
	}
	else {
		CAPP_PRINT_ERROR("Could not find uniform %s in shader #%u", name.c_str(), _programID);
	}
}

void Shader::setUniform(const std::string& name, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w) const {
	CAPP_GL_CALL(GLint location = glGetUniformLocation(_programID, name.c_str()));

	if(location != -1) {
		CAPP_GL_CALL(glUniform4fv(location, 1, &glm::vec4(x, y, z, w)[0]));
	}
	else {
		CAPP_PRINT_ERROR("Could not find uniform %s in shader #%u", name.c_str(), _programID);
	}
}

void Shader::setUniform(const std::string& name, const glm::mat4& value) {
	CAPP_GL_CALL(GLint location = glGetUniformLocation(_programID, name.c_str()));

	if(location != -1) {
		CAPP_GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]));
	}
	else {
		CAPP_PRINT_ERROR("Could not find uniform %s in shader #%u", name.c_str(), _programID);
	}
}