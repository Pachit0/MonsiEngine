#include <Monsi.h>
#include <Core/EntryPoint.h>
#include "GameLayer.h"

class GameApp : public Monsi::Application {
public:
	GameApp()
		: Application({ "Base Monsi Game", Monsi::RenderTypeEnum::Renderer3D, 1280, 720 })
	{
		PushLayer(new GameLayer());
	}

	~GameApp() override = default;
};

Monsi::Application* Monsi::CreateApplication() {
	return new GameApp();
}