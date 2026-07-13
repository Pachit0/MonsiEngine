#include <Monsi.h>
#include <Core/EntryPoint.h>
#include "Sandbox3D.h"

class SandboxApp3D : public Monsi::Application {
public:
	SandboxApp3D()
		: Application({"Sandbox3D", Monsi::RenderTypeEnum::Renderer3D, 1600, 900 }) {
		PushLayer(new Sandbox3D());
	}

	~SandboxApp3D() override = default;

};

Monsi::Application* Monsi::CreateApplication() {
	return new SandboxApp3D();
}