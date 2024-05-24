#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Shader
{
private:
	//the program ID
	unsigned int ID;
public:
	Shader(const char* vertexPath, const char* fragmentPath);

	//useProgram
	void use();

	unsigned int getID();

	// utility uniform setters
	void setBool(const string& name, bool value) const;
	void setInt(const string& name, int value) const;
	void setFloat(const string& name, float value) const;
};

