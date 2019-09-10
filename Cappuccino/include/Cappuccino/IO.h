#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

///<this code is brought to you by the courtesy of Emilian>
//loads the file as a string
static bool loadFileAsString(const std::string & file, std::string& output)
{
	//create the ifstream object
	std::ifstream inStream(file.data());

	//if the stream contains badbits, dont do anything
	if (!inStream.good()) {
		std::cout << "Failed to load a file as a string: badbits are at it again!\n";
		return false;
	}


	//sets the output to the file's string

	//creating out stringstream object
	std::ostringstream contents;
	//sends the stream buffer object's contents to the out stream
	contents << inStream.rdbuf();

	//setting value of passed in string to the contents of the out stream
	output = contents.str();

	

	return true;

}