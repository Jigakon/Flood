#ifndef RENDERER2D__H
#define RENDERER2D__H

#include "OrthographicCamera.h"
#include "Flood/Renderer/Camera.h"
#include "Flood/Renderer/EditorCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "Flood/ECS/Components/SpriteRenderer.h"

namespace Flood
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera); // remove
		static void EndScene();
		static void Flush();

		// Draw Quads
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>&, const glm::vec4& color, float tilingFactor = 1.0f);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>&, const glm::vec4& color, float tilingFactor = 1.0f);


		// Draw Rotated Quads
		// the rotation angle should be in degrees
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color = glm::vec4(1.0f));
		// the rotation angle should be in degrees
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color = glm::vec4(1.0f));
		// the rotation angle should be in degrees
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		// the rotation angle should be in degrees
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		// the rotation angle should be in degrees
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		// the rotation angle should be in degrees
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, int entityID, float tilingFactor = 1.0f);

		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

	// Only on Development 
	public:
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static Statistics GetStats();
		static void ResetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};
}

#endif // !RENDERER2D__H
