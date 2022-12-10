#include "Camera.h"

game::Int2 Camera::m_screenResolution = { 640,480 };

Camera::Camera()
	:
	m_zoom(64.f),
	m_speed(6.f),
	m_location({ 0,0 })
{
}

void Camera::Move(game::Float2 amount, float deltaTime)
{
	m_location = m_location + amount * (m_speed * deltaTime / 1000);
}
void Camera::MoveTo(game::Float2 destination, float deltaTime)
{
	game::Float2 distance{ destination - m_location };

	m_location += distance * (m_speed * deltaTime / 1000);
}

void Camera::SetZoom(float amount)
{
	m_zoom = amount;
}
void Camera::DecreaseZoom(float factor)
{
	m_zoom -= m_zoom * factor;
}
void Camera::IncreaseZoom(float factor)
{
	m_zoom += m_zoom * factor;
}

float Camera::GetZoom()
{
	return m_zoom;
}

game::Float2 Camera::ScreenLocToWorldLoc(float screenX, float screenY)
{
	// Loc to Corn (SU)
	game::Float2 A{ -screenX, -screenY };
	// WU
	A = {
		SU_to_WU(A.x),
		-SU_to_WU(A.y)
	};

	// Corn to Cam (SU)
	game::Float2 B{
		m_screenResolution.x / 2,
		m_screenResolution.y / 2
	};
	// WU
	B = {
		SU_to_WU(B.x),
		-SU_to_WU(B.y)
	};

	// Cam to Cent (WU)
	game::Float2 C{ -m_location };

	// Loc to Cent
	game::Float2 D{ A + B + C };

	// Cent to Loc
	return -D;
}
game::Float2 Camera::WorldLocToScreenLoc(float worldX, float worldY)
{
	// Loc to Cent (WU)
	game::Float2 A{ -worldX, -worldY };
	// SU
	A = {
		WU_to_SU(A.x),
		-WU_to_SU(A.y)
	};

	// Cent to Cam (WU)
	game::Float2 B{ m_location };
	// SU
	B = {
		WU_to_SU(B.x),
		-WU_to_SU(B.y)
	};

	// Cam to Corn (SU)
	game::Float2 C{
		-m_screenResolution.x / 2,
		-m_screenResolution.y / 2
	};

	// Loc to Corn
	game::Float2 D{ A + B + C };

	// Corn to Loc
	return -D;
}
game::Rect Camera::WorldTransformToScreenRect(game::Float2 location, game::Float2 size)
{
	game::Float2 bottom_left{
		WorldLocToScreenLoc(
			location.x - size.x / 2,
			location.y - size.y / 2
		)
	};
	game::Float2 top_right{
		WorldLocToScreenLoc(
			location.x + size.x / 2,
			location.y + size.y / 2
		)
	};

	return {
		bottom_left.x,
		top_right.y,
		top_right.x,
		bottom_left.y
	};
}

float Camera::SU_to_WU(float screenUnit)
{
	return screenUnit / m_zoom;
}
float Camera::WU_to_SU(float worldUnit)
{
	return worldUnit * m_zoom;
}