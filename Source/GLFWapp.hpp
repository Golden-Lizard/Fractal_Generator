#pragma once

#include<glew.h>
#include<glfw3.h>
#include<memory>
#include<exception>
#include<iostream>
#include<functional>

#include "Cursor.h"

bool static fullscreen;
bool static wireframe;

class GLFWapp
{
   public:
	   GLFWapp(std::string const & windName);
	   GLFWapp(const GLFWapp&);
	   GLFWapp(GLFWapp&&);
       
	   ~GLFWapp();

	   GLFWapp & operator=(const GLFWapp&);
	   GLFWapp & operator=(GLFWapp&&);

	   _forceinline 
	   void makeCurrent(void);	   
	   void setViewport(void);
	   bool shouldClose(void);

	   void setKeyCallBack(void(*func)(GLFWwindow *,const int, const int, const int, const int));
	   void setKeyCallBack(void);

	   std::pair<float,float> getSize();

	   friend void KeyCBFunc(GLFWwindow*, int const, int const, int const, int const);

	   GLint SetCursor(const char * path);

	   GLFWwindow * getWindow(void);
	   void UpdateViewport (void);


	   GLuint mainLoop();

  private:
	  GLFWwindow * window;
	  GLFWmonitor* monitor;
	  
	  std::unique_ptr<int> width;
	  std::unique_ptr<int> height;

	  void(*KeyCallBack)(GLFWwindow*, int const, int const, int const, int const);
	  std::function<void(void)> MouseCallback;
	  std::function<void(void)> ScrollCallback;

	  //Cursor cursor;
	  std::string windowName;

	  GLboolean glewExperimental;
	  class Null_window;
	  class Bad_Glew;

};