#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
class IExamInterface;
class InventoryManager
{
public:
	InventoryManager() = default;
	void SetUseItem(const bool canUseItem) { m_UseItem = canUseItem; };
	void SetGrabItem(const bool canGrabItem) { m_GrabItem = canGrabItem; };
	void SetRemoveItem(const bool canRemoveItem) { m_RemoveItem = canRemoveItem; };
	void Update(IExamInterface* pInterface);
	bool CanGrabItem() const { return m_GrabItem; };
	bool CanUseItem() const { return m_UseItem; };
	bool CanRemoveItem() const { return m_RemoveItem; };
	UINT GetInventorySlot() const { return m_InventorySlot; };

	ItemInfo ReturnItemInfo(IExamInterface* pInterface);
private:
	void HandleControls(IExamInterface* pInterface);
	bool m_GrabItem = false;
	bool m_UseItem = false;
	bool m_RemoveItem = false;
	UINT m_InventorySlot = 0;
	const int m_TotalSlots = 4;
};

