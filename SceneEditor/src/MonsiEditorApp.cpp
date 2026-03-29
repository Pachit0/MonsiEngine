#include <Monsi.h>
#include <Core/EntryPoint.h>

#include "MonsiEditorLayer.h"

namespace Monsi {

	class MonsiEditor : public Application {
	public:
		MonsiEditor() {
			PushLayer(new EditorLayer());
		}

		~MonsiEditor() override = default;

	};

	Application* CreateApplication() {
		return new MonsiEditor();
	}

}