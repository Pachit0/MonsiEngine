#include "MonsiPch.h"
#include "SceneManager.h"
#include "Entity.h"
#include "Components.h"
#include "MeshBuilder.h"
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

	template<>
	struct convert<Monsi::SphereParams> {
		static Node encode(const Monsi::SphereParams& rhs) {
			Node node;
			node["Radius"] = rhs.radius;
			node["Rings"] = rhs.rings;
			node["Sectors"] = rhs.sectors;
			return node;
		}
		static bool decode(const Node& node, Monsi::SphereParams& rhs) {
			if (!node.IsMap()) return false;
			rhs.radius = node["Radius"].as<float>();
			rhs.rings = node["Rings"].as<uint32_t>();
			rhs.sectors = node["Sectors"].as<uint32_t>();
			return true;
		}
	};

	template<>
	struct convert<Monsi::GridParams> {
		static Node encode(const Monsi::GridParams& rhs) {
			Node node;
			node["Width"] = rhs.width;
			node["Depth"] = rhs.depth;
			node["Columns"] = rhs.columns;
			node["Rows"] = rhs.rows;
			return node;
		}
		static bool decode(const Node& node, Monsi::GridParams& rhs) {
			if (!node.IsMap()) return false;
			rhs.width = node["Width"].as<float>();
			rhs.depth = node["Depth"].as<float>();
			rhs.columns = node["Columns"].as<uint32_t>();
			rhs.rows = node["Rows"].as<uint32_t>();
			return true;
		}
	};

	template<>
	struct convert<Monsi::CubeParams> {
		static Node encode(const Monsi::CubeParams& rhs) {
			Node node;
			node["Size"] = rhs.size;
			return node;
		}
		static bool decode(const Node& node, Monsi::CubeParams& rhs) {
			if (!node.IsMap()) return false;
			rhs.size = node["Size"].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Monsi::CylinderParams> {
		static Node encode(const Monsi::CylinderParams& rhs) {
			Node node;
			node["Radius"] = rhs.radius;
			node["Height"] = rhs.height;
			node["Sectors"] = rhs.sectors;
			return node;
		}
		static bool decode(const Node& node, Monsi::CylinderParams& rhs) {
			if (!node.IsMap()) return false;
			rhs.radius = node["Radius"].as<float>();
			rhs.height = node["Height"].as<float>();
			rhs.sectors = node["Sectors"].as<uint32_t>();
			return true;
		}
	};

	template<>
	struct convert<Monsi::ConeParams> {
		static Node encode(const Monsi::ConeParams& rhs) {
			Node node;
			node["Radius"] = rhs.radius;
			node["Height"] = rhs.height;
			node["Sectors"] = rhs.sectors;
			return node;
		}
		static bool decode(const Node& node, Monsi::ConeParams& rhs) {
			if (!node.IsMap()) return false;
			rhs.radius = node["Radius"].as<float>();
			rhs.height = node["Height"].as<float>();
			rhs.sectors = node["Sectors"].as<uint32_t>();
			return true;
		}
	};

	template<>
	struct convert<Monsi::TorusParams> {
		static Node encode(const Monsi::TorusParams& rhs) {
			Node node;
			node["MajorRadius"] = rhs.majorRadius;
			node["MinorRadius"] = rhs.minorRadius;
			node["MajorSegments"] = rhs.majorSegments;
			node["MinorSegments"] = rhs.minorSegments;
			return node;
		}
		static bool decode(const Node& node, Monsi::TorusParams& rhs) {
			if (!node.IsMap()) return false;
			rhs.majorRadius = node["MajorRadius"].as<float>();
			rhs.minorRadius = node["MinorRadius"].as<float>();
			rhs.majorSegments = node["MajorSegments"].as<uint32_t>();
			rhs.minorSegments = node["MinorSegments"].as<uint32_t>();
			return true;
		}
	};

	template<>
	struct convert<Monsi::QuadParams> {
		static Node encode(const Monsi::QuadParams& rhs) {
			Node node;
			node["Width"] = rhs.width;
			node["Height"] = rhs.height;
			return node;
		}
		static bool decode(const Node& node, Monsi::QuadParams& rhs) {
			if (!node.IsMap()) return false;
			rhs.width = node["Width"].as<float>();
			rhs.height = node["Height"].as<float>();
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

			out << YAML::Key << "PrimitiveType" << YAML::Value << (int)mc.MeshAsset->GetPrimitiveType();
			out << YAML::Key << "AmbientColor" << YAML::Value << mc.MeshAsset->GetMaterial()->AmbientColor;
			out << YAML::Key << "DiffuseColor" << YAML::Value << mc.MeshAsset->GetMaterial()->DiffuseColor;
			out << YAML::Key << "SpecularColor" << YAML::Value << mc.MeshAsset->GetMaterial()->SpecularColor;
			out << YAML::Key << "Shininess" << YAML::Value << mc.MeshAsset->GetMaterial()->Shininess;

			out << YAML::Key << "PrimitiveParams";
			out << YAML::Value;
			std::visit([&out](auto&& p) {
				using T = std::decay_t<decltype(p)>;
				if constexpr (std::is_same_v<T, std::monostate>) {
					out << YAML::Null;
				}
				else {
					YAML::Node node(p);
					out << node;
				}
				}, mc.MeshAsset->GetPrimitiveParams());

			out << YAML::EndMap;
		}

		if (entity.HasComponent<ModelComponent>()) {
			auto& mc = entity.GetComponent<ModelComponent>();
			out << YAML::Key << "ModelComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "AssetPath" << YAML::Value << mc.ModelAsset->GetFilePath();
			out << YAML::Key << "CalcTangentSpace" << YAML::Value << mc.ModelAsset->GetModelSettings().CalcTangentSpace;
			out << YAML::Key << "FlipUVs" << YAML::Value << mc.ModelAsset->GetModelSettings().FlipUVs;
			out << YAML::Key << "GenSmoothNormals" << YAML::Value << mc.ModelAsset->GetModelSettings().GenSmoothNormals;
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
				out << YAML::Value << filePath[i];
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
					Reference<Material> material = CreateReference<Material>();
					material->AmbientColor = meshComponentNode["AmbientColor"].as<glm::vec3>();
					material->DiffuseColor = meshComponentNode["DiffuseColor"].as<glm::vec3>();
					material->SpecularColor = meshComponentNode["SpecularColor"].as<glm::vec3>();
					material->Shininess = meshComponentNode["Shininess"].as<float>();

					auto type = (PrimitiveType)meshComponentNode["PrimitiveType"].as<int>();
					auto paramsNode = meshComponentNode["PrimitiveParams"];

					PrimitiveParams params = MeshBuilder::MakeDefaultParams(type);

					switch (type) {
					case PrimitiveType::Sphere: {
						SphereParams p;
						p.radius = paramsNode["Radius"].as<float>();
						p.rings = paramsNode["Rings"].as<uint32_t>();
						p.sectors = paramsNode["Sectors"].as<uint32_t>();
						params = p;
						break;
					}
					case PrimitiveType::Grid: {
						GridParams p;
						p.width = paramsNode["Width"].as<float>();
						p.depth = paramsNode["Depth"].as<float>();
						p.columns = paramsNode["Columns"].as<uint32_t>();
						p.rows = paramsNode["Rows"].as<uint32_t>();
						params = p;
						break;
					}
					case PrimitiveType::Cube: {
						CubeParams p;
						p.size = paramsNode["Size"].as<float>();
						params = p;
						break;
					}
					case PrimitiveType::Cone: {
						ConeParams p;
						p.height = paramsNode["Height"].as<float>();
						p.radius = paramsNode["Radius"].as<float>();
						p.sectors = paramsNode["Sectors"].as<uint32_t>();
						params = p;
						break;
					}
					case PrimitiveType::Cylinder: {
						CylinderParams p;
						p.height = paramsNode["Height"].as<float>();
						p.radius = paramsNode["Radius"].as<float>();
						p.sectors = paramsNode["Sectors"].as<uint32_t>();
						params = p;
						break;
					}
					case PrimitiveType::Quad: {
						QuadParams p;
						p.height = paramsNode["Height"].as<float>();
						p.width = paramsNode["Width"].as<float>();
						params = p;
						break;
					}
					case PrimitiveType::Torus: {
						TorusParams p;
						p.majorRadius = paramsNode["MajorRadius"].as<float>();
						p.minorRadius = paramsNode["MinorRadius"].as<float>();
						p.majorSegments = paramsNode["MajorSegments"].as<uint32_t>();
						p.minorSegments = paramsNode["MinorSegments"].as<uint32_t>();
						params = p;
						break;
					}
					case PrimitiveType::None: {
						break;
					}
					default: ENGINE_LOG_ERROR("PrimitiveParams Error!"); break;
					}

					auto& mc = deserializedEntity.AddComponent<MeshComponent>();
					mc = MeshBuilder::CreateFromParams(params, material);
				}

				auto modelComponentNode = entityNode["ModelComponent"];
				if (modelComponentNode) {
					ModelImportSettings settings;
					std::string assetPath = modelComponentNode["AssetPath"].as<std::string>();
					settings.CalcTangentSpace = modelComponentNode["CalcTangentSpace"].as<bool>();
					settings.FlipUVs = modelComponentNode["FlipUVs"].as<bool>();
					settings.GenSmoothNormals = modelComponentNode["GenSmoothNormals"].as<bool>();

					Reference<Monsi::Model> model = CreateReference<Monsi::Model>(assetPath, settings);

					auto& mc = deserializedEntity.AddComponent<ModelComponent>(model);
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