#include "MonsiPch.h"
#include "SceneManager.h"
#include "Entity.h"
#include "Components.h"
#include "MeshBuilder.h"
#include "ModelImportSettings.h"
#include "Scripts/PlayerControllerScript.h"
#include "Scripts/CameraControllerScript.h"
#include "Scripts/CameraFollowScript.h"
#include <yaml-cpp/yaml.h>
#include <unordered_map>

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

	struct DeferredCameraFollowLink {
		CameraFollowScript* FollowScript = nullptr;
		std::string TargetTag;
		uint64_t TargetID = 0;
		bool HasTargetID = false;
	};

	static uint64_t EntityToID(entt::entity handle) {
		return (uint64_t)(uint32_t)handle;
	}

	static void SerializeModelSettings(YAML::Emitter& out, const ModelImportSettings& settings) {
		out << YAML::Key << "CalcTangentSpace" << YAML::Value << settings.CalcTangentSpace;
		out << YAML::Key << "FlipUVs" << YAML::Value << settings.FlipUVs;
		out << YAML::Key << "GenSmoothNormals" << YAML::Value << settings.GenSmoothNormals;
		out << YAML::Key << "PreTransformVertices" << YAML::Value << settings.PreTransformVertices;
		out << YAML::Key << "MakeLeftHanded" << YAML::Value << settings.MakeLeftHanded;
		out << YAML::Key << "FlipWindingOrder" << YAML::Value << settings.FlipWindingOrder;
		out << YAML::Key << "CoordinateSystem" << YAML::Value << settings.CoordinateSystem;
	}

	template<typename T>
	static T ReadOrDefault(const YAML::Node& node, const char* key, const T& defaultValue) {
		auto child = node[key];
		return child ? child.as<T>() : defaultValue;
	}

	static ModelImportSettings DeserializeModelSettings(const YAML::Node& node) {
		ModelImportSettings settings;
		settings.CalcTangentSpace = ReadOrDefault<bool>(node, "CalcTangentSpace", settings.CalcTangentSpace);
		settings.FlipUVs = ReadOrDefault<bool>(node, "FlipUVs", settings.FlipUVs);
		settings.GenSmoothNormals = ReadOrDefault<bool>(node, "GenSmoothNormals", settings.GenSmoothNormals);
		settings.PreTransformVertices = ReadOrDefault<bool>(node, "PreTransformVertices", settings.PreTransformVertices);
		settings.MakeLeftHanded = ReadOrDefault<bool>(node, "MakeLeftHanded", settings.MakeLeftHanded);
		settings.FlipWindingOrder = ReadOrDefault<bool>(node, "FlipWindingOrder", settings.FlipWindingOrder);
		settings.CoordinateSystem = ReadOrDefault<int>(node, "CoordinateSystem", settings.CoordinateSystem);
		return settings;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity, entt::registry& registry) {
		out << YAML::BeginMap;
		out << YAML::Key << "Entity_ID" << YAML::Value << EntityToID((entt::entity)entity);

		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>();
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << tag.Tag;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<RelationshipComponent>()) {
			auto& rc = entity.GetComponent<RelationshipComponent>();

			std::vector<uint64_t> childIDs;
			for (auto child : rc.Children) {
				if (registry.valid(child))
					childIDs.push_back(EntityToID(child));
			}

			bool hasParent = rc.Parent != entt::null && registry.valid(rc.Parent);
			if (hasParent || !childIDs.empty()) {
				out << YAML::Key << "RelationshipComponent";
				out << YAML::BeginMap;
				if (hasParent) {
					out << YAML::Key << "Parent" << YAML::Value << EntityToID(rc.Parent);
				}
				out << YAML::Key << "Children" << YAML::Value << YAML::Flow << YAML::BeginSeq;
				for (auto childID : childIDs) {
					out << childID;
				}
				out << YAML::EndSeq;
				out << YAML::EndMap;
			}
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

		if (entity.HasComponent<ShadowMapComponent>()) {
			auto& smc = entity.GetComponent<ShadowMapComponent>();
			out << YAML::Key << "ShadowMapComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Width" << YAML::Value << smc.Settings.Width;
			out << YAML::Key << "Height" << YAML::Value << smc.Settings.Height;
			out << YAML::Key << "LightDistance" << YAML::Value << smc.Settings.LightDistance;
			out << YAML::Key << "Near" << YAML::Value << smc.Settings.NearPlane;
			out << YAML::Key << "Far" << YAML::Value << smc.Settings.FarPlane;
			out << YAML::Key << "OrthoSize" << YAML::Value << smc.Settings.OrthoSize;
			out << YAML::Key << "ShadowIntensity" << YAML::Value << smc.Settings.ShadowIntensity;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<NativeScriptComponent>()) {
			auto& nsc = entity.GetComponent<NativeScriptComponent>();
			out << YAML::Key << "NativeScriptComponent";
			out << YAML::BeginMap;

			if (auto* playerScript = nsc.GetScriptAs<PlayerControllerScript>()) {
				out << YAML::Key << "ScriptClassName" << YAML::Value << "PlayerControllerScript";
			}
			else if (auto* camScript = nsc.GetScriptAs<CameraControllerScript>()) {
				out << YAML::Key << "ScriptClassName" << YAML::Value << "CameraControllerScript";
				out << YAML::Key << "LookMode" << YAML::Value << (int)camScript->GetLookMode();
				out << YAML::Key << "YawPitchRoll" << YAML::Value << camScript->GetYawPitchRoll();
			}
			else if (auto* followScript = nsc.GetScriptAs<CameraFollowScript>()) {
				out << YAML::Key << "ScriptClassName" << YAML::Value << "CameraFollowScript";
				out << YAML::Key << "LookAtTarget" << YAML::Value << followScript->GetLookAtTarget();
				out << YAML::Key << "LookAtOffset" << YAML::Value << followScript->GetLookAtOffset();
				out << YAML::Key << "PositionSmoothSpeed" << YAML::Value << followScript->GetPositionSmoothSpeed();
				out << YAML::Key << "RotationSmoothSpeed" << YAML::Value << followScript->GetRotationSmoothSpeed();
				out << YAML::Key << "PositionOffset" << YAML::Value << followScript->GetPositionOffset();
				out << YAML::Key << "RotationOffset" << YAML::Value << followScript->GetRotationOffset();

				Entity target = followScript->GetTarget();
				if (target && registry.valid((entt::entity)target)) {
					out << YAML::Key << "TargetID" << YAML::Value << EntityToID((entt::entity)target);
					if (target.HasComponent<TagComponent>()) {
						out << YAML::Key << "TargetTag" << YAML::Value << target.GetComponent<TagComponent>().Tag;
					}
				}
			}

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SSAOComponent>()) {
			auto& sc = entity.GetComponent<SSAOComponent>();
			out << YAML::Key << "SSAOComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "KernelSize" << YAML::Value << sc.Settings.KernelSize;
			out << YAML::Key << "Radius" << YAML::Value << sc.Settings.Radius;
			out << YAML::Key << "Bias" << YAML::Value << sc.Settings.Bias;
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

		if (entity.HasComponent<StaticMeshComponent>()) {
			auto& mc = entity.GetComponent<StaticMeshComponent>();
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

		if (entity.HasComponent<StaticModelComponent>()) {
			auto& mc = entity.GetComponent<StaticModelComponent>();
			out << YAML::Key << "ModelComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "AssetPath" << YAML::Value << mc.ModelAsset->GetFilePath();
			SerializeModelSettings(out, mc.ModelAsset->GetModelSettings());
			out << YAML::EndMap;
		}

		if (entity.HasComponent<AnimatedModelComponent>()) {
			auto& mc = entity.GetComponent<AnimatedModelComponent>();
			out << YAML::Key << "AnimatedModelComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "AssetPath" << YAML::Value << mc.ModelAsset->GetFilePath();
			SerializeModelSettings(out, mc.ModelAsset->GetModelSettings());
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SkyBoxComponent>()) {
			auto& sbc = entity.GetComponent<SkyBoxComponent>();

			out << YAML::Key << "SkyBoxComponent";
			out << YAML::BeginMap;

			if (!sbc.SingleFilePath.empty()) {
				out << YAML::Key << "SingleFilePath" << YAML::Value << sbc.SingleFilePath;
			}
			else {
				out << YAML::Key << "AssetPath";
				out << YAML::BeginSeq;
				for (const auto& path : sbc.FilePath) {
					out << YAML::Value << path;
				}
				out << YAML::EndSeq;
			}

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

			SerializeEntity(out, entity, m_Scene->m_Registry);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	bool SceneManager::LoadScene(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		if (!stream.is_open()) return false;

		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data;
		try {
			data = YAML::Load(strStream.str());
		}
		catch (const YAML::Exception& e) {
			ENGINE_LOG_ERROR("Failed to parse scene '{0}': {1}", filePath, e.what());
			return false;
		}

		if (!data["Scene"]) return false;

		std::string sceneName = data["Scene"].as<std::string>();
		ENGINE_LOG_TRACE("Loading scene '{0}'", sceneName);

		std::vector<DeferredCameraFollowLink> deferredCameraFollowLinks;
		std::unordered_map<uint64_t, entt::entity> idToEntity;
		std::vector<std::pair<uint64_t, YAML::Node>> relationshipNodes;

		auto entities = data["Entities"];
		if (entities) {
			for (auto entityNode : entities) {
				uint64_t uuid = entityNode["Entity_ID"] ? entityNode["Entity_ID"].as<uint64_t>() : 0;

				std::string name;
				auto tagComponentNode = entityNode["TagComponent"];
				if (tagComponentNode) {
					name = tagComponentNode["Tag"].as<std::string>();
				}

				Entity deserializedEntity = m_Scene->CreateEntity(name, false);
				ENGINE_LOG_TRACE("Loaded entity with ID = {0}, name = {1}", uuid, name);

				idToEntity[uuid] = (entt::entity)deserializedEntity;

				auto relationshipComponentNode = entityNode["RelationshipComponent"];
				if (relationshipComponentNode) {
					relationshipNodes.emplace_back(uuid, relationshipComponentNode);
				}

				auto transformComponentNode = entityNode["TransformComponent"];
				if (transformComponentNode) {
					auto& tc = deserializedEntity.AddComponent<TransformComponent>();
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

				auto shadowMapComponent = entityNode["ShadowMapComponent"];
				if (shadowMapComponent) {
					auto width = shadowMapComponent["Width"].as<uint32_t>();
					auto height = shadowMapComponent["Height"].as<uint32_t>();

					auto smr = ShadowMap::Create(width, height);
					auto& smc = deserializedEntity.AddComponent<ShadowMapComponent>(smr);

					smc.Settings.Width = width;
					smc.Settings.Height = height;
					smc.Settings.LightDistance = shadowMapComponent["LightDistance"].as<float>();
					smc.Settings.NearPlane = shadowMapComponent["Near"].as<float>();
					smc.Settings.FarPlane = shadowMapComponent["Far"].as<float>();
					smc.Settings.OrthoSize = shadowMapComponent["OrthoSize"].as<float>();
					smc.Settings.ShadowIntensity = shadowMapComponent["ShadowIntensity"].as<float>();
				}

				auto ssaoComponentNode = entityNode["SSAOComponent"];
				if (ssaoComponentNode) {
					auto ssaoInstance = SSAO::Create();
					auto& sc = deserializedEntity.AddComponent<SSAOComponent>(ssaoInstance);

					sc.Settings.KernelSize = ssaoComponentNode["KernelSize"].as<int>();
					sc.Settings.Radius = ssaoComponentNode["Radius"].as<float>();
					sc.Settings.Bias = ssaoComponentNode["Bias"].as<float>();

					if (sc.SSAOInstance) {
						sc.SSAOInstance->SetSettings(sc.Settings);
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

					auto& mc = deserializedEntity.AddComponent<StaticMeshComponent>();
					mc.MeshAsset = MeshBuilder::CreateFromParams(params, material);
				}

				auto modelComponentNode = entityNode["ModelComponent"];
				if (modelComponentNode) {
					std::string assetPath = modelComponentNode["AssetPath"].as<std::string>();
					ModelImportSettings settings = DeserializeModelSettings(modelComponentNode);

					Reference<Monsi::StaticModel> model = CreateReference<Monsi::StaticModel>(assetPath, settings);

					auto& mc = deserializedEntity.AddComponent<StaticModelComponent>(model);
				}

				auto animatedModelComponentNode = entityNode["AnimatedModelComponent"];
				if (animatedModelComponentNode) {
					std::string assetPath = animatedModelComponentNode["AssetPath"].as<std::string>();
					ModelImportSettings settings = DeserializeModelSettings(animatedModelComponentNode);

					Reference<Monsi::AnimatedModel> model = CreateReference<Monsi::AnimatedModel>(assetPath, settings);

					auto& amc = deserializedEntity.AddComponent<AnimatedModelComponent>(model);
				}

				auto skyBoxComponentNode = entityNode["SkyBoxComponent"];
				if (skyBoxComponentNode) {
					auto singleFilePathNode = skyBoxComponentNode["SingleFilePath"];
					auto assetPathsNode = skyBoxComponentNode["AssetPath"];

					if (singleFilePathNode) {
						std::string singlePath = singleFilePathNode.as<std::string>();
						auto skyBoxTexture = Monsi::CubeMapTexture::Create(singlePath);

						deserializedEntity.AddComponent<Monsi::SkyBoxComponent>(skyBoxTexture, singlePath);
					}
					else if (assetPathsNode && assetPathsNode.IsSequence()) {
						std::array<std::string, 6> assetPaths;
						for (size_t i = 0; i < 6 && i < assetPathsNode.size(); i++) {
							assetPaths[i] = assetPathsNode[i].as<std::string>();
						}

						auto skyBoxTexture = Monsi::CubeMapTexture::Create(assetPaths);

						deserializedEntity.AddComponent<Monsi::SkyBoxComponent>(skyBoxTexture, assetPaths);
					}
				}

				auto nativeScriptComponentNode = entityNode["NativeScriptComponent"];
				if (nativeScriptComponentNode) {
					std::string scriptClassName = nativeScriptComponentNode["ScriptClassName"] ? nativeScriptComponentNode["ScriptClassName"].as<std::string>() : std::string();

					if (scriptClassName == "PlayerControllerScript") {
						auto& nsc = deserializedEntity.AddComponent<NativeScriptComponent>();
						nsc.Bind<PlayerControllerScript>();
					}
					else if (scriptClassName == "CameraControllerScript") {
						auto& nsc = deserializedEntity.AddComponent<NativeScriptComponent>();
						nsc.Bind<CameraControllerScript>();
						auto* camScript = nsc.GetScriptAs<CameraControllerScript>();

						if (camScript) {
							if (nativeScriptComponentNode["LookMode"]) {
								camScript->SetLookMode((CameraControllerScript::LookMode)nativeScriptComponentNode["LookMode"].as<int>());
							}
							if (nativeScriptComponentNode["YawPitchRoll"]) {
								glm::vec3 ypr = nativeScriptComponentNode["YawPitchRoll"].as<glm::vec3>();
								camScript->SetYawPitchRoll(ypr.x, ypr.y, ypr.z);
							}
						}
					}
					else if (scriptClassName == "CameraFollowScript") {
						auto& nsc = deserializedEntity.AddComponent<NativeScriptComponent>();
						nsc.Bind<CameraFollowScript>();
						auto* followScript = nsc.GetScriptAs<CameraFollowScript>();

						if (followScript) {
							if (nativeScriptComponentNode["LookAtTarget"]) {
								followScript->SetLookAtTarget(nativeScriptComponentNode["LookAtTarget"].as<bool>());
							}
							if (nativeScriptComponentNode["LookAtOffset"]) {
								followScript->SetLookAtOffset(nativeScriptComponentNode["LookAtOffset"].as<glm::vec3>());
							}
							if (nativeScriptComponentNode["PositionSmoothSpeed"]) {
								followScript->SetPositionSmoothSpeed(nativeScriptComponentNode["PositionSmoothSpeed"].as<float>());
							}
							if (nativeScriptComponentNode["RotationSmoothSpeed"]) {
								followScript->SetRotationSmoothSpeed(nativeScriptComponentNode["RotationSmoothSpeed"].as<float>());
							}
							if (nativeScriptComponentNode["PositionOffset"]) {
								followScript->SetPositionOffset(nativeScriptComponentNode["PositionOffset"].as<glm::vec3>());
							}
							if (nativeScriptComponentNode["RotationOffset"]) {
								followScript->SetRotationOffset(nativeScriptComponentNode["RotationOffset"].as<glm::quat>());
							}

							DeferredCameraFollowLink link;
							link.FollowScript = followScript;
							if (nativeScriptComponentNode["TargetTag"]) {
								link.TargetTag = nativeScriptComponentNode["TargetTag"].as<std::string>();
							}
							if (nativeScriptComponentNode["TargetID"]) {
								link.TargetID = nativeScriptComponentNode["TargetID"].as<uint64_t>();
								link.HasTargetID = true;
							}
							if (link.HasTargetID || !link.TargetTag.empty()) {
								deferredCameraFollowLinks.push_back(link);
							}
						}
						else {
							ENGINE_LOG_ERROR("CameraFollowScript instance was null after Bind");
						}
					}
				}
			}

			for (const auto& [ownerID, relationshipNode] : relationshipNodes) {
				auto ownerIt = idToEntity.find(ownerID);
				if (ownerIt == idToEntity.end()) continue;

				Entity owner{ ownerIt->second, m_Scene.get() };

				auto childrenNode = relationshipNode["Children"];
				if (childrenNode && childrenNode.IsSequence()) {
					for (auto childNode : childrenNode) {
						auto childIt = idToEntity.find(childNode.as<uint64_t>());
						if (childIt == idToEntity.end() || childIt->second == ownerIt->second) continue;

						Entity child{ childIt->second, m_Scene.get() };
						child.SetParent(owner, false);
					}
				}
			}

			for (const auto& [ownerID, relationshipNode] : relationshipNodes) {
				auto ownerIt = idToEntity.find(ownerID);
				if (ownerIt == idToEntity.end() || !relationshipNode["Parent"]) continue;

				auto parentIt = idToEntity.find(relationshipNode["Parent"].as<uint64_t>());
				if (parentIt == idToEntity.end() || parentIt->second == ownerIt->second) continue;

				Entity owner{ ownerIt->second, m_Scene.get() };
				bool alreadyParented = owner.HasComponent<RelationshipComponent>() && owner.GetComponent<RelationshipComponent>().Parent == parentIt->second;
				if (!alreadyParented) {
					owner.SetParent(Entity{ parentIt->second, m_Scene.get() }, false);
				}
			}

			for (const auto& link : deferredCameraFollowLinks) {
				bool found = false;

				if (link.HasTargetID) {
					auto targetIt = idToEntity.find(link.TargetID);
					if (targetIt != idToEntity.end()) {
						link.FollowScript->SetTarget(Entity{ targetIt->second, m_Scene.get() });
						found = true;
					}
				}

				if (!found && !link.TargetTag.empty()) {
					for (auto entHandle : m_Scene->m_Registry.view<entt::entity>()) {
						Entity ent{ entHandle, m_Scene.get() };
						if (ent.HasComponent<TagComponent>() && ent.GetComponent<TagComponent>().Tag == link.TargetTag) {
							link.FollowScript->SetTarget(ent);
							found = true;
							break;
						}
					}
				}

				if (!found) {
					ENGINE_LOG_ERROR("CameraFollowScript target '{0}' not found", link.TargetTag);
				}
			}
		}
		return true;
	}
}