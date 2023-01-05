#pragma once
#include "stdafx.h"
namespace Elite
{
	bool IsInRect(Vector2& pos, Vector2& bottomRectPos, float width, float height)
	{
		bool isInXRange{ pos.x > bottomRectPos.x && pos.x < bottomRectPos.x + width };
		bool isInYRange{ pos.y > bottomRectPos.y && pos.y < bottomRectPos.y + height };

		return isInXRange && isInYRange;
	}
}