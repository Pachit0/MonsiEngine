#include <MonsiPch.h>
#include <Monsi.h>

class ExampleLayer : public Monsi::Layer {
public:
	ExampleLayer() : Layer("Hello Mommy <3") {
	
	}

	void OnLayerUpdate() override {
		CLIENT_LOG_INFO("ExampleLayer::Update");
	}

	void OnLayerEvent(Monsi::Event& event) override {
		CLIENT_LOG_TRACE("{0}", event);
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