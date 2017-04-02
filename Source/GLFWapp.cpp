#include "GLFWapp.hpp"
#include "Controls.h"

class GLFWapp::Null_window :public std::exception
{
  public:
	const wchar_t * error() const throw()
	{
		return L"\nCannot create window!";
	};
};


class GLFWapp::Bad_Glew :public std::exception
{
public:
	const wchar_t * error() const throw()
	{
		return L"\nCannot initialize glew!";
	};
};

GLFWapp::GLFWapp(std::string const & windName) :
	monitor(glfwGetPrimaryMonitor()),
	windowName(windName),
	width (new int(0)),
    height (new int(0))
{
	auto video = glfwGetVideoMode(monitor);
	/*
	*height=(video->height);
	*width = (video->width);
	*/
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);

	try
	 {
		window = glfwCreateWindow(GL_MAX_WIDTH, GL_MAX_HEIGHT, 
			                      windowName.c_str(), monitor, 
			                      nullptr);
			      
		if (window == nullptr)
			throw Null_window();
	} 
	catch (...)
	{
		std::wcerr << L"ERROR, failed to initialize window!";
		glfwTerminate();
		return;
	}

	this->makeCurrent();

	try
	{
		glewExperimental = GL_TRUE;

		if (glewInit() != GLEW_OK)
			throw Bad_Glew();
	}
	catch (...)
	{
		std::wcerr << L"ERROR";
		return;
	}

	this->setViewport();
}

GLFWapp::GLFWapp(const GLFWapp & other)
{
}

GLFWapp::GLFWapp(GLFWapp && other)
{
}

GLFWapp::~GLFWapp()
{
	glfwTerminate();
}

GLFWapp & GLFWapp::operator=(const GLFWapp &)
{
	return *this;
}

GLFWapp & GLFWapp::operator=(GLFWapp &&)
{
	return *this;
}

void GLFWapp::setViewport(void)
{
	glfwGetFramebufferSize(window,
		                   width.get(),
		                   height.get());

	glViewport(0,0,*width,*height); 
}

bool GLFWapp::shouldClose(void)
{
	return bool(glfwWindowShouldClose(window));
}

void GLFWapp::setKeyCallBack(void(*func)(GLFWwindow *,const int, const int, const int, const int))
{
	glfwSetKeyCallback
	(
		this->window,
		GLFWkeyfun(func)
	);
}

void GLFWapp::setKeyCallBack(void)
{
	KeyCallBack = &KeyCBFunc;

	glfwSetKeyCallback
	(
		this->window,
		GLFWkeyfun(KeyCallBack)
	);

}

std::pair<float, float> GLFWapp::getSize()
{
	return std::make_pair(*width, *height);
}

void KeyCBFunc(GLFWwindow * win, int const key, int const scancode, int const action, int const mode)
{
	extern GLfloat pos_x;
	extern GLfloat pos_y;
	extern GLfloat zoom_ctrl;

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_DOWN)
		{
			pos_y == 0.1f? pos_y==0.1f : pos_y-= 0.1f;
			getPosition().y -= 0.05f;
		};

		if (key == GLFW_KEY_UP)
		{
			getPosition().y += 0.05f;
			pos_y == 8.0f? pos_y = 8.0f : pos_y += 0.1f;
		}

		if (key == GLFW_KEY_LEFT)
		{
			pos_x == 0.1f ? pos_x == 0.1f : pos_x -= 0.1f;
			getPosition().x -= 0.05f;
		}
		if (key == GLFW_KEY_RIGHT)
		{
			pos_x == 8.0f ? pos_x = 8.0f : pos_x += 0.1f;
			getPosition().x += 0.05f;
		}
		switch (key)
		{
		case GLFW_KEY_ESCAPE:

			glfwSetWindowShouldClose(win, GL_TRUE);
			break;

		case GLFW_KEY_COMMA:
			getPosition() = glm::vec3(0.0f, 0.0f, 5.0f);
			break;

		case GLFW_KEY_F7:
		{
			pos_x = pos_y = -0.5f;
			zoom_ctrl = 4.5f;
		}break;

		case GLFW_KEY_F11:

			switch (fullscreen)
			{
				case false:
				{
					glfwSetWindowMonitor(win, glfwGetPrimaryMonitor(), NULL, NULL, GL_MAX_WIDTH, GL_MAX_HEIGHT, 30);
					fullscreen = !fullscreen;
				}
				break;

				case true:
				{
					glfwSetWindowMonitor(win, nullptr, 100, 100, 400, 400, 30);
					fullscreen = !fullscreen;
				}
				break;

			}
			break;

		case GLFW_KEY_F10:

			switch (wireframe)
			{
			case true:
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				wireframe = !wireframe;
			}
			break;

			case false:
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				wireframe = !wireframe;
			}
			break;
			}

		default: break;
		}
	}
}

GLint GLFWapp::SetCursor(const char* path)
{
	Cursor curs(path);
	curs.SetCursor(this->window);
	
	return EXIT_SUCCESS;
}

GLFWwindow * GLFWapp::getWindow(void)
{
	return this->window;
}

void GLFWapp::UpdateViewport(void)
{
}

GLuint GLFWapp::mainLoop()
{
	return 0;
}

_forceinline void GLFWapp::makeCurrent(void)
{
	glfwMakeContextCurrent(window);
}
