#pragma once

namespace game
{
	struct Int2
	{
		int x = 0, y = 0;
	};

	struct Float2
	{
		float x = 0, y = 0;

		Float2 operator *(float f)
		{
			return { x * f, y * f };
		}
		Float2 operator /(float f)
		{
			return { x / f, y / f };
		}
		Float2 operator +(float f)
		{
			return { x + f, y + f };
		}

		Float2 operator +(Float2 ff)
		{
			return { x + ff.x, y + ff.y };
		}
		Float2 operator -()
		{
			return { -x, -y };
		}
		Float2 operator -(Float2 ff)
		{
			return { x - ff.x, y - ff.y };
		}
		Float2 operator -(float f)
		{
			return { x - f, y - f };
		}

		bool operator ==(Float2 ff) { return x == ff.x && y == ff.y ? true : false; }

		void operator +=(Float2 ff) { x += ff.x; y += ff.y; };
		void operator -=(Float2 ff) { x -= ff.x; y -= ff.y; };
	};

	struct Rect
	{
		float l, t, r, b;
	};
}
