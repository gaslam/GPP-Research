#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
#include <vector>

class IExamInterface;
class InventoryManager
{
public:
	InventoryManager() = default;
	bool GrabItem(IExamInterface* pInterface, EntityInfo& entity, ItemInfo& item);
	void UseItem(IExamInterface* pInterface);
	void RemoveItem(IExamInterface* pInterface);
	void SetGrabItem(const bool canGrabItem) { m_GrabItem = canGrabItem; };
	bool CanGrabItem() const { return m_GrabItem; };
	bool PlayerUsesWeapon() const { return m_CanUseWeapon; };
	bool IsFull() const { return m_IsFull; };

	static bool GetItemInfo(IExamInterface* pInterface, EntityInfo& entity, ItemInfo& item);
private:
	bool m_IsFull = false;
	bool m_GrabItem = false;
	bool m_UseItem = false;
	bool m_RemoveItem = false;
	bool m_CanUseWeapon = false;
	UINT m_InventorySlot = 0;
	const int m_TotalSlots = 5;
	std::vector<int> m_WeaponSlots{};
};

