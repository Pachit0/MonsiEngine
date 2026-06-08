#include "SceneHierarchyUnit.h"

#include <imgui.h>

#include "Components.h"

namespace Monsi {

	SceneHierarchyUnit::SceneHierarchyUnit(const Reference<Scene>& scene)
	{
		SetContext(scene);

	}

	void SceneHierarchyUnit::SetContext(const Reference<Scene>& scene)
	{
		m_Scene = scene;
	}

	void SceneHierarchyUnit::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		if (m_Scene)
		{
			for (auto entity : m_Scene->m_Registry.view<entt::entity>())
				{
					Entity ent{ entity, m_Scene.get() };
					if (ent.HasComponent<TagComponent>()) {
						DrawEntityNode(ent);
					}
				};
		}

		ImGui::End();
	}

	void SceneHierarchyUnit::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_Selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_Selected = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)1337, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

	}

}