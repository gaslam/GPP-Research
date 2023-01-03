#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
#include <vector>

class IExamInterface;
class InventoryManager
{
public:
	InventoryManager() = default;
	void GrabItem(IExamInterface* pInterface, EntityInfo& entity, ItemInfo& item);
	void UseItem(IExamInterface* pInterface);
	void RemoveItem(IExamInterface* pInterface);
	void SetUseItem(const bool canUseItem) { m_UseItem = canUseItem; };
	void SetGrabItem(const bool canGrabItem) { m_GrabItem = canGrabItem; };
	void SetRemoveItem(const bool canRemoveItem) { m_RemoveItem = canRemoveItem; };
	void Update(IExamInterface* pInterface);
	bool CanGrabItem() const { return m_GrabItem; };
	bool CanUseItem() const { return m_UseItem; };
	bool CanRemoveItem() const { return m_RemoveItem; };
	bool PlayerUsesWeapon() const { return m_CanUseWeapon; };
	UINT GetInventorySlot() const { return m_InventorySlot; };

	ItemInfo ReturnPlayerSelectedItemInfo(IExamInterface* pInterface);
private:
	void HandleControls(IExamInterface* pInterface);
	bool m_GrabItem = false;
	bool m_UseItem = false;
	bool m_RemoveItem = false;
	bool m_CanUseWeapon = false;
	UINT m_InventorySlot = 0;
	const int m_TotalSlots = 4;
	std::vector<int> m_WeaponSlots{};
};

