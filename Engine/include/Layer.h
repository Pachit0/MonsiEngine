#pragma once
#include "Core.h"
#include "Events.h"

namespace Monsi {

	class ENGINE_API Layer {
	public:
		Layer(const std::string& LayerName = "Default");
		virtual ~Layer();

		virtual void OnLayerAttach() {}
		virtual void OnLayerUpdate() {}
		virtual void OnLayerDetach() {}
		virtual void OnLayerEvent(Event& event) {}

		inline const std::string& GetDebugName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}