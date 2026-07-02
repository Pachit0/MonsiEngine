#pragma once
#include "glm/glm.hpp"

namespace MonsiColors {

	// === Core Standard Basics ===
	inline constexpr glm::vec3 White{ 1.0f, 1.0f, 1.0f };
	inline constexpr glm::vec3 Black{ 0.0f, 0.0f, 0.0f };
	inline constexpr glm::vec3 Gray{ 0.5f, 0.5f, 0.5f };
	inline constexpr glm::vec3 LightGray{ 0.75f, 0.75f, 0.75f };
	inline constexpr glm::vec3 DarkGray{ 0.25f, 0.25f, 0.25f };

	inline constexpr glm::vec3 Red{ 1.0f, 0.0f, 0.0f };
	inline constexpr glm::vec3 Green{ 0.0f, 1.0f, 0.0f };
	inline constexpr glm::vec3 Blue{ 0.0f, 0.0f, 1.0f };
	inline constexpr glm::vec3 Yellow{ 1.0f, 1.0f, 0.0f };
	inline constexpr glm::vec3 Cyan{ 0.0f, 1.0f, 1.0f };
	inline constexpr glm::vec3 Magenta{ 1.0f, 0.0f, 1.0f };

	// === Extended Rich Palette ===
	inline constexpr glm::vec3 Orange{ 1.0f, 0.647f, 0.0f };
	inline constexpr glm::vec3 Purple{ 0.502f, 0.0f, 0.502f };
	inline constexpr glm::vec3 Pink{ 1.0f, 0.753f, 0.796f };
	inline constexpr glm::vec3 Teal{ 0.0f, 0.502f, 0.502f };
	inline constexpr glm::vec3 Navy{ 0.0f, 0.0f, 0.502f };
	inline constexpr glm::vec3 Olive{ 0.502f, 0.502f, 0.0f };
	inline constexpr glm::vec3 Maroon{ 0.502f, 0.0f, 0.0f };
	inline constexpr glm::vec3 Crimson{ 0.863f, 0.078f, 0.235f };
	inline constexpr glm::vec3 Coral{ 1.0f, 0.498f, 0.314f };
	inline constexpr glm::vec3 Brown{ 0.588f, 0.294f, 0.0f };

	// === Materials & Metals ===
	inline constexpr glm::vec3 Gold{ 1.0f, 0.843f, 0.0f };
	inline constexpr glm::vec3 Silver{ 0.753f, 0.753f, 0.753f };
	inline constexpr glm::vec3 Bronze{ 0.804f, 0.498f, 0.196f };
	inline constexpr glm::vec3 Emerald{ 0.314f, 0.784f, 0.471f };

	// === Cyberpunk / Neon Highlights ===
	inline constexpr glm::vec3 NeonPink{ 1.0f, 0.0f, 0.5f };
	inline constexpr glm::vec3 NeonBlue{ 0.0f, 0.949f, 1.0f };
	inline constexpr glm::vec3 NeonGreen{ 0.224f, 1.0f, 0.078f };
	inline constexpr glm::vec3 NeonYellow{ 0.945f, 1.0f, 0.0f };
	inline constexpr glm::vec3 NeonPurple{ 0.702f, 0.0f, 1.0f };

	// === Soft Pastels ===
	inline constexpr glm::vec3 PastelRed{ 1.0f, 0.416f, 0.416f };
	inline constexpr glm::vec3 PastelBlue{ 0.686f, 0.933f, 0.933f };
	inline constexpr glm::vec3 PastelGreen{ 0.467f, 0.867f, 0.467f };
	inline constexpr glm::vec3 PastelYellow{ 1.0f, 1.0f, 0.6f };
	inline constexpr glm::vec3 Lavender{ 0.902f, 0.902f, 0.98f };
	inline constexpr glm::vec3 Mint{ 0.596f, 0.984f, 0.596f };

	// === Engine UI / Utility Colors ===
	inline constexpr glm::vec3 ClearColor{ 0.1f, 0.1f, 0.12f };
	inline constexpr glm::vec3 GridColor{ 0.3f, 0.3f, 0.3f };
	inline constexpr glm::vec3 SelectedItem{ 0.2f, 0.6f, 1.0f };
	inline constexpr glm::vec3 Warning{ 1.0f, 0.753f, 0.0f };
	inline constexpr glm::vec3 Error{ 0.9f, 0.1f, 0.1f };
	inline constexpr glm::vec3 Success{ 0.1f, 0.8f, 0.1f };
}