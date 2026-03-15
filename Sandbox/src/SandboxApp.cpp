#include <Monsi.h>
#include <Core/EntryPoint.h>
#include "ExampleLayer.h"
#include "Sandbox2D.h"

class Sandbox : public Monsi::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() override = default;

};

Monsi::Application* Monsi::CreateApplication() {
	return new Sandbox();
}