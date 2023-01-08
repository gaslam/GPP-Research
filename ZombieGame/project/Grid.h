#pragma once
#include "stdafx.h"
#include "Exam_HelperStructs.h"
#include "IExamInterface.h"
#include <vector>

class Grid final
{
public:
	Grid(IExamInterface* pInterface, int rowsAndCols = 50);
	void DrawGrid(IExamInterface* pInterface) const;
	void Update(IExamInterface* pInterface, float dt);
	Elite::Vector2 GetNextAvailableCellPos(Elite::Vector2& agentPos);
private:
	float m_TimeToBecomeUnvisitedAgain{60.f};
	struct Cell {
		Elite::Vector2 center{};
		Elite::Vector2 bottom{};
		float currentTimePassed{};
		bool visited = false;
	};
	int m_RowsAndCols;
	float m_Height{};
	float m_Width{};
	float m_CellHeight{};
	float m_CellWidth{};

	Elite::Vector2 m_BottomLeftPos;

	std::vector<Cell>m_Cells{};

	void MarkCellVisited(Elite::Vector2& agentPos);
};