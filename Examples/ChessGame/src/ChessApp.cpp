#include <Monsi.h>
#include <Core/EntryPoint.h>

#include "ChessGame.h"

class ChessApp : public Monsi::Application {
public:
	ChessApp()
		: Application({ "Chess Game" }) {
		PushLayer(new Chess());
	}

	~ChessApp() override = default;

};

Monsi::Application* Monsi::CreateApplication() {
	return new ChessApp();
}