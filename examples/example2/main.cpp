#include "engine.hpp"
#include "myScene.hpp"
#include <memory>

int main(void) {
	std::shared_ptr<Hiruki::Engine> engine = std::make_shared<Hiruki::Engine>(1920/2, 1080/2, 1, 144);
	std::shared_ptr<Hiruki::Scene> scene = std::make_shared<MainScene>();

	// engine->setFpsLimit(144);
	engine->enableRasterOptimizations(6);

	engine->setScene(scene);
	engine->run();
}
