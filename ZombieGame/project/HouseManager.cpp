#include "stdafx.h"
#include "HouseManager.h"
#include "Exam_HelperStructs.h"
#include "EBlackboard.h"
#include <algorithm>



bool HouseManager::Add(HouseInfo& houseInfo)
{
	if (IsAlreadyFound(houseInfo.Center))
	{
		return false;
	}
	FoundHouse foundHouse{};
	foundHouse.house = houseInfo;
	foundHouse.status = Status::found;
	m_FoundHouses.push_back(foundHouse);
	++m_AmountOfUnvisitedHouses;
	return true;
}

void HouseManager::Update(float dt, Blackboard* pBlackboard, AgentInfo& agent)
{

	for (size_t i{}; i < m_FoundHouses.size(); ++i)
	{
		Vector2 minPos{ m_FoundHouses[i].house.Center.x - m_FoundHouses[i].house.Size.x / 2.f, m_FoundHouses[i].house.Center.y - m_FoundHouses[i].house.Size.y / 2.f };
		Vector2 maxPos{ m_FoundHouses[i].house.Center.x + m_FoundHouses[i].house.Size.x / 2.f, m_FoundHouses[i].house.Center.y + m_FoundHouses[i].house.Size.y / 2.f };

		bool isInXRange{agent.Position.x > minPos.x && agent.Position.x < maxPos.x};
		bool isInYRange{ agent.Position.y > minPos.y && agent.Position.y < maxPos.y};

		if (isInXRange && isInYRange && m_FoundHouses[i].status != Status::visited)
		{
			if ((agent.Position - m_FoundHouses[i].house.Center).MagnitudeSquared() <= (3.f * 3.f))
			{
				m_FoundHouses[i].status = Status::visited;
			}
			else if (isInXRange && isInYRange)
			{
				m_FoundHouses[i].status = Status::beingVisited;
			}
		}

		if (m_FoundHouses[i].status != Status::visited)
		{
			continue;
		}
		m_FoundHouses[i].m_TimePassed += dt;
		if (m_FoundHouses[i].m_TimePassed >= m_TimeToRememberHouse)
		{
			m_FoundHouses.erase(m_FoundHouses.begin() + i);
			--m_AmountOfUnvisitedHouses;
		}
	}
}

bool HouseManager::IsAlreadyFound(Vector2& center)
{
	auto it = std::find_if(m_FoundHouses.begin(), m_FoundHouses.end(), [&](FoundHouse house)
		{
			return house.house.Center == center;
		});

	return it != m_FoundHouses.end();
}

bool HouseManager::GetNextAvailableHouse(HouseInfo& house)
{
	for (FoundHouse& foundHouse : m_FoundHouses)
	{
		if (foundHouse.status == Status::beingVisited)
		{
			house = foundHouse.house;
			return true;
		}
	}

	for (FoundHouse& foundHouse: m_FoundHouses)
	{
		if (foundHouse.status == Status::found)
		{
			house = foundHouse.house;
			return true;
		}
	}
	return false;
}
