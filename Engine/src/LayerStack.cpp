#include "LayerStack.h"

namespace Monsi {

	LayerStack::LayerStack() {
		m_LayerInsert = m_LayerVector.begin();
	}

	LayerStack::~LayerStack() {
		for (Layer* Layer : m_LayerVector) {
			delete Layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		m_LayerInsert = m_LayerVector.emplace(m_LayerInsert, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		m_LayerVector.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_LayerVector.begin(), m_LayerVector.end(), layer);
		if (it != m_LayerVector.end()) {
			m_LayerVector.erase(it);
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(m_LayerVector.begin(), m_LayerVector.end(), overlay);
		if (it != m_LayerVector.end()) {
			m_LayerVector.erase(it);
		}
	}

}