#include "engine.hpp"
#include "myScene.hpp"
#include <memory>

int main(void) {
	std::shared_ptr<Hiruki::Engine> engine = std::make_shared<Hiruki::Engine>(1800, 1000, 1, 144);
	std::shared_ptr<Hiruki::Scene> scene = std::make_shared<MainScene>();

	engine->setScene(scene);
	engine->run();
}
