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
		}
		if ((item.Type == eItemType::PISTOL || item.Type == eItemType::SHOTGUN) && itemAdded)
		{
			m_CanUseWeapon = true;
			m_WeaponSlots.push_back(i);
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

void InventoryManager::UseItem(IExamInterface* pInterface)
{
	if (m_UseItem)
	{
		//Use an item (make sure there is an item at the given inventory slot)
		pInterface->Inventory_UseItem(m_InventorySlot);
	}
}

void InventoryManager::RemoveItem(IExamInterface* pInterface)
{
	if (m_RemoveItem)
	{
		//Remove an item from a inventory slot
		pInterface->Inventory_RemoveItem(m_InventorySlot);
		std::remove_if(m_WeaponSlots.begin(), m_WeaponSlots.end(), [&](int slot) {
			return slot == m_InventorySlot;
			});

		m_IsFull = false;
	}
}

bool InventoryManager::GetItemInfo(IExamInterface* pInterface, EntityInfo& entity, ItemInfo& item)
{
	if (pInterface->Item_GetInfo(entity, item))
	{
		return true;
	}
	return false;
}


//	if (m_InventorySlot > 0)
//	{
//		m_CanUseWeapon = false;
//		--m_InventorySlot;
//		for (int slot : m_WeaponSlots)
//		{
//			if (slot == m_InventorySlot)
//			{
//				m_CanUseWeapon = true;
//			}
//		}
//		std::cout << "Current selected slot: SLOT(" << m_InventorySlot << ")\n";
//	}
//}
//else if (pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Plus))
//{
//	if (m_InventorySlot < m_TotalSlots)
//	{
//		m_CanUseWeapon = false;
//		++m_InventorySlot;
//		for (int slot : m_WeaponSlots)
//		{
//			if (slot == m_InventorySlot)
//			{
//				m_CanUseWeapon = true;
//			}
//		}
//		std::cout << "Current selected slot: SLOT(" << m_InventorySlot << ")\n";
//	}
//}
