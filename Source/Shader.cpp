#include "Shader.h"
#include <iostream>

class Shader::Shader_Compile_Error : public std::exception
{
public:
	const wchar_t * error() throw()
	{
		return L"Failed to compile shader";
	}
};

class Shader::Linking_Failure : public std::exception
{
public:
	const wchar_t * error() throw()
	{
		return L"Failed to link shader program";
	}
};

Shader::Shader(const wchar_t * vS_Path, const wchar_t * fS_Path)
{
	std::ifstream shader_V = this->readGLSL(vS_Path);
	std::ifstream shader_F = this->readGLSL(fS_Path);

	std::stringstream _VertexShader;
	std::stringstream _FragmentShader;

	readStream(shader_V, _VertexShader);
	readStream(shader_F, _FragmentShader);

	vertex = _VertexShader.str();
	fragment = _FragmentShader.str();

	this->compile();
}

Shader::Shader(const char * vS_Path, const char * fS_Path)
{
	std::ifstream shader_V = this->readGLSL(vS_Path);
	std::ifstream shader_F = this->readGLSL(fS_Path);

	std::stringstream _VertexShader;
	std::stringstream _FragmentShader;

	readStream(shader_V, _VertexShader);
	readStream(shader_F, _FragmentShader);

	vertex = _VertexShader.str();
	fragment = _FragmentShader.str();

	this->compile();

	shader_V.close();
	shader_F.close();
}

Shader::~Shader()
{
	vertex.clear();
	fragment.clear();
}

Shader::Shader(const Shader & other)
{
	this->vertex = other.vertex;
	this->fragment = other.fragment;

	this->compile();
}

Shader::Shader(Shader && other)
{
	this->vertex = std::move(other.vertex);
	this->fragment = std::move(other.fragment);

	this->compile();
}

Shader & Shader::operator=(const Shader & other)
{
	this->vertex = other.vertex;
	this->fragment = other.fragment;

	this->compile();

	return *this;
}

Shader & Shader::operator=(const Shader && other)
{
	this->vertex = std::move(other.vertex);
	this->fragment = std::move(other.fragment);

	this->compile();

	return *this;
}

void Shader::check()
{
	
	std::clog.write(vertex.c_str(), vertex.size());
	std::clog << '\n';
    std::clog.write(fragment.c_str(), fragment.size());
}

void Shader::operator()()
{
	glUseProgram(ProgramID);
}

void Shader::compile()
{
	GLuint vertexShader = compileV();
	GLuint fragmentShader = compileF();

	this->ProgramID = glCreateProgram();
	if (!vertexShader || !fragmentShader)
	{
		std::wcerr << L"Error compiling shaders";
		return;
	}

	glAttachShader(this->ProgramID, vertexShader);
	glAttachShader(this->ProgramID, fragmentShader);

	glLinkProgram(this->ProgramID);

	try
	{
		GLint check = 0;
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &check);

		if (!check)
			throw Linking_Failure();
	}
	catch (...)
	{
		GLchar * log = new GLchar[0xffff]{ "" };

		glGetProgramInfoLog(ProgramID, 0xffff, NULL, log);
		std::cerr << log;


		delete[]log;
		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

GLuint Shader::compileV()
{
	GLuint vertexShader;
	GLint check;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//const GLchar * src = vertex.c_str();
	GLchar * src = new GLchar[fragment.length()];

	GLuint iter = 0;

	for (auto & letter : vertex)
	{
		src[iter] = letter;
		++iter;
	}

	glShaderSource(vertexShader, 1, &src, NULL);
	glCompileShader(vertexShader);

	try
	{
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &check);

		if (!check)
			throw Shader_Compile_Error();

		return vertexShader;
	}
	catch (std::exception const & e)
	{
		GLchar * log = new GLchar[0xffff]{ "" };

		glGetProgramInfoLog(ProgramID, 0xffff, NULL, log);
		std::cerr << log;


		delete[]log;
		return -1;
	}

	return -1;
}

GLuint Shader::compileF()
{
	GLint check;
	GLuint fragmentShader;

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//const GLchar * src = fragment.c_str();

	GLchar * src = new GLchar[fragment.length()];

	GLuint iter = 0;

	for (auto & letter : fragment)
	{
		src[iter] = letter;
		++iter;
	}

	glShaderSource(fragmentShader, 1, &src, NULL);
	glCompileShader(fragmentShader);

	try
	{
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &check);
		std::clog << check << '\n';

		if (check == 0)
			throw Shader_Compile_Error();

		return fragmentShader;
	}
	catch (std::exception const & e)
	{
		GLchar * log = new GLchar[0xffff]{ "" };

		glGetProgramInfoLog(ProgramID, 0xffff, NULL, log);
		std::cerr << log;


		delete[]log;
		return -1;
	}

	return -1;
}

GLuint Shader::getProgramID()
{
	return GLuint(ProgramID);
}

__forceinline void Shader::readStream(std::ifstream & source, std::stringstream & SS)
{
	SS << source.rdbuf();
}

__forceinline std::ifstream Shader::readGLSL(wchar_t const * path)
{
	std::ifstream _glslSource;

	try
	{
		_glslSource.open(path, std::ios::in | std::ios::beg);
	}
	catch (std::ifstream::failure e)
	{
		std::wcerr << "Error opening file";
	}

	return _glslSource;
}

std::ifstream Shader::readGLSL(char const *)
{
	return std::ifstream();
}
