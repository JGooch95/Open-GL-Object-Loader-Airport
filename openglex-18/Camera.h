#pragma once

class Camera
{
	private:
		sf::Vector3f m_Position; //X and Z Position of the camera
		sf::Vector3f m_lookAt; //The Cameras direction.
		sf::Vector3f m_View; //The point the camera looks at

	public:
		Camera();
		void Turn(float xAngle,float yAngle);
		void HandleInput();
		void Point();
};