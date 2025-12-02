#include "mopch.h"
#include <Monsi.h>

class Sandbox : public Monsi::Application {
public:
	Sandbox() {

	}

	~Sandbox() override = default;

};

Monsi::Application* Monsi::CreateApplication() {
	return new Sandbox();
}