#ifndef GRAPHICSCONTEXT__H
#define GRAPHICSCONTEXT__H

namespace Flood
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Scope<GraphicsContext> Create (void* window);
	};

}

#endif //!GRAPHICSCONTEXT
