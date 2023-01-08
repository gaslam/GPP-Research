#pragma once
#include "Exam_HelperStructs.h"
#include <vector>
#include "EBlackboard.h"

using namespace Elite;
class HouseManager
{
public:
	HouseManager() = default;

	enum class Status
	{
		found,
		beingVisited,
		visited
	};

	bool Add(HouseInfo& houseInfo);
	void Update(float dt, Blackboard* pBlackboard, AgentInfo& agent);
	bool IsAlreadyFound(Vector2& center);
	bool GetNextAvailableHouse(HouseInfo& house);
	int GetAmountOfHousesToVisit() const { return m_AmountOfUnvisitedHouses; };
private:
	float m_TimeToRememberHouse{ 30 };
	int m_AmountOfUnvisitedHouses{};

	struct FoundHouse{
		int houseId{};
		float m_TimePassed{};
		HouseInfo house{};
		Status status{};
	};

	std::vector<FoundHouse> m_FoundHouses{};
};

