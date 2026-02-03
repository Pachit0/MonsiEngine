#pragma once

#include "Core.h"
#include "Layer.h"

#include <vector>

namespace Monsi {

	class ENGINE_API LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_LayerVector.begin(); }
		std::vector<Layer*>::iterator end() { return m_LayerVector.end(); }
	private:
		std::vector<Layer*> m_LayerVector;
		unsigned int m_LayerInsertIndex = 0;
	};

}