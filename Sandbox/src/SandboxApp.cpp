#include <Monsi.h>
#include "imgui.h"

class ExampleLayer : public Monsi::Layer {
public:
	ExampleLayer() : Layer("Hello!") {
		
	}

	void OnLayerUpdate() override {

		if (Monsi::Input::KeyPressed(MONSI_KEY_TAB)) {
			ENGINE_LOG_TRACE("TAB HAS BEEN PRESSED!");
		}

	}

	void OnLayerEvent(Monsi::Event& event) override {
		//CLIENT_LOG_TRACE("{0}", event);
	}
	
	void OnImGuiDraw() override {
		ImGui::Begin("TEST");
		ImGui::Text("Hello world");
		ImGui::End();
	}
	
};

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