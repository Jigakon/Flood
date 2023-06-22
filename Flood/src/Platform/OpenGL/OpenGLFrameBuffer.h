#ifndef OPENGLFRAMEBUFFER__H
#define OPENGLFRAMEBUFFER__H

#include "Flood/Renderer/FrameBuffer.h"

namespace Flood
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specs);
		virtual ~OpenGLFrameBuffer();
	
		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { FLOOD_ENGINE_ASSERT(index < m_ColorAttachments.size(), "index exceeded color attachment size"); return m_ColorAttachments[index]; }

		virtual const FrameBufferSpecification& GetSpecifications() const override { return m_Specification; };
	private:
		uint32_t m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecs;
		FrameBufferTextureSpecification m_DepthAttachmentSpec;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}

#endif // !OPENGLFRAMEBUFFER__H
