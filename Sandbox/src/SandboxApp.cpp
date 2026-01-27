#include <Monsi.h>
#include <Core/EntryPoint.h>
#include "ExampleLayer.h"

class Sandbox : public Monsi::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() override = default;

};

Monsi::Application* Monsi::CreateApplication() {
	return new Sandbox();
}