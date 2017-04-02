
#define GLFW_DLL
#include <glew.h>
#include <glfw3.h>

#include "3rd Party/glm/glm/glm.hpp"
#include "3rd Party/glm/glm/gtc/matrix_transform.hpp"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include<memory>
#include<exception>
#include<chrono>
#include<random>

#define STB_IMAGE_IMPLEMENTATION
#include"3rd Party/stb_image.h"

#include "Shader.h"
#include "GLFWapp.hpp"

#define WIN_NULL L"NULLPTR WINDOW"
#define EXIT_ERROR -1

class Null_window :public std::exception
{
	public:
		const wchar_t * error() const throw()
		{
		    return WIN_NULL;
		};
};

class Bad_Glew :public std::exception
{
     public:
		 const wchar_t * error() const throw()
		 {
			 return L"GLEW ERROR";
	     }
};

template<typename char_type, typename io_stream>
GLuint inline setStreamBuffer(io_stream & stream, std::vector<char_type> buffer)
{
	stream.rdbuf()->pubsetbuf(buffer.data(),buffer.size());

	return 0;
}

#pragma region("Fractal controls")

GLfloat zoom_ctrl = 5.5f;
GLfloat pos_x = -0.5f;
GLfloat pos_y = -0.5f;

#pragma endregion

int wmain(int argc, wchar_t ** argv)
{
	std::ios::sync_with_stdio(false);

	std::vector<wchar_t> wclog_buff;

	wclog_buff.reserve(0xFFFF);
	setStreamBuffer(std::wclog,wclog_buff);

	std::vector<wchar_t> wcerr_buff;

	wcerr_buff.reserve(0xFFFF);
	setStreamBuffer(std::wcerr, wcerr_buff);

	std::vector<char> cerr_buff;

	cerr_buff.reserve(0xFFFF);
	setStreamBuffer(std::cerr, cerr_buff);

	std::vector<char> cout_buff;

	cout_buff.reserve(0xFFFF);
	setStreamBuffer(std::cout, cout_buff);

	std::vector<wchar_t> wcout_buff;

	cout_buff.reserve(0xFFFF);
	setStreamBuffer(std::wcout, wcout_buff);

	std::vector<char> clog_buff;

	clog_buff.reserve(0xFFFF);
	setStreamBuffer(std::clog, clog_buff);

	std::string name = "windoeze";
	GLFWapp viewer(name);
	viewer.setKeyCallBack();

	Shader myShader(L"VertexShader_01.glsl", L"FragmentShader_01.glsl");
	myShader.check();

	glEnable(GL_CULL_FACE);

	auto resolution = viewer.getSize();

	GLfloat ndcVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	
	GLuint screenVAO;
	GLuint screenVBO;
	glGenVertexArrays(1, &screenVAO);
	glGenBuffers(1, &screenVBO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ndcVertices), &ndcVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GL_FLOAT),(GLvoid*)0);
	glEnableVertexAttribArray(1);
 	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GL_FLOAT),(GLvoid*) (2*sizeof(GLfloat)));
	glBindVertexArray(0);

	int x, y, channels;
	BYTE * image = stbi_load("C:\\Users\\Dorin\\Desktop\\stuff\\gradient.png",
		            &x, &y, &channels, 0x4);

	GLuint gradient;
	glGenTextures(1, &gradient);
	glBindTexture(GL_TEXTURE_1D, gradient);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage1D(GL_TEXTURE_1D,0, GL_RGBA, 0x100,0,GL_RGBA, GL_UNSIGNED_BYTE,image);
	glEnable(GL_TEXTURE_1D);

	glGenerateMipmap(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D, 0);

	glm::vec3 rnd_col;

	std::random_device rnd;

	/*glUniform1f(glGetUniformLocation(myShader.getProgramID(),
			"gradient"), 0);*/
	void(*scrollFunc)(GLFWwindow*, double, double) = &scrollCBack;
	glfwSetScrollCallback(viewer.getWindow(), scrollFunc);

	//glUniform3f(rands, 1.0f / (rnd() % 100), 1.0f / (rnd() % 100), 1.0f / (rnd() % 100));

	GLuint rands(glGetUniformLocation(myShader.getProgramID(), "rands"));
	GLuint scale(glGetUniformLocation(myShader.getProgramID(), "scale"));
	GLuint center(glGetUniformLocation(myShader.getProgramID(), "center"));
	GLuint size(glGetUniformLocation(myShader.getProgramID(), "size"));
	GLuint relative_pos(glGetUniformLocation(myShader.getProgramID(), "position"));
	GLuint zoom(glGetUniformLocation(myShader.getProgramID(), "zoom"));

	while (!glfwWindowShouldClose(viewer.getWindow()))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader();
		glUniform1f(scale, resolution.second / resolution.first);
		glUniform2f(center, 0.7f, 0.7f);
		glUniform2f(size, resolution.first, resolution.second);
		glUniform2f(relative_pos, pos_x, pos_y);
		glUniform1f(zoom, zoom_ctrl);

		glUniform3f(rands,float(1/rnd()%100), float(1/ rnd() % 100), float(1/ rnd() % 100));

		glBindVertexArray(screenVAO);
		glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
		glBindTexture(GL_TEXTURE_1D, gradient);
		glDrawArrays(GL_TRIANGLES, 0, 24);

		glBindVertexArray(0);

		std::this_thread::sleep_for(std::chrono::milliseconds(30));

		glfwSwapBuffers(viewer.getWindow());
		glfwPollEvents();
	}


	glfwTerminate();
	return EXIT_SUCCESS;
}
