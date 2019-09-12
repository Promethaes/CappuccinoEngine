#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/CappMacros.h"

#include <fstream>
#include <sstream>

using string = std::string;
using ifstream = std::ifstream;
using sstream = std::stringstream;

string Cappuccino::Shader::_shaderDirectory = CAPP_PATH + R"(\Assets\Shaders\)";

Cappuccino::Shader::Shader(const string& vertShaderPath, const string& fragShaderPath, const string& geoShaderPath) {
	_programID = 0;
	GLuint vertShader = 0, fragShader = 0, geoShader = 0;

	CAPP_PRINT_N("----------COMPILING SHADERS----------");

	CAPP_PRINT_N("------VERTEX SHADER------");
	compileShader(vertShaderPath, ShaderType::VERTEX, vertShader);
	CAPP_PRINT_N("-----FRAGMENT SHADER-----");
	compileShader(fragShaderPath, ShaderType::FRAGMENT, fragShader);
	if(!geoShaderPath.empty()) {
		CAPP_PRINT_N("-----GEOMETRY SHADER-----");
		compileShader(geoShaderPath, ShaderType::GEOMETRY, geoShader);
	}

	CAPP_PRINT_N("----------LINKING SHADERS AND CREATING SHADER PROGRAM----------");
	createProgram(vertShader, fragShader, geoShader);

	/*
	#pragma region SHADER SOURCE CODE EXTRACTION

	//CAPP_PRINT("----------READING SHADER SOURCE CODE----------");
	//
	//ifstream vertShaderFile, fragShaderFile;				// Input file stream for both shaders to find the files on disk
	//sstream vertShaderStream, fragShaderStream;				// String stream files to read the data from the files
	//string vertShaderString, fragShaderString;				// Normal strings to convert into C strings later
	//const GLchar* vertShaderSource, * fragShaderSource;		// C strings that will hold the final shader source code

	//// Ensure input file streams can throw exceptions if necessary
	//vertShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	//fragShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	//try {
	//	// Open the files
	//	vertShaderFile.open(_shaderDirectory + vertShaderPath);
	//	fragShaderFile.open(_shaderDirectory + fragShaderPath);

	//	// Read from the files and write to the string streams
	//	vertShaderStream << vertShaderFile.rdbuf();
	//	fragShaderStream << fragShaderFile.rdbuf();

	//	// Convert string streams into strings to obtain source code
	//	vertShaderString = vertShaderStream.str();
	//	fragShaderString = fragShaderStream.str();

	//	// Close the input file stream
	//	vertShaderFile.close();
	//	fragShaderFile.close();

	//	// Convert string into C strings to obtain source code that can be used by OpenGL
	//	vertShaderSource = vertShaderString.c_str();
	//	fragShaderSource = fragShaderString.c_str();
	//}
	//catch (ifstream::failure& error) {
	//	// Output error message to console
	//	CAPP_PRINT("ERROR::SHADER::FILE_READ_UNSUCCESSFUL");
	//	vertShaderSource = "";
	//	fragShaderSource = "";

	//}

	#pragma endregion

	#pragma region SHADER COMPILATION/PROGRAM CREATION

	

	
	//GLint success;																		// Variable to check if any shaders did not compile properly
	//const GLint infoLogSize = 512;														// Size of info log buffer (for error messages)

	///// VERTEX SHADER ///
	//CAPP_PRINT("Creating and compiling vertex shader...");
	//
	//vertShader = glCreateShader(GL_VERTEX_SHADER);										// Create vertex shader object with unique ID
	//glShaderSource(vertShader, 1, &vertShaderSource, NULL);								// Bind the shader code to the shader
	//glCompileShader(vertShader);														// Compile the vertex shader

	//glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);								// Check the compile status of the vertex shader

	//if (!success) {
	//	GLchar infoLog[infoLogSize];													// If the shader did not compile, create a char buffer for the error message                   
	//	glGetShaderInfoLog(vertShader, infoLogSize, NULL, infoLog);						// Copy the error message
	//	CAPP_PRINT_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
	//	CAPP_PRINT_ERROR(infoLog);														// Output error message to the console
	//}
	//CAPP_PRINT("Success!");

	///// FRAGMENT SHADER ///
	//CAPP_PRINT("Creating and compiling fragment shader...");
	//
	//fragShader = glCreateShader(GL_FRAGMENT_SHADER);									// Create fragment shader object with unique ID
	//glShaderSource(fragShader, 1, &fragShaderSource, NULL);								// Bind the shader code to the shader
	//glCompileShader(fragShader);														// Compile the fragment shader

	//glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);								// Check the compile status of the fragment shader
	//if (!success) {
	//	GLchar infoLog[infoLogSize];													// If the shader did not compile, create a char buffer for the error message                   
	//	glGetShaderInfoLog(fragShader, infoLogSize, NULL, infoLog);						// Copy the error message
	//	CAPP_PRINT_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
	//	CAPP_PRINT_ERROR(infoLog);														// Output error message to the console
	//}
	//CAPP_PRINT("Success!");

	///// SHADER PROGRAM CREATION ///
	//CAPP_PRINT("Creating and linking shaders in shader program...");
	//
	//_programID = glCreateProgram();														// Create the shader program
	//glAttachShader(_programID, vertShader);												// Attach the vertex shader
	//glAttachShader(_programID, fragShader);												// Attach the fragment shader
	//glLinkProgram(_programID);															// Link the attached shaders together
	//
	//glGetProgramiv(_programID, GL_LINK_STATUS, &success);								// Check the link status of the program
	//if (!success) {
	//	GLchar infoLog[infoLogSize];													// If the program did not link the shaders, create a char buffer for the error message                   
	//	glGetProgramInfoLog(_programID, infoLogSize, NULL, infoLog);					// Copy the error message
	//	CAPP_PRINT_ERROR("ERROR::PROGRAM::SHADERS::LINK_FAILED\n");
	//	CAPP_PRINT_ERROR(infoLog);														// Output error message to the console
	//}
	//
	//CAPP_PRINT("Success! Deleting shaders...");
	//glDeleteShader(vertShader);															// Delete the vertex shader (to save space, not needed anymore)
	//glDeleteShader(fragShader);															// Delete the fragment shader

	#pragma endregion
	*/
}

void Cappuccino::Shader::use() { glUseProgram(_programID); }

void Cappuccino::Shader::changeShaderDirectory(const string& directory) { _shaderDirectory = directory; }

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

GLuint Cappuccino::Shader::getID() const { return _programID; }

void Cappuccino::Shader::compileShader(const string& shaderPath, const ShaderType& type, GLuint& shader) {
	ifstream shaderFile;
	sstream shaderStream;
	string shaderString;
	const GLchar *shaderSource;

	shaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	CAPP_PRINT("Reading source code...");
	try {
		shaderFile.open(_shaderDirectory + shaderPath);
		shaderStream << shaderFile.rdbuf();
		shaderString = shaderStream.str();
		shaderFile.close();
		
		shaderSource = shaderString.c_str();
		throw 1;
	}
	catch(ifstream::failure& error) {
		CAPP_PRINT_ERROR("Failed to read shader from file!");
		CAPP_PRINT_ERROR("%s", error.what());
		shaderSource = "";
	}
	catch(int success) {
		if(success) {
			CAPP_PRINT_N("Success!");
		}
	}

	CAPP_PRINT("Compiling shader...");
	GLint success;
	GLuint shaderType = NULL;

	switch(type) {
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
	if(!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		
		CAPP_PRINT_ERROR("Failed to compile shader!");
		CAPP_PRINT_ERROR(infoLog);
	}
	CAPP_PRINT_N("Success!");
}

void Cappuccino::Shader::createProgram(const GLuint vertex, const GLuint fragment, const GLuint geometry) {
	_programID = glCreateProgram();						

	CAPP_PRINT_N("Linking shaders...");
	if (vertex)
		glAttachShader(_programID, vertex);				
	else
		CAPP_PRINT_ERROR("No vertex shader linked!");

	if(fragment)
		glAttachShader(_programID, fragment);			
	else
		CAPP_PRINT_ERROR("No fragment shader linked!");
	
	if(geometry)
		glAttachShader(_programID, geometry);			
	else
		CAPP_PRINT_ERROR("No geometry shader linked!");

	CAPP_PRINT("Creating program...");
	GLint success;
	
	glLinkProgram(_programID);							

	glGetProgramiv(_programID, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];							            
		glGetProgramInfoLog(_programID, 512, NULL, infoLog);
		CAPP_PRINT_ERROR("Failed to create the shader program!");
		CAPP_PRINT_ERROR(infoLog);						
	}

	CAPP_PRINT_N("Success! Deleting shaders...");
	glDeleteShader(vertex);								
	glDeleteShader(fragment);							
}

