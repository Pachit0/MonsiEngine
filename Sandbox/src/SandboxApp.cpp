#include <MonsiPch.h>
#include <Monsi.h>

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
};

class Sandbox : public Monsi::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Monsi::ImGuiLayer());
	}

	~Sandbox() override = default;

};

Monsi::Application* Monsi::CreateApplication() {
	return new Sandbox();
}