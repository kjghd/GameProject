#include "Camera.h"
#include "GameObject.h"

game::float2 Camera::m_screenResolution = { 1280,720 };

Camera::Camera(GameObject* pOwner)
	:
	pOwner(pOwner),
	m_zoom(64.f),
	m_speed(4.f),
	m_offset{ 0,0 }
{
}

void Camera::OffsetTo(game::float2 destination, float deltaTime)
{
	game::float2 origin{ pOwner ? pOwner->m_location : game::float2{0,0} };

	game::float2 distance = destination - (origin + m_offset);

	m_offset += distance * (m_speed * deltaTime / 1000);
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

game::float2 Camera::ScreenLocToWorldLoc(float screenX, float screenY)
{
	game::float2 origin{ pOwner ? pOwner->m_location : game::float2{0,0} };

	// Loc to Corn (SU)
	game::float2 A{ -screenX, -screenY };
	// WU
	A = {
		PX_to_WU(A.x),
		-PX_to_WU(A.y)
	};

	// Corn to Cam (SU)
	game::float2 B{
		m_screenResolution.x / 2,
		m_screenResolution.y / 2
	};
	// WU
	B = {
		PX_to_WU(B.x),
		-PX_to_WU(B.y)
	};

	// Cam to Cent (WU)
	game::float2 C{ -(origin + m_offset) };

	// Loc to Cent
	game::float2 D{ A + B + C };

	// Cent to Loc
	return -D;
}
game::float2 Camera::WorldLocToScreenLoc(float worldX, float worldY)
{
	game::float2 origin{ pOwner ? pOwner->m_location : game::float2{0,0} };

	// Loc to Cent (WU)
	game::float2 A{ -worldX, -worldY };
	// SU
	A = {
		WU_to_PX(A.x),
		-WU_to_PX(A.y)
	};

	// Cent to Cam (WU)
	game::float2 B{ origin + m_offset };
	// SU
	B = {
		WU_to_PX(B.x),
		-WU_to_PX(B.y)
	};

	// Cam to Corn (SU)
	game::float2 C{
		-m_screenResolution.x / 2,
		-m_screenResolution.y / 2
	};

	// Loc to Corn
	game::float2 D{ A + B + C };

	// Corn to Loc
	return -D;
}
game::rect Camera::WorldTransformToScreenRect(game::float2 location, game::float2 size)
{
	game::float2 bottom_left{
		WorldLocToScreenLoc(
			location.x - size.x / 2,
			location.y - size.y / 2
		)
	};
	game::float2 top_right{
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

float Camera::PX_to_WU(float screenUnit)
{
	return screenUnit / m_zoom;
}
float Camera::WU_to_PX(float worldUnit)
{
	return worldUnit * m_zoom;
}

std::string Camera::Serialise()
{
	std::string str;

	//str += game::DataToString<game::int2>(m_screenResolution);
	//str += game::DataToString<game::float2>(m_origin);
	//str += game::DataToString<game::float2>(m_offset);
	//str += game::DataToString<float>(m_zoom);
	//str += game::DataToString<float>(m_speed);

	return str;
}