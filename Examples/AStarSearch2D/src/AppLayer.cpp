#include <Monsi.h>
#include <Core/EntryPoint.h>

#include "AStarSearch2D.h"

namespace Monsi {

	class AStarSearch : public Application {
	public:
		AStarSearch()
			: Application({ "AStarSearch2D", Monsi::RenderTypeEnum::Renderer2D }) {
			PushLayer(new AStarSearch2D());
		}

		~AStarSearch() override = default;

	};

	Application* CreateApplication() {
		return new AStarSearch();
	}

}