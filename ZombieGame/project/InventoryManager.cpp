#include "stdafx.h"
#include "InventoryManager.h"
#include "../inc/IExamInterface.h"
#include <algorithm> 

using namespace std;

bool InventoryManager::GrabItem(IExamInterface* pInterface, EntityInfo& entity, ItemInfo& item)
{
	if (m_IsFull || !pInterface->Item_Grab(entity, item))
	{
		return false;
	}
	for (int i{}; i < m_TotalSlots; ++i)
	{
		bool itemAdded{ false };
		if (pInterface->Inventory_AddItem(i, item))
		{
			m_GrabItem = false;
			itemAdded = true;
			++m_AmountOfItems;
		}
		if (itemAdded)
		{
			if (i == m_TotalSlots - 1)
			{
				m_IsFull = true;
			}
			return true;
		}
	}

	return false;
}

bool InventoryManager::UseItem(IExamInterface* pInterface, int slot)
{
	//Use an item (make sure there is an item at the given inventory slot)
	return pInterface->Inventory_UseItem(slot);
}

bool InventoryManager::RemoveItem(IExamInterface* pInterface, int slot)
{
	//Remove an item from a inventory slot
	if (!pInterface->Inventory_RemoveItem(slot))
	{
		return false;
	}
	--m_AmountOfItems;
	m_IsFull = false;
	return true;
}

bool InventoryManager::GetItem(IExamInterface* pInterface, eItemType type, ItemInfo& item, int& slot)
{
	for (int i{}; i < m_TotalSlots; ++i)
	{
		ItemInfo currentItem{};
		if (pInterface->Inventory_GetItem(i, currentItem) && currentItem.Type == type)
		{
			item = currentItem;
			slot = i;
			return true;
		}
	}
	return true;
}

bool InventoryManager::GetItemInfo(IExamInterface* pInterface, EntityInfo& entity, ItemInfo& item)
{
	if (pInterface->Item_GetInfo(entity, item))
	{
		return true;
	}
	return false;
}