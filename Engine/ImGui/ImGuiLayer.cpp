#include "MonsiPch.h"
#include "ImGuiLayer.h"

#include "imgui_impl_opengl3.h"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "Engine.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Monsi {

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {

	}

	ImGuiLayer::~ImGuiLayer() {

	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = ImVec4{ 0.95f, 0.96f, 0.98f, 1.0f };
		colors[ImGuiCol_TextDisabled] = ImVec4{ 0.50f, 0.50f, 0.50f, 1.0f };
		colors[ImGuiCol_TextSelectedBg] = ImVec4{ 0.30f, 0.55f, 0.90f, 0.35f };

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.10f, 0.105f, 0.11f, 1.0f };
		colors[ImGuiCol_ChildBg] = ImVec4{ 0.10f, 0.105f, 0.11f, 1.0f };
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.10f, 0.105f, 0.11f, 1.0f };

		colors[ImGuiCol_Border] = ImVec4{ 0.05f, 0.05f, 0.05f, 0.5f };
		colors[ImGuiCol_BorderShadow] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.0f };

		colors[ImGuiCol_FrameBg] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.30f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.25f, 0.255f, 0.26f, 1.0f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.095f, 0.10f, 0.105f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.095f, 0.10f, 0.105f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.095f, 0.10f, 0.105f, 1.0f };

		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.09f, 0.095f, 0.10f, 1.0f };

		colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.09f, 0.095f, 0.10f, 1.0f };
		colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.25f, 0.255f, 0.26f, 1.0f };
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.32f, 0.325f, 0.33f, 1.0f };
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.40f, 0.405f, 0.41f, 1.0f };

		colors[ImGuiCol_CheckMark] = ImVec4{ 0.30f, 0.55f, 0.90f, 1.0f };
		colors[ImGuiCol_SliderGrab] = ImVec4{ 0.30f, 0.55f, 0.90f, 1.0f };
		colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.40f, 0.65f, 1.00f, 1.0f };

		colors[ImGuiCol_Button] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.30f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.25f, 0.255f, 0.26f, 1.0f };

		colors[ImGuiCol_Header] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.30f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.25f, 0.255f, 0.26f, 1.0f };

		colors[ImGuiCol_Separator] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.30f, 0.55f, 0.90f, 0.78f };
		colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.30f, 0.55f, 0.90f, 1.0f };

		colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.30f, 0.55f, 0.90f, 0.20f };
		colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.30f, 0.55f, 0.90f, 0.60f };
		colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.30f, 0.55f, 0.90f, 0.90f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.385f, 0.39f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.285f, 0.29f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };

		colors[ImGuiCol_DockingPreview] = ImVec4{ 0.30f, 0.55f, 0.90f, 0.70f };
		colors[ImGuiCol_DockingEmptyBg] = ImVec4{ 0.10f, 0.105f, 0.11f, 1.0f };

		colors[ImGuiCol_PlotLines] = ImVec4{ 0.61f, 0.61f, 0.61f, 1.0f };
		colors[ImGuiCol_PlotLinesHovered] = ImVec4{ 0.30f, 0.55f, 0.90f, 1.0f };
		colors[ImGuiCol_PlotHistogram] = ImVec4{ 0.30f, 0.55f, 0.90f, 1.0f };
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4{ 0.40f, 0.65f, 1.00f, 1.0f };

		colors[ImGuiCol_TableHeaderBg] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TableBorderStrong] = ImVec4{ 0.05f, 0.05f, 0.05f, 1.0f };
		colors[ImGuiCol_TableBorderLight] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TableRowBg] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.0f };
		colors[ImGuiCol_TableRowBgAlt] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.03f };

		colors[ImGuiCol_DragDropTarget] = ImVec4{ 0.30f, 0.55f, 0.90f, 0.9f };
		colors[ImGuiCol_NavHighlight] = ImVec4{ 0.30f, 0.55f, 0.90f, 1.0f };
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.70f };
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4{ 0.20f, 0.20f, 0.20f, 0.20f };
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4{ 0.20f, 0.20f, 0.20f, 0.35f };
	}

	void ImGuiLayer::SetLightThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = ImVec4{ 0.15f, 0.15f, 0.16f, 1.0f };
		colors[ImGuiCol_TextDisabled] = ImVec4{ 0.55f, 0.55f, 0.55f, 1.0f };
		colors[ImGuiCol_TextSelectedBg] = ImVec4{ 0.20f, 0.45f, 0.85f, 0.35f };

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.94f, 0.94f, 0.94f, 1.0f };
		colors[ImGuiCol_ChildBg] = ImVec4{ 0.94f, 0.94f, 0.94f, 1.0f };
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.97f, 0.97f, 0.97f, 1.0f };

		colors[ImGuiCol_Border] = ImVec4{ 0.80f, 0.80f, 0.80f, 0.5f };
		colors[ImGuiCol_BorderShadow] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.0f };

		colors[ImGuiCol_FrameBg] = ImVec4{ 0.88f, 0.88f, 0.88f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.78f, 0.83f, 0.93f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.73f, 0.78f, 0.88f, 1.0f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.90f, 0.90f, 0.90f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.86f, 0.86f, 0.86f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.90f, 0.90f, 0.90f, 1.0f };

		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.88f, 0.88f, 0.88f, 1.0f };

		colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.90f, 0.90f, 0.90f, 1.0f };
		colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.75f, 0.75f, 0.75f, 1.0f };
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.65f, 0.65f, 0.65f, 1.0f };
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.55f, 0.55f, 0.55f, 1.0f };

		colors[ImGuiCol_CheckMark] = ImVec4{ 0.20f, 0.45f, 0.85f, 1.0f };
		colors[ImGuiCol_SliderGrab] = ImVec4{ 0.20f, 0.45f, 0.85f, 1.0f };
		colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.15f, 0.35f, 0.70f, 1.0f };

		colors[ImGuiCol_Button] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.70f, 0.80f, 0.95f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.60f, 0.75f, 0.95f, 1.0f };

		colors[ImGuiCol_Header] = ImVec4{ 0.80f, 0.80f, 0.80f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.70f, 0.80f, 0.95f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.60f, 0.75f, 0.95f, 1.0f };

		colors[ImGuiCol_Separator] = ImVec4{ 0.80f, 0.80f, 0.80f, 1.0f };
		colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.20f, 0.45f, 0.85f, 0.78f };
		colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.20f, 0.45f, 0.85f, 1.0f };

		colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.20f, 0.45f, 0.85f, 0.20f };
		colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.20f, 0.45f, 0.85f, 0.60f };
		colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.20f, 0.45f, 0.85f, 0.90f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.86f, 0.86f, 0.86f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.70f, 0.80f, 0.95f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.78f, 0.85f, 0.97f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.88f, 0.88f, 0.88f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.83f, 0.83f, 0.83f, 1.0f };

		colors[ImGuiCol_DockingPreview] = ImVec4{ 0.20f, 0.45f, 0.85f, 0.50f };
		colors[ImGuiCol_DockingEmptyBg] = ImVec4{ 0.94f, 0.94f, 0.94f, 1.0f };

		colors[ImGuiCol_PlotLines] = ImVec4{ 0.39f, 0.39f, 0.39f, 1.0f };
		colors[ImGuiCol_PlotLinesHovered] = ImVec4{ 0.20f, 0.45f, 0.85f, 1.0f };
		colors[ImGuiCol_PlotHistogram] = ImVec4{ 0.20f, 0.45f, 0.85f, 1.0f };
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4{ 0.15f, 0.35f, 0.70f, 1.0f };

		colors[ImGuiCol_TableHeaderBg] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };
		colors[ImGuiCol_TableBorderStrong] = ImVec4{ 0.75f, 0.75f, 0.75f, 1.0f };
		colors[ImGuiCol_TableBorderLight] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };
		colors[ImGuiCol_TableRowBg] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.0f };
		colors[ImGuiCol_TableRowBgAlt] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.03f };

		colors[ImGuiCol_DragDropTarget] = ImVec4{ 0.20f, 0.45f, 0.85f, 0.90f };
		colors[ImGuiCol_NavHighlight] = ImVec4{ 0.20f, 0.45f, 0.85f, 1.0f };
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.70f };
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4{ 0.80f, 0.80f, 0.80f, 0.20f };
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4{ 0.80f, 0.80f, 0.80f, 0.35f };
	}

	void ImGuiLayer::SetClassicThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = ImVec4{ 0.90f, 0.90f, 0.90f, 1.00f };
		colors[ImGuiCol_TextDisabled] = ImVec4{ 0.60f, 0.60f, 0.60f, 1.00f };
		colors[ImGuiCol_TextSelectedBg] = ImVec4{ 0.45f, 0.45f, 0.90f, 0.35f };

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.70f };
		colors[ImGuiCol_ChildBg] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f };
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.05f, 0.05f, 0.10f, 0.90f };

		colors[ImGuiCol_Border] = ImVec4{ 0.70f, 0.70f, 0.70f, 0.65f };
		colors[ImGuiCol_BorderShadow] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f };

		colors[ImGuiCol_FrameBg] = ImVec4{ 0.80f, 0.80f, 0.80f, 0.30f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.90f, 0.80f, 0.80f, 0.40f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.90f, 0.65f, 0.65f, 0.45f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.27f, 0.27f, 0.54f, 0.83f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.32f, 0.32f, 0.63f, 0.87f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.40f, 0.40f, 0.80f, 0.20f };

		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.40f, 0.40f, 0.55f, 0.80f };

		colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.20f, 0.25f, 0.30f, 0.60f };
		colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.40f, 0.40f, 0.80f, 0.30f };
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.40f, 0.40f, 0.80f, 0.40f };
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.80f, 0.50f, 0.50f, 0.40f };

		colors[ImGuiCol_CheckMark] = ImVec4{ 0.90f, 0.90f, 0.90f, 0.50f };
		colors[ImGuiCol_SliderGrab] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.30f };
		colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.80f, 0.50f, 0.50f, 1.00f };

		colors[ImGuiCol_Button] = ImVec4{ 0.67f, 0.40f, 0.40f, 0.60f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.67f, 0.40f, 0.40f, 1.00f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.80f, 0.50f, 0.50f, 1.00f };

		colors[ImGuiCol_Header] = ImVec4{ 0.40f, 0.40f, 0.90f, 0.45f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.45f, 0.45f, 0.90f, 0.80f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.53f, 0.53f, 0.87f, 1.00f };

		colors[ImGuiCol_Separator] = ImVec4{ 0.50f, 0.50f, 0.50f, 1.00f };
		colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.60f, 0.60f, 0.70f, 1.00f };
		colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.70f, 0.70f, 0.90f, 1.00f };

		colors[ImGuiCol_ResizeGrip] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.15f };
		colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.78f, 0.82f, 1.00f, 0.60f };
		colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.78f, 0.82f, 1.00f, 0.90f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.30f, 0.30f, 0.60f, 0.79f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.45f, 0.45f, 0.90f, 0.80f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.40f, 0.40f, 0.72f, 0.92f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.28f, 0.28f, 0.57f, 0.82f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.35f, 0.35f, 0.67f, 0.89f };

		colors[ImGuiCol_DockingPreview] = ImVec4{ 0.40f, 0.40f, 0.90f, 0.31f };
		colors[ImGuiCol_DockingEmptyBg] = ImVec4{ 0.20f, 0.20f, 0.20f, 1.00f };

		colors[ImGuiCol_PlotLines] = ImVec4{ 1.00f, 1.00f, 1.00f, 1.00f };
		colors[ImGuiCol_PlotLinesHovered] = ImVec4{ 0.90f, 0.70f, 0.00f, 1.00f };
		colors[ImGuiCol_PlotHistogram] = ImVec4{ 0.90f, 0.70f, 0.00f, 1.00f };
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4{ 1.00f, 0.60f, 0.00f, 1.00f };

		colors[ImGuiCol_TableHeaderBg] = ImVec4{ 0.27f, 0.27f, 0.38f, 1.00f };
		colors[ImGuiCol_TableBorderStrong] = ImVec4{ 0.31f, 0.31f, 0.45f, 1.00f };
		colors[ImGuiCol_TableBorderLight] = ImVec4{ 0.26f, 0.26f, 0.35f, 1.00f };
		colors[ImGuiCol_TableRowBg] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f };
		colors[ImGuiCol_TableRowBgAlt] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.05f };

		colors[ImGuiCol_DragDropTarget] = ImVec4{ 1.00f, 1.00f, 0.00f, 0.90f };
		colors[ImGuiCol_NavHighlight] = ImVec4{ 0.45f, 0.45f, 0.90f, 0.80f };
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.70f };
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4{ 0.80f, 0.80f, 0.80f, 0.20f };
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4{ 0.20f, 0.20f, 0.20f, 0.35f };
	}

	void ImGuiLayer::SetCyanThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = ImVec4{ 0.90f, 1.00f, 1.00f, 1.00f };
		colors[ImGuiCol_TextDisabled] = ImVec4{ 0.45f, 0.65f, 0.68f, 1.00f };

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.04f, 0.12f, 0.14f, 1.00f };
		colors[ImGuiCol_ChildBg] = ImVec4{ 0.04f, 0.12f, 0.14f, 1.00f };
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.05f, 0.14f, 0.16f, 1.00f };

		colors[ImGuiCol_Border] = ImVec4{ 0.10f, 0.45f, 0.50f, 0.70f };
		colors[ImGuiCol_BorderShadow] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f };

		colors[ImGuiCol_FrameBg] = ImVec4{ 0.08f, 0.22f, 0.25f, 1.00f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.10f, 0.35f, 0.40f, 1.00f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.12f, 0.45f, 0.50f, 1.00f };

		colors[ImGuiCol_Button] = ImVec4{ 0.08f, 0.22f, 0.25f, 1.00f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.10f, 0.45f, 0.50f, 1.00f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.12f, 0.55f, 0.60f, 1.00f };

		colors[ImGuiCol_Header] = ImVec4{ 0.08f, 0.22f, 0.25f, 1.00f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.10f, 0.45f, 0.50f, 1.00f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.12f, 0.55f, 0.60f, 1.00f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.06f, 0.18f, 0.21f, 1.00f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.10f, 0.45f, 0.50f, 1.00f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.12f, 0.55f, 0.60f, 1.00f };

		colors[ImGuiCol_CheckMark] = ImVec4{ 0.20f, 0.95f, 1.00f, 1.00f };
		colors[ImGuiCol_SliderGrab] = ImVec4{ 0.15f, 0.75f, 0.85f, 1.00f };
		colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.20f, 0.95f, 1.00f, 1.00f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.03f, 0.10f, 0.12f, 1.00f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.06f, 0.25f, 0.30f, 1.00f };
		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.05f, 0.16f, 0.18f, 1.00f };

		colors[ImGuiCol_Separator] = ImVec4{ 0.10f, 0.45f, 0.50f, 1.00f };
		colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.20f, 0.75f, 0.85f, 1.00f };
		colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.25f, 0.90f, 1.00f, 1.00f };

		colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.00f, 0.70f, 0.70f, 0.20f };
		colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.00f, 0.70f, 0.70f, 0.60f };
		colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.00f, 0.70f, 0.70f, 0.90f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.00f, 0.70f, 0.70f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.285f, 0.29f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };

		colors[ImGuiCol_DockingPreview] = ImVec4{ 0.00f, 0.70f, 0.70f, 0.70f };
		colors[ImGuiCol_DockingEmptyBg] = ImVec4{ 0.10f, 0.105f, 0.11f, 1.0f };

		colors[ImGuiCol_PlotLines] = ImVec4{ 0.61f, 0.61f, 0.61f, 1.0f };
		colors[ImGuiCol_PlotLinesHovered] = ImVec4{ 0.00f, 0.70f, 0.70f, 1.0f };
		colors[ImGuiCol_PlotHistogram] = ImVec4{ 0.00f, 0.70f, 0.70f, 1.0f };
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4{ 0.00f, 0.85f, 0.85f, 1.0f };

		colors[ImGuiCol_TableHeaderBg] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TableBorderStrong] = ImVec4{ 0.05f, 0.05f, 0.05f, 1.0f };
		colors[ImGuiCol_TableBorderLight] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TableRowBg] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.0f };
		colors[ImGuiCol_TableRowBgAlt] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.03f };

		colors[ImGuiCol_DragDropTarget] = ImVec4{ 0.00f, 0.70f, 0.70f, 0.90f };
		colors[ImGuiCol_NavHighlight] = ImVec4{ 0.00f, 0.70f, 0.70f, 1.0f };
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.70f };
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4{ 0.20f, 0.20f, 0.20f, 0.20f };
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4{ 0.20f, 0.20f, 0.20f, 0.35f };
	}

	void ImGuiLayer::SetMagentaThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = ImVec4{ 1.00f, 0.90f, 1.00f, 1.00f };
		colors[ImGuiCol_TextDisabled] = ImVec4{ 0.65f, 0.45f, 0.65f, 1.00f };

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.14f, 0.04f, 0.14f, 1.00f };
		colors[ImGuiCol_ChildBg] = ImVec4{ 0.14f, 0.04f, 0.14f, 1.00f };
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.17f, 0.05f, 0.17f, 1.00f };

		colors[ImGuiCol_Border] = ImVec4{ 0.55f, 0.12f, 0.55f, 0.70f };
		colors[ImGuiCol_BorderShadow] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f };

		colors[ImGuiCol_FrameBg] = ImVec4{ 0.27f, 0.09f, 0.27f, 1.00f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.45f, 0.15f, 0.45f, 1.00f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.60f, 0.18f, 0.60f, 1.00f };

		colors[ImGuiCol_Button] = ImVec4{ 0.27f, 0.09f, 0.27f, 1.00f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.55f, 0.15f, 0.55f, 1.00f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.75f, 0.20f, 0.75f, 1.00f };

		colors[ImGuiCol_Header] = ImVec4{ 0.27f, 0.09f, 0.27f, 1.00f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.55f, 0.15f, 0.55f, 1.00f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.75f, 0.20f, 0.75f, 1.00f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.21f, 0.07f, 0.21f, 1.00f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.55f, 0.15f, 0.55f, 1.00f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.75f, 0.20f, 0.75f, 1.00f };

		colors[ImGuiCol_CheckMark] = ImVec4{ 1.00f, 0.30f, 1.00f, 1.00f };
		colors[ImGuiCol_SliderGrab] = ImVec4{ 0.85f, 0.20f, 0.85f, 1.00f };
		colors[ImGuiCol_SliderGrabActive] = ImVec4{ 1.00f, 0.40f, 1.00f, 1.00f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.10f, 0.03f, 0.10f, 1.00f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.35f, 0.08f, 0.35f, 1.00f };
		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.17f, 0.05f, 0.17f, 1.00f };

		colors[ImGuiCol_Separator] = ImVec4{ 0.55f, 0.12f, 0.55f, 1.00f };
		colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.85f, 0.20f, 0.85f, 1.00f };
		colors[ImGuiCol_SeparatorActive] = ImVec4{ 1.00f, 0.40f, 1.00f, 1.00f };

		colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.70f, 0.00f, 0.70f, 0.20f };
		colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.70f, 0.00f, 0.70f, 0.60f };
		colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.70f, 0.00f, 0.70f, 0.90f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.70f, 0.00f, 0.70f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.285f, 0.29f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };

		colors[ImGuiCol_DockingPreview] = ImVec4{ 0.70f, 0.00f, 0.70f, 0.70f };
		colors[ImGuiCol_DockingEmptyBg] = ImVec4{ 0.10f, 0.105f, 0.11f, 1.0f };

		colors[ImGuiCol_PlotLines] = ImVec4{ 0.61f, 0.61f, 0.61f, 1.0f };
		colors[ImGuiCol_PlotLinesHovered] = ImVec4{ 0.70f, 0.00f, 0.70f, 1.0f };
		colors[ImGuiCol_PlotHistogram] = ImVec4{ 0.70f, 0.00f, 0.70f, 1.0f };
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4{ 0.85f, 0.00f, 0.85f, 1.0f };

		colors[ImGuiCol_TableHeaderBg] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TableBorderStrong] = ImVec4{ 0.05f, 0.05f, 0.05f, 1.0f };
		colors[ImGuiCol_TableBorderLight] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TableRowBg] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.0f };
		colors[ImGuiCol_TableRowBgAlt] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.03f };

		colors[ImGuiCol_DragDropTarget] = ImVec4{ 0.70f, 0.00f, 0.70f, 0.90f };
		colors[ImGuiCol_NavHighlight] = ImVec4{ 0.70f, 0.00f, 0.70f, 1.0f };
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.70f };
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4{ 0.20f, 0.20f, 0.20f, 0.20f };
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4{ 0.20f, 0.20f, 0.20f, 0.35f };
	}

	void ImGuiLayer::SetRedThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = ImVec4{ 1.00f, 0.92f, 0.92f, 1.00f };
		colors[ImGuiCol_TextDisabled] = ImVec4{ 0.65f, 0.45f, 0.45f, 1.00f };

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.15f, 0.05f, 0.05f, 1.00f };
		colors[ImGuiCol_ChildBg] = ImVec4{ 0.15f, 0.05f, 0.05f, 1.00f };
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.18f, 0.06f, 0.06f, 1.00f };

		colors[ImGuiCol_Border] = ImVec4{ 0.55f, 0.12f, 0.12f, 0.70f };
		colors[ImGuiCol_BorderShadow] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f };

		colors[ImGuiCol_FrameBg] = ImVec4{ 0.28f, 0.10f, 0.10f, 1.00f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.45f, 0.15f, 0.15f, 1.00f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.60f, 0.18f, 0.18f, 1.00f };

		colors[ImGuiCol_Button] = ImVec4{ 0.28f, 0.10f, 0.10f, 1.00f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.55f, 0.15f, 0.15f, 1.00f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.75f, 0.20f, 0.20f, 1.00f };

		colors[ImGuiCol_Header] = ImVec4{ 0.28f, 0.10f, 0.10f, 1.00f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.55f, 0.15f, 0.15f, 1.00f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.75f, 0.20f, 0.20f, 1.00f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.22f, 0.08f, 0.08f, 1.00f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.55f, 0.15f, 0.15f, 1.00f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.75f, 0.20f, 0.20f, 1.00f };

		colors[ImGuiCol_CheckMark] = ImVec4{ 1.00f, 0.25f, 0.25f, 1.00f };
		colors[ImGuiCol_SliderGrab] = ImVec4{ 0.85f, 0.20f, 0.20f, 1.00f };
		colors[ImGuiCol_SliderGrabActive] = ImVec4{ 1.00f, 0.35f, 0.35f, 1.00f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.10f, 0.03f, 0.03f, 1.00f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.35f, 0.08f, 0.08f, 1.00f };
		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.18f, 0.05f, 0.05f, 1.00f };

		colors[ImGuiCol_Separator] = ImVec4{ 0.55f, 0.12f, 0.12f, 1.00f };
		colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.85f, 0.20f, 0.20f, 1.00f };
		colors[ImGuiCol_SeparatorActive] = ImVec4{ 1.00f, 0.35f, 0.35f, 1.00f };

		colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.80f, 0.20f, 0.20f, 0.20f };
		colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.80f, 0.20f, 0.20f, 0.60f };
		colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.80f, 0.20f, 0.20f, 0.90f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.80f, 0.20f, 0.20f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.285f, 0.29f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };

		colors[ImGuiCol_DockingPreview] = ImVec4{ 0.80f, 0.20f, 0.20f, 0.70f };
		colors[ImGuiCol_DockingEmptyBg] = ImVec4{ 0.10f, 0.105f, 0.11f, 1.0f };

		colors[ImGuiCol_PlotLines] = ImVec4{ 0.61f, 0.61f, 0.61f, 1.0f };
		colors[ImGuiCol_PlotLinesHovered] = ImVec4{ 0.80f, 0.20f, 0.20f, 1.0f };
		colors[ImGuiCol_PlotHistogram] = ImVec4{ 0.80f, 0.20f, 0.20f, 1.0f };
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4{ 0.95f, 0.30f, 0.30f, 1.0f };

		colors[ImGuiCol_TableHeaderBg] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TableBorderStrong] = ImVec4{ 0.05f, 0.05f, 0.05f, 1.0f };
		colors[ImGuiCol_TableBorderLight] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TableRowBg] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.0f };
		colors[ImGuiCol_TableRowBgAlt] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.03f };

		colors[ImGuiCol_DragDropTarget] = ImVec4{ 0.80f, 0.20f, 0.20f, 0.90f };
		colors[ImGuiCol_NavHighlight] = ImVec4{ 0.80f, 0.20f, 0.20f, 1.0f };
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.70f };
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4{ 0.20f, 0.20f, 0.20f, 0.20f };
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4{ 0.20f, 0.20f, 0.20f, 0.35f };
	}

	void ImGuiLayer::SetBlueThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = ImVec4{ 1.00f, 0.90f, 1.00f, 1.00f };
		colors[ImGuiCol_TextDisabled] = ImVec4{ 0.65f, 0.45f, 0.65f, 1.00f };

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.14f, 0.04f, 0.14f, 1.00f };
		colors[ImGuiCol_ChildBg] = ImVec4{ 0.14f, 0.04f, 0.14f, 1.00f };
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.17f, 0.05f, 0.17f, 1.00f };

		colors[ImGuiCol_Border] = ImVec4{ 0.55f, 0.12f, 0.55f, 0.70f };
		colors[ImGuiCol_BorderShadow] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f };

		colors[ImGuiCol_FrameBg] = ImVec4{ 0.27f, 0.09f, 0.27f, 1.00f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.45f, 0.15f, 0.45f, 1.00f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.60f, 0.18f, 0.60f, 1.00f };

		colors[ImGuiCol_Button] = ImVec4{ 0.27f, 0.09f, 0.27f, 1.00f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.55f, 0.15f, 0.55f, 1.00f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.75f, 0.20f, 0.75f, 1.00f };

		colors[ImGuiCol_Header] = ImVec4{ 0.27f, 0.09f, 0.27f, 1.00f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.55f, 0.15f, 0.55f, 1.00f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.75f, 0.20f, 0.75f, 1.00f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.21f, 0.07f, 0.21f, 1.00f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.55f, 0.15f, 0.55f, 1.00f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.75f, 0.20f, 0.75f, 1.00f };

		colors[ImGuiCol_CheckMark] = ImVec4{ 1.00f, 0.30f, 1.00f, 1.00f };
		colors[ImGuiCol_SliderGrab] = ImVec4{ 0.85f, 0.20f, 0.85f, 1.00f };
		colors[ImGuiCol_SliderGrabActive] = ImVec4{ 1.00f, 0.40f, 1.00f, 1.00f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.10f, 0.03f, 0.10f, 1.00f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.35f, 0.08f, 0.35f, 1.00f };
		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.17f, 0.05f, 0.17f, 1.00f };

		colors[ImGuiCol_Separator] = ImVec4{ 0.55f, 0.12f, 0.55f, 1.00f };
		colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.85f, 0.20f, 0.85f, 1.00f };
		colors[ImGuiCol_SeparatorActive] = ImVec4{ 1.00f, 0.40f, 1.00f, 1.00f };

		colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.10f, 0.30f, 0.90f, 0.20f };
		colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.10f, 0.30f, 0.90f, 0.60f };
		colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.10f, 0.30f, 0.90f, 0.90f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.10f, 0.30f, 0.90f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.285f, 0.29f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.20f, 0.205f, 0.21f, 1.0f };

		colors[ImGuiCol_DockingPreview] = ImVec4{ 0.10f, 0.30f, 0.90f, 0.70f };
		colors[ImGuiCol_DockingEmptyBg] = ImVec4{ 0.10f, 0.105f, 0.11f, 1.0f };

		colors[ImGuiCol_PlotLines] = ImVec4{ 0.61f, 0.61f, 0.61f, 1.0f };
		colors[ImGuiCol_PlotLinesHovered] = ImVec4{ 0.10f, 0.30f, 0.90f, 1.0f };
		colors[ImGuiCol_PlotHistogram] = ImVec4{ 0.10f, 0.30f, 0.90f, 1.0f };
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4{ 0.20f, 0.40f, 1.00f, 1.0f };

		colors[ImGuiCol_TableHeaderBg] = ImVec4{ 0.15f, 0.155f, 0.16f, 1.0f };
		colors[ImGuiCol_TableBorderStrong] = ImVec4{ 0.05f, 0.05f, 0.05f, 1.0f };
		colors[ImGuiCol_TableBorderLight] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TableRowBg] = ImVec4{ 0.00f, 0.00f, 0.00f, 0.0f };
		colors[ImGuiCol_TableRowBgAlt] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.03f };

		colors[ImGuiCol_DragDropTarget] = ImVec4{ 0.10f, 0.30f, 0.90f, 0.90f };
		colors[ImGuiCol_NavHighlight] = ImVec4{ 0.10f, 0.30f, 0.90f, 1.0f };
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4{ 1.00f, 1.00f, 1.00f, 0.70f };
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4{ 0.20f, 0.20f, 0.20f, 0.20f };
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4{ 0.20f, 0.20f, 0.20f, 0.35f };
	}

	void ImGuiLayer::SetTheme(ImGuiTheme theme)
	{
		m_CurrentTheme = theme;
		switch (theme)
		{
		case ImGuiTheme::Dark:    SetDarkThemeColors();    break;
		case ImGuiTheme::Light:   SetLightThemeColors();   break;
		case ImGuiTheme::Classic: SetClassicThemeColors(); break;
		case ImGuiTheme::Cyan:    SetCyanThemeColors();    break;
		case ImGuiTheme::Magenta: SetMagentaThemeColors(); break;
		case ImGuiTheme::Red:     SetRedThemeColors();     break;
		case ImGuiTheme::Blue:    SetBlueThemeColors();    break;
		}
	}

	void ImGuiLayer::OnLayerAttach() {
		ENGINE_PROFILER_FUNCTION();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		SetTheme(m_CurrentTheme);

		io.FontDefault = io.Fonts->AddFontFromFileTTF(FONT_PATH "Ubuntu/Ubuntu-Regular.ttf", 16);
		io.Fonts->AddFontFromFileTTF(FONT_PATH "Ubuntu/Ubuntu-Bold.ttf", 16);

		// Setup scaling
		ImGuiStyle& style = ImGui::GetStyle();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnLayerDetach() {
		ENGINE_PROFILER_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin() {
		ENGINE_PROFILER_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End() {
		ENGINE_PROFILER_FUNCTION();
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	};

	void ImGuiLayer::OnImGuiDraw() {

	}

	void ImGuiLayer::OnLayerEvent(Event& event)
	{
		if (m_BlockImGuiEvents) {
			ImGuiIO& io = ImGui::GetIO();
			event.Handled |= event.IsInCategory(CategoryMouse) & io.WantCaptureMouse;
			event.Handled |= event.IsInCategory(CategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}
}