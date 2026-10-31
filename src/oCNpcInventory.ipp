namespace GOTHIC_NAMESPACE 
{
    oCItem* oCNpcInventory::IsInInvNotEquipped(const zSTRING& instance, int amount, zList<oCItem>& itemsToBeEquipped)
    {
        auto itemIndex = parser->GetIndex(instance);
        if (itemIndex <= 0)
        {
            return nullptr;
        }

        this->UnpackAllItems();

    #if ENGINE <= Engine_G1A
        for (int cat = 0; cat < INV_MAX; cat++)
        {
            auto invList = this->inventory[cat].next;
    #else
        auto invList = this->inventory.next;
    #endif

        while (invList)
        {
            auto item = invList->GetData();
            invList = invList->next;
            
            if (item->instanz != itemIndex)
            {
                continue;
            }

            if (item->HasFlag(ITM_FLAG_ACTIVE))
            {
                continue;
            }

            if (item->amount < amount)
            {
                continue;
            }

            if (itemsToBeEquipped.IsIn(item))
            {
                continue;
            }

            return item;
        }

    #if ENGINE <= Engine_G1A
        }
    #endif
        return nullptr;
    }
}