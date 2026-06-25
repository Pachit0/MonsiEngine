#include <Monsi.h>
#include <Core/EntryPoint.h>
#include "Sandbox2D.h"

class SandboxApp2D : public Monsi::Application {
public:
	SandboxApp2D()
		: Application({ "Sandbox2D", Monsi::RenderTypeEnum::Renderer2D }) {
		PushLayer(new Sandbox2D());
	}

	~SandboxApp2D() override = default;

};

Monsi::Application* Monsi::CreateApplication() {
	return new SandboxApp2D();
}