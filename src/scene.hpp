#ifndef HIRUKI_SCENE_H
#define HIRUKI_SCENE_H

#include <memory>

namespace Hiruki {
	class Engine;

	class Scene {
	public:
		virtual void Setup() = 0;
		virtual void Update(float deltaTime) = 0;

		std::weak_ptr<Engine> engine;
	};
}

#endif
