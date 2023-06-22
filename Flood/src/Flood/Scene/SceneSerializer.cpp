#include "flpch.h"
#include "SceneSerializer.h"

#include <yaml-cpp/yaml.h>

#include "Flood/ECS/Core/ECSManager.h"

#include "Flood/ECS/Components/Components.h"
#include "Flood/ECS/Components/CameraComponent.h"
#include "Flood/ECS/Components/CameraControllerComponent.h"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Flood
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow; // -> [1, 2, 3]
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_Scene(scene){}

	static void SerializeEntity(YAML::Emitter& out, ECS::Entity entity, ECS::ECSManager& manager)
	{
		out << YAML::BeginMap;
			out << YAML::Key << "Entity" << YAML::Value << "1234567890123456"; // Entity ID

			if (manager.HasComponent<TagComponent>(entity))
			{
				out << YAML::Key << "TagComponent";
				out << YAML::BeginMap;
					auto& tag = manager.GetComponent<TagComponent>(entity).Name;
					out << YAML::Key << "Tag" << YAML::Value << tag;
				out << YAML::EndMap;
			}

			if (manager.HasComponent<TransformComponent>(entity))
			{
				out << YAML::Key << "TransformComponent";
				out << YAML::BeginMap;

					auto& tc = manager.GetComponent<TransformComponent>(entity);
					out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
					out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
					out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

				out << YAML::EndMap;
			}

			if (manager.HasComponent<CameraComponent>(entity))
			{
				out << YAML::Key << "CameraComponent";
				out << YAML::BeginMap;

				auto& cc = manager.GetComponent<CameraComponent>(entity);
				auto& cam = cc.Camera;

				out << YAML::Key << "Camera" << YAML::Value;
				out << YAML::BeginMap;

				out << YAML::Key << "ProjectionType" << YAML::Value << (int)cam.GetProjectionType();
				out << YAML::Key << "PerspectiveFOV" << YAML::Value << cam.GetPerspectiveVerticalFOV();
				out << YAML::Key << "PerspectiveNear" << YAML::Value << cam.GetPerspectiveNearClip();
				out << YAML::Key << "PerspectiveFar" << YAML::Value << cam.GetPerspectiveFarClip();
				out << YAML::Key << "OrthographicSize" << YAML::Value << cam.GetOrthographicSize();
				out << YAML::Key << "OrthographicNear" << YAML::Value << cam.GetPerspectiveNearClip();
				out << YAML::Key << "OrthographicFar" << YAML::Value << cam.GetOrthographicFarClip();

				out << YAML::EndMap;
				
				out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
				out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

				out << YAML::EndMap;
			}

			if (manager.HasComponent<SpriteRendererComponent>(entity))
			{
				out << YAML::Key << "SpriteRendererComponent";
				out << YAML::BeginMap;
				auto& src = manager.GetComponent<SpriteRendererComponent>(entity);
				out << YAML::Key << "Color" << YAML::Value << src.Color;
				std::string path = "";
				if (src.Texture!=nullptr)
					path = src.Texture->GetPath();
				if (path.empty()) 
					path = "";
				out << YAML::Key << "TexturePath" << YAML::Value << path;
				out << YAML::Key << "TilingFactor" << YAML::Value << src.TilingFactor;
				out << YAML::EndMap;
			}

			if (manager.HasComponent<CameraControllerComponent>(entity))
			{
				out << YAML::Key << "CameraControllerComponent";
				out << YAML::BeginMap;
				auto& ccc = manager.GetComponent<CameraControllerComponent>(entity);
				out << YAML::Key << "Speed" << YAML::Value << ccc.speed;
				out << YAML::EndMap;
			}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Name";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		auto entities = m_Scene->m_ECSManager.GetEntities();
		for (auto& entity : entities)
		{
			SerializeEntity(out, entity, m_Scene->m_ECSManager);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		FLOOD_ENGINE_ASSERT(false, "Serialization Runtime Not Implemented");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data;
		try
		{
			data = YAML::Load(strStream.str());
		} 
		catch (YAML::ParserException ECS)
		{
			return false;
		}
		if (!data["Scene"])
			return false;
		std::string sceneName = data["Scene"].as<std::string>();
		FLOOD_ENGINE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				auto deserializedEntity = m_Scene->CreateEntity();
				
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
				{
					auto name = tagComponent["Tag"].as<std::string>();
					m_Scene->m_ECSManager.AddComponent<TagComponent>(deserializedEntity, { name.c_str() });
					FLOOD_ENGINE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);
				}

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = m_Scene->m_ECSManager.AddComponent<TransformComponent>(deserializedEntity, {});
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = m_Scene->m_ECSManager.AddComponent<CameraComponent>(deserializedEntity, {});
					auto cam = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cam["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cam["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cam["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cam["PerspectiveFar"].as<float>());
					
					cc.Camera.SetOrthographicSize(cam["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cam["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cam["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}
			
				auto cameraControllerComponent = entity["CameraControllerComponent"];
				if (cameraControllerComponent)
				{
					auto& ccc = m_Scene->m_ECSManager.AddComponent<CameraControllerComponent>(deserializedEntity, {});
					ccc.speed = cameraControllerComponent["Speed"].as<float>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = m_Scene->m_ECSManager.AddComponent<SpriteRendererComponent>(deserializedEntity, {});
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					auto path = spriteRendererComponent["TexturePath"].as<std::string>();
					if (!path.empty())
						src.Texture = Texture2D::Create(path);
					src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		FLOOD_ENGINE_ASSERT(false, "Deserialization Runtime Not Implemented");
		return false;
	}

}
