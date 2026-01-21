#include "MonsiPch.h"
#include "LayerStack.h"

namespace Monsi {

	LayerStack::LayerStack() {
		m_LayerVector.begin();
	}

	LayerStack::~LayerStack() {
		for (Layer* Layer : m_LayerVector) {
			delete Layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		m_LayerVector.emplace(m_LayerVector.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		m_LayerVector.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_LayerVector.begin(), m_LayerVector.end(), layer);
		if (it != m_LayerVector.end()) {
			m_LayerVector.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(m_LayerVector.begin(), m_LayerVector.end(), overlay);
		if (it != m_LayerVector.end()) {
			m_LayerVector.erase(it);
		}
	}

}