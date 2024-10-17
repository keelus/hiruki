#ifndef HIRUKI_SCENE_H
#define HIRUKI_SCENE_H

#include "math/vector3.hpp"
#include <memory>

namespace Hiruki {
	class Engine;

	class Scene {
		public:
			class Camera {
				public:
					Camera() : m_Position(), m_Target(Math::Vector3::forward()), m_Up(Math::Vector3::up()) {}

					Math::Vector3 &getPosition() { return m_Position; }
					Math::Vector3 &getTarget() { return m_Target; }
					Math::Vector3 &getUp() { return m_Up; }

					Math::Vector3 getPosition() const { return m_Position; }
					Math::Vector3 getTarget() const { return m_Target; }
					Math::Vector3 getUp() const { return m_Up; }

					void setPosition(const Math::Vector3 &position) {
						m_Position = position;
					}

					void setTarget(const Math::Vector3 &target) {
						m_Target = target;
					}

					void setUp(const Math::Vector3 &up) {
						m_Up = up;
					}
					
				private:
					Math::Vector3 m_Position;
					Math::Vector3 m_Target;
					Math::Vector3 m_Up;
			};

			virtual void setup() = 0;
			virtual void handleEvents(float deltaTime) = 0;
			virtual void update(float deltaTime) = 0;

			const Camera getCamera() const { return m_Camera; }

			std::weak_ptr<Engine> engine;

		protected:
			Camera m_Camera;
	};
}

#endif
