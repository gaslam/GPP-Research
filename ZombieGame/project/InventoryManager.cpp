#include "stdafx.h"
#include "InventoryManager.h"
#include "../inc/IExamInterface.h"
#include <algorithm> 

using namespace std;


void InventoryManager::GrabItem(IExamInterface* pInterface, EntityInfo& entity, ItemInfo& item)
{
			//Once grabbed, you can add it to a specific inventory slot
			//Slot must be empty
			if (pInterface->Item_Grab(entity, item))
			{
				if (item.Type == eItemType::PISTOL || item.Type == eItemType::SHOTGUN)
				{
					m_CanUseWeapon = true;
					m_WeaponSlots.push_back(m_InventorySlot);
				}
				pInterface->Inventory_AddItem(m_InventorySlot, item);
			}
}

ItemInfo InventoryManager::ReturnPlayerSelectedItemInfo(IExamInterface* pInterface)
{
	ItemInfo info{};
	pInterface->Inventory_GetItem(m_InventorySlot, info);
	return info;
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
	}
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
