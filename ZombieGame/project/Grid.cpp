#include "stdafx.h"
#include "Grid.h"
#include "Utils.h"

using namespace Elite;

Grid::Grid(IExamInterface* pInterface, int rowsAndCols) :
	m_RowsAndCols{rowsAndCols}
{
	auto worldInfo = pInterface->World_GetInfo();
	m_CellWidth = worldInfo.Dimensions.x / rowsAndCols;
	m_CellHeight = worldInfo.Dimensions.y / rowsAndCols;

	m_Width = worldInfo.Dimensions.x;
	m_Height = worldInfo.Dimensions.y;

	Vector2{ worldInfo.Center };
	Vector2{ worldInfo.Center };

	//Add rows and cols
	m_BottomLeftPos =  worldInfo.Center - worldInfo.Dimensions / 2.f;
	for (int row{}; row < rowsAndCols; ++row)
	{
		for (int col{}; col < rowsAndCols; ++col)
		{
			Cell cell{};
			cell.bottom.x = (col % rowsAndCols) * m_CellWidth;
			cell.center.x = cell.bottom.x + m_CellWidth / 2.f;
			cell.bottom.y = (row % rowsAndCols) * m_CellHeight;
			cell.center.y = cell.bottom.y + m_CellHeight / 2.f;
			m_Cells.emplace_back(cell);
		}
	}
}

void Grid::DrawGrid(IExamInterface* pInterface) const
{
	float centerPointRadius{ 2.f };
	const Vector3 visitedGreenColor{ Vector3{0,1,0} };
	const Vector3 unvisitedGrayColor{ Vector3{0.73f,0.72f,0.71f} };
	const float cellWidthDivBy2{ m_CellWidth / 2.f };
	const float cellHeightDivBy2{ m_CellHeight / 2.f };

	for (int i{1}; i < m_RowsAndCols; ++i)
	{
		Vector2 xEndPos{ (i % m_RowsAndCols) * m_CellWidth,0 };
		Vector2 yEndPos{ 0,(i % m_RowsAndCols) * m_CellHeight };
		pInterface->Draw_Segment(m_BottomLeftPos + xEndPos, Vector2{ m_BottomLeftPos.x, m_BottomLeftPos.y + m_Height } + xEndPos, unvisitedGrayColor);
		pInterface->Draw_Segment(m_BottomLeftPos + yEndPos, Vector2{ m_BottomLeftPos.x + m_Width, m_BottomLeftPos.y } + yEndPos, unvisitedGrayColor);
	}

	//DrawRect around grid
	pInterface->Draw_Segment(m_BottomLeftPos, Vector2{ m_BottomLeftPos.x, m_BottomLeftPos.y + m_Height }, unvisitedGrayColor);
	pInterface->Draw_Segment(Vector2{ m_BottomLeftPos.x, m_BottomLeftPos.y + m_Height }, Vector2{ m_BottomLeftPos.x + m_Width, m_BottomLeftPos.y + m_Height }, unvisitedGrayColor);
	pInterface->Draw_Segment(Vector2{m_BottomLeftPos.x + m_Width, m_BottomLeftPos.y + m_Height }, Vector2{m_BottomLeftPos.x + m_Width, m_BottomLeftPos.y}, unvisitedGrayColor);
	pInterface->Draw_Segment(Vector2{m_BottomLeftPos.x + m_Width, m_BottomLeftPos.y}, m_BottomLeftPos, unvisitedGrayColor);


	//DrawRects around visited cells
	for (const Cell& cell : m_Cells)
	{
		if (cell.visited)
		{
			const Vector2 points[4]{
	m_BottomLeftPos + Vector2{cell.center.x - cellWidthDivBy2,cell.center.y - cellHeightDivBy2},
	m_BottomLeftPos + Vector2{cell.center.x - cellWidthDivBy2,cell.center.y + cellHeightDivBy2},
	m_BottomLeftPos + Vector2{cell.center.x + cellWidthDivBy2,cell.center.y + cellHeightDivBy2},
	m_BottomLeftPos + Vector2{cell.center.x + cellWidthDivBy2,cell.center.y - cellHeightDivBy2}
			};
			pInterface->Draw_Polygon(points, 4, visitedGreenColor);
		}
	}
}

void Grid::Update(IExamInterface* pInterface)
{
		AgentInfo agentInfo = pInterface->Agent_GetInfo();
		MarkCellVisited(agentInfo.Position);
}

Vector2 Grid::GetNextAvailableCellPos(Elite::Vector2& agentPos)
{
	float closestDistance{ FLT_MAX };
	Cell closestCell{};
	for (size_t i{}; i < m_Cells.size(); ++i)
	{
		if (m_Cells[i].visited)
		{
			continue;
		}
		const float distanceSqr{ agentPos.DistanceSquared(m_BottomLeftPos + m_Cells[i].center) };
		if (distanceSqr < closestDistance)
		{
			closestDistance = distanceSqr;
			closestCell = m_Cells[i];
		}
	}

	return m_BottomLeftPos + closestCell.center;
}

void Grid::MarkCellVisited(Vector2& agentPos)
{
	for (Cell& cell : m_Cells)
	{
		if (cell.visited)
		{
			continue;
		}

		if (IsInRect(agentPos, m_BottomLeftPos + cell.bottom, m_CellWidth, m_CellHeight))
		{
			cell.visited = true;
		}
	}
}
