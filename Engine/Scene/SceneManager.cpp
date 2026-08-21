#include "MonsiPch.h"
#include "SceneManager.h"
#include "Entity.h"
#include "Components.h"
#include "MeshBuilder.h" //temporary
#include <yaml-cpp/yaml.h>

namespace YAML {
	Emitter& operator<<(Emitter& out, const glm::vec3& v) {
		out << Flow << BeginSeq << v.x << v.y << v.z << EndSeq;
		return out;
	}
	Emitter& operator<<(Emitter& out, const glm::vec4& v) {
		out << Flow << BeginSeq << v.x << v.y << v.z << v.w << EndSeq;
		return out;
	}
	Emitter& operator<<(Emitter& out, const glm::quat& q) {
		out << Flow << BeginSeq << q.w << q.x << q.y << q.z << EndSeq;
		return out;
	}

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::quat> {
		static Node encode(const glm::quat& rhs) {
			Node node;
			node.push_back(rhs.w);
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::quat& rhs) {
			if (!node.IsSequence() || node.size() != 4) return false;

			rhs.w = node[0].as<float>();
			rhs.x = node[1].as<float>();
			rhs.y = node[2].as<float>();
			rhs.z = node[3].as<float>();
			return true;
		}
	};
}

namespace Monsi {

	static void SerializeEntity(YAML::Emitter& out, Entity entity) {
		out << YAML::BeginMap;
		out << YAML::Key << "Entity_ID" << YAML::Value << "1337420"; // TODO: replace with a UUID per entity

		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>();
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << tag.Tag;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>()) {
			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>()) {
			auto& src = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << src.Color;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<DirectionalLightComponent>()) {
			auto& dlc = entity.GetComponent<DirectionalLightComponent>();
			out << YAML::Key << "DirectionalLightComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << dlc.Color;
			out << YAML::Key << "Direction" << YAML::Value << dlc.Direction;
			out << YAML::Key << "Intensity" << YAML::Value << dlc.Intensity;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<PointLightComponent>()) {
			auto& plc = entity.GetComponent<PointLightComponent>();
			out << YAML::Key << "PointLightComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << plc.Color;
			out << YAML::Key << "Intensity" << YAML::Value << plc.Intensity;
			out << YAML::Key << "Radius" << YAML::Value << plc.Radius;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>()) {
			auto& cc = entity.GetComponent<CameraComponent>();
			auto& cam = cc.Camera;

			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

			out << YAML::Key << "Camera";
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)cam.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << cam.GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << cam.GetPerspectiveNear();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << cam.GetPerspectiveFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << cam.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << cam.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << cam.GetOrthographicFarClip();
			out << YAML::EndMap;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshComponent>()) {
			auto& mc = entity.GetComponent<MeshComponent>();
			out << YAML::Key << "MeshComponent";
			out << YAML::BeginMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<ModelComponent>()) {
			auto& mc = entity.GetComponent<ModelComponent>();
			out << YAML::Key << "ModelComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "AssetPath" << YAML::Value << mc.ModelAsset->GetFilePath();
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SkyBoxComponent>()) {
			auto& sbc = entity.GetComponent<SkyBoxComponent>();
			auto& filePath = sbc.SkyBox->GetFilePath();
			out << YAML::Key << "SkyBoxComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "AssetPath";
			out << YAML::BeginSeq;
			for (int i = 0; i < 6; i++) {
				out  << YAML::Value << filePath[i];
			}
			out << YAML::EndSeq;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	SceneManager::SceneManager(const Reference<Scene>& Scene) : m_Scene(Scene)
	{
	}

	void SceneManager::SaveScene(const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene";
		out << YAML::Value << "Name";
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;

		for (auto entityHandle : m_Scene->m_Registry.view<entt::entity>()) {
			Entity entity{ entityHandle, m_Scene.get() };
			if (!entity) continue;

			SerializeEntity(out, entity);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	bool SceneManager::LoadScene(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();
		YAML::Node data = YAML::Load(strStream.str());
		
		if (!data["Scene"]) return false;

		std::string sceneName = data["Scene"].as<std::string>();
		ENGINE_LOG_TRACE("Loading scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities) {
			for (auto entityNode : entities) {
				uint64_t uuid = entityNode["Entity_ID"].as<uint64_t>();
				
				std::string name;
				auto tagComponentNode = entityNode["TagComponent"];
				if (tagComponentNode) {
					name = tagComponentNode["Tag"].as<std::string>();
				}

				ENGINE_LOG_TRACE("Loaded entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				auto transformComponentNode = entityNode["TransformComponent"];
				if (transformComponentNode) {
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponentNode["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponentNode["Rotation"].as<glm::quat>();
					tc.Scale = transformComponentNode["Scale"].as<glm::vec3>();
				}

				auto spriteRendererComponentNode = entityNode["SpriteRendererComponent"];
				if (spriteRendererComponentNode) {
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponentNode["Color"].as<glm::vec4>();
				}

				auto directionalLightComponentNode = entityNode["DirectionalLightComponent"];
				if (directionalLightComponentNode) {
					auto& dlc = deserializedEntity.AddComponent<DirectionalLightComponent>();
					dlc.Color = directionalLightComponentNode["Color"].as<glm::vec3>();
					dlc.Direction = directionalLightComponentNode["Direction"].as<glm::vec3>();
					dlc.Intensity = directionalLightComponentNode["Intensity"].as<float>();
				}

				auto pointLightComponentNode = entityNode["PointLightComponent"];
				if (pointLightComponentNode) {
					auto& plc = deserializedEntity.AddComponent<PointLightComponent>();
					plc.Color = pointLightComponentNode["Color"].as<glm::vec3>();
					plc.Intensity = pointLightComponentNode["Intensity"].as<float>();
					plc.Radius = pointLightComponentNode["Radius"].as<float>();
				}

				auto cameraComponentNode = entityNode["CameraComponent"];
				if (cameraComponentNode) {
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					auto cameraNode = cameraComponentNode["Camera"];

					cc.Primary = cameraComponentNode["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponentNode["FixedAspectRatio"].as<bool>();

					if (cameraNode) {
						using ProjectionType = decltype(cc.Camera.GetProjectionType());
						cc.Camera.SetProjectionType((ProjectionType)cameraNode["ProjectionType"].as<int>());
						cc.Camera.SetPerspectiveFOV(cameraNode["PerspectiveFOV"].as<float>());
						cc.Camera.SetPerspectiveNear(cameraNode["PerspectiveNear"].as<float>());
						cc.Camera.SetPerspectiveFar(cameraNode["PerspectiveFar"].as<float>());
						cc.Camera.SetOrthographicSize(cameraNode["OrthographicSize"].as<float>());
						cc.Camera.SetOrthographicNearClip(cameraNode["OrthographicNear"].as<float>());
						cc.Camera.SetOrthographicFarClip(cameraNode["OrthographicFar"].as<float>());
					}
				}

				auto meshComponentNode = entityNode["MeshComponent"];
				if (meshComponentNode) {
					Reference<Material> test = CreateReference<Material>();
					test->AmbientColor = glm::vec3(0.3f, 0.3f, 0.3f);
					test->DiffuseColor = glm::vec3(0.3f, 0.3f, 0.3f);
					test->SpecularColor = glm::vec3(0.3f, 0.3f, 0.3f);
					test->Shininess = 33.3f;
					deserializedEntity.AddComponent<MeshComponent>(MeshBuilder::CreateCube(0.5f, test));
				}

				auto modelComponentNode = entityNode["ModelComponent"];
				if (modelComponentNode) {
					std::string assetPath = modelComponentNode["AssetPath"].as<std::string>();
					Reference<Monsi::Model> test = CreateReference<Monsi::Model>(assetPath);
					auto& mc = deserializedEntity.AddComponent<ModelComponent>(test);
				}

				auto skyBoxComponentNode = entityNode["SkyBoxComponent"];
				if (skyBoxComponentNode) {
					auto assetPathsNode = skyBoxComponentNode["AssetPath"];
					if (assetPathsNode && assetPathsNode.IsSequence()) {
						std::array<std::string, 6> assetPaths;
						for (size_t i = 0; i < 6 && i < assetPathsNode.size(); i++) {
							assetPaths[i] = assetPathsNode[i].as<std::string>();
						}

						auto skyBoxPass = CreateReference<Monsi::SkyBoxPass>(assetPaths);
						skyBoxPass->Init();

						auto skyBoxTexture = Monsi::CubeMapTexture::Create(assetPaths);

						deserializedEntity.AddComponent<Monsi::SkyBoxComponent>(skyBoxPass, skyBoxTexture);
					}
				}
			}
		}

		return true;
	}

}