#include <Monsi.h>

class Sandbox : public Monsi::Application {
public:
	Sandbox() {

	}

	~Sandbox() override = default;

};

std::unique_ptr<Monsi::Application> Monsi::CreateApplication() {
	return std::make_unique<Sandbox>();
}