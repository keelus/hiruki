#include "engine.hpp"
#include "myScene.hpp"
#include <memory>

int main(void) {
	std::shared_ptr<Hiruki::Engine> engine = std::make_shared<Hiruki::Engine>(400, 400, 2, 144);
	std::shared_ptr<Hiruki::Scene> scene = std::make_shared<MainScene>();

	engine->setFpsLimit(144);
	engine->enableRasterOptimizations(6);

	engine->setScene(scene);
	engine->run();
}
