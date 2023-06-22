#ifndef SCENESERIALIZER__H
#define SCENESERIALIZER__H

#include "Scene.h"

namespace Flood
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);
	
		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};
}

#endif // !SCENESERIALIZER__H
