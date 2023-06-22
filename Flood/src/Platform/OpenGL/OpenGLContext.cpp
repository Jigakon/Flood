#include "flpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Flood
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		FLOOD_ENGINE_ASSERT(windowHandle, "WindowHandle is null!")
	}

	void OpenGLContext::Init()
	{
		FLOOD_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FLOOD_ENGINE_ASSERT(status, "Failed to initialize Glad!")

		FLOOD_ENGINE_INFO("OpenGL Info :\n\t\t\tVendor   : {0}\n\t\t\tRenderer : {1}\n\t\t\tVersion  : {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION))

		FLOOD_ENGINE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Flood requires at least OpenGL version 4.5");
		/* OLD 
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		*/
	}

	void OpenGLContext::SwapBuffers()
	{
		FLOOD_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}