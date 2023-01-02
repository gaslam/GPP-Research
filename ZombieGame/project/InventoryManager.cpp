#include "stdafx.h"
#include "InventoryManager.h"
#include "../inc/IExamInterface.h"


void InventoryManager::Update(IExamInterface* pInterface)
{
	HandleControls(pInterface);
}

ItemInfo InventoryManager::ReturnItemInfo(IExamInterface* pInterface)
{
	ItemInfo info = {};
	pInterface->Inventory_GetItem(m_InventorySlot, info);
	return info;
}

void InventoryManager::HandleControls(IExamInterface* pInterface)
{
	if (pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_G))
	{
		m_GrabItem = true;
	}
	else if (pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_U))
	{
		m_UseItem = true;
	}
	else if (pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_R))
	{
		m_RemoveItem = true;
	}
	else if (pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Q))
	{
		ItemInfo info = ReturnItemInfo(pInterface);
		std::cout << (int)info.Type << std::endl;
	}
	else if (pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Minus))
	{
		if (m_InventorySlot > 0)
		{
			--m_InventorySlot;
			std::cout << "Current selected slot: SLOT(" << m_InventorySlot << ")\n";
		}
	}
	else if (pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Plus))
	{
		if (m_InventorySlot < m_TotalSlots)
		{
			++m_InventorySlot;
			std::cout << "Current selected slot: SLOT(" << m_InventorySlot << ")\n";
		}
	}
}