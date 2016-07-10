#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	m_Position = sf::Vector3f(250.0f, 10.0f, -90.0f); //Position of the camera
	m_lookAt = sf::Vector3f(250.0f, 10.0f, -91.0f); //The Cameras direction.
	m_View = sf::Vector3f(250.0f, 10.0f, -91.0f); //The point the camera looks at
}

void Camera::Point()
{
	gluLookAt(m_Position.x, m_Position.y,m_Position.z, /* The position the camera is at*/ 
			  m_View.x, m_View.y, m_View.z, /* The point to look at */ 
			 0.0f, 1.0f, 0.0f); /* positive Y up vector */
}

void Camera::Turn(float xAngle,float yAngle) 
{
	 sf::Vector3f ViewDir = m_View - m_Position; 
	 
	 //Rotates the camera by the amount given
	 m_lookAt.x = (cosf(-xAngle) * ViewDir.x) - (sinf(-xAngle) * ViewDir.z); 
	 m_lookAt.z = (sinf(-xAngle) * ViewDir.x) + (cosf(-xAngle) * ViewDir.z);

	 //Adds the position to the rotation so the camera is positioned correctly
	 m_View.x = m_Position.x + m_lookAt.x;
	 m_View.y += yAngle * 2.0f;
	 m_View.z = m_Position.z + m_lookAt.z;
}

void Camera::HandleInput() 
{
	float speed = 1.0f;//The rate the camera moves at.

	//Forwards / Backwards
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) //If the UP or W key is pressed
	{
		//The position is moved forward from the camera
		m_Position.x += m_lookAt.x * speed;
		m_Position.z += m_lookAt.z * speed;

		//The point being looked at is moved forward from the camera
		m_View.x  += m_lookAt.x * speed;
		m_View.z  += m_lookAt.z * speed;
		
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))//If the DOWN or S key is pressed
	{
		//The position is moved backward from the camera
		m_Position.x -= m_lookAt.x * speed ;
		m_Position.z -= m_lookAt.z * speed;

		//The point being looked at is moved backward from the camera
		m_View.x  -= m_lookAt.x * speed;
		m_View.z  -= m_lookAt.z * speed;
	}

	//Strafing Left / Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) //If the LEFT or A key is pressed
	{
		//The position is moved left from the camera
		m_Position.x -= (-m_lookAt.z * speed) ;
		m_Position.z -= (m_lookAt.x * speed) ;

		//The point being looked at is moved left from the camera
		m_View.x  -=  (-m_lookAt.z * speed) ;
		m_View.z  -= (m_lookAt.x * speed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) //If the RIGHT or D key is pressed
	{
		//The position is moved right from the camera
		m_Position.x += (-m_lookAt.z * speed);
		m_Position.z += (m_lookAt.x * speed);

		//The point being looked at is moved right from the camera
		m_View.x  +=  (-m_lookAt.z * speed);
		m_View.z  += (m_lookAt.x * speed);
	}

	//Strafing Up / Down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) //If the space key is pressed
	{
		//The position is moved up from the camera
		m_Position.y +=  speed;

		//The point being looked at is moved up from the camera
		m_View.y  +=   speed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) //If the left control is pressed
	{
		//The position is moved down from the camera
		m_Position.y -= speed;

		//The point being looked at is moved down from the camera
		m_View.y -= speed;
	}
	
}