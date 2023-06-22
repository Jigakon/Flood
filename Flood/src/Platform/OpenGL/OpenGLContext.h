#ifndef OPENGLCONTEXT__H
#define OPENGLCONTEXT__H

#include "Flood/Renderer/GraphicsContext.h"

struct GLFWwindow;
namespace Flood
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;

	};
}

#endif // !defineOPENGLCONTEXT__H
