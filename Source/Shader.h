#pragma once

#ifndef _SHADER_
#define _SHADER_

#include <sstream>
#include <vector>
#include <fstream>
#include <memory>
#include <exception>

#include <glew.h>

class Shader
{
public:

	Shader() = default;
	Shader(const wchar_t * vS_Path, const wchar_t * fS_Path);
	Shader(const char    * vS_Path, const char * fS_Path);

	~Shader();
	Shader(const Shader & other);
	Shader(Shader && other);

	Shader & operator=(const Shader & other);
	Shader & operator=(const Shader && other);

	void compile();
	GLuint compileV();
	GLuint compileF();
	GLuint getProgramID();

	void check();
	void Shader::operator()();

	void readStream(std::ifstream & source, std::stringstream & SS);

	std::ifstream readGLSL(wchar_t const *);
	std::ifstream readGLSL(char const *);

private:

	GLuint ProgramID;

	std::string vertex;
	std::string fragment;

	class Shader_Compile_Error;
	class Linking_Failure;
};

#endif