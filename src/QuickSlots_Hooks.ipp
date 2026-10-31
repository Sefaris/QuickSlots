namespace GOTHIC_NAMESPACE 
{
    void __fastcall oCSpell_EndTimedEffect_RestorePlayer(::Union::Registers& reg)
    {
        auto npc = *reinterpret_cast<oCNpc**>(zSwitch(reg.ebp, reg.ebp, reg.esi, reg.esi) + 0x40);
        if (!npc)
        {
            return;
        }

        if(npc != player)
        {
            return;
        }

        if (player->HasBodyStateModifier(BS_MOD_TRANSFORMED) || player->HasBodyStateModifier(BS_MOD_CONTROLLED) || player->HasBodyStateModifier(BS_MOD_CONTROLLING))
        {
            return;
        }

        quickSlots->RefreshSpellBook();
    }
    auto PartialHook_oCSpell_EndTimedEffect_RestorePlayer = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x0047f8c7, 0x0048a484, 0x00485c3a, 0x0048725a)), &oCSpell_EndTimedEffect_RestorePlayer);

    void __fastcall oCNpc_RemoveItem(::Union::Registers& reg)
    {
        auto npc = reinterpret_cast<oCNpc*>(reg.edi);
        if (!npc)
        {
            return;
        }

        if(npc != player)
        {
            return;
        }

        if (player->HasBodyStateModifier(BS_MOD_TRANSFORMED) || player->HasBodyStateModifier(BS_MOD_CONTROLLED) || player->HasBodyStateModifier(BS_MOD_CONTROLLING))
        {
            return;
        }

        quickSlots->RefreshSpellBook();
    }
    auto PartialHook_oCNpc_RemoveFromInv_1 = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x006a52a1, 0x006d7d81, 0x006eab21, 0x007495e1)), &oCNpc_RemoveItem);
    auto PartialHook_oCNpc_RemoveFromInv_2 = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x006a52ee, 0x006d7dce, 0x006eab6e, 0x0074962e)), &oCNpc_RemoveItem);
    auto PartialHook_oCNpc_RemoveFromInv_3 = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x006a5349, 0x006d7e29, 0x006eabc9, 0x00749689)), &oCNpc_RemoveItem);
    auto PartialHook_oCNpc_DoRemoveFromInventory = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x006a1650, 0x006d3de0, 0x006e68b0, 0x00745340)), &oCNpc_RemoveItem);

    void __fastcall oCNpc_Equip(::Union::Registers& reg)
    {
        auto npc = reinterpret_cast<oCNpc*>(reg.edi);
        if(npc != player)
        {
            return;
        }

        auto item = reinterpret_cast<oCItem*>(reg.ebp);

        auto itemCat = player->inventory2.GetCategory(item);
        auto itemActive = item->HasFlag(ITM_FLAG_ACTIVE);

        auto SlotNr = quickSlots->GetBestFreeSlot();

        if(itemCat == INV_COMBAT)
        {
            if(!itemActive && !quickSlots->IsRegistered(item))
            {
                quickSlots->ClearSpellBook();
                quickSlots->RegisterItem(item, SlotNr);
                quickSlots->RestoreSpellBook();
            }
        }
        else if(itemCat == INV_RUNE)
        {
            if(!itemActive && !quickSlots->IsRegistered(item))
            {
                quickSlots->ClearSpellBook();
                quickSlots->RegisterItem(item, SlotNr);
                quickSlots->RestoreSpellBook();
            }
            reg.eip = zSwitch(0x00696c13, 0x006c875a, 0x006db866, 0x0073a026);
        }
    }
    auto PartialHook_oCNpc_Equip = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x000696919, 0x006c8469, 0x006db579, 0x00739cb7)), &oCNpc_Equip);

    void __fastcall oCItemContainer_Draw(::Union::Registers& reg)
    {
        if (player->HasBodyStateModifier(BS_MOD_TRANSFORMED) || player->HasBodyStateModifier(BS_MOD_CONTROLLED) || player->HasBodyStateModifier(BS_MOD_CONTROLLING))
        {
            return;
        }

        auto container = reinterpret_cast<oCItemContainer*>(zSwitch(reg.esi, reg.ebp, reg.ebp, reg.ebp));

        if (!container->right)
        {
            return;
        }

        auto textView = container->textView;
        if (!textView)
        {
            return;
        }

        auto item = reinterpret_cast<oCItem*>(zSwitch(reg.ebx, reg.esi, reg.ebx, reg.ebx));
        auto hotkey = quickSlots->GetSlotNumberFromItem(item);

        auto canPrintHotkey = [&hotkey, item]() -> bool
        {
            if (hotkey == -1)
            {
                return false;
            }

            auto melee = player->GetEquippedMeleeWeapon();
            auto ranged = player->GetEquippedRangedWeapon();

            if (melee && item->GetInstanceName() == melee->GetInstanceName() && !item->HasFlag(ITM_FLAG_ACTIVE))
            {
                return false;
            }

            if (ranged && item->GetInstanceName() == ranged->GetInstanceName() && !item->HasFlag(ITM_FLAG_ACTIVE))
            {
                return false;
            }

            return true;
        } ();

        auto fontName = zSTRING{ "FONT_DEFAULT.TGA" };
        if (quickSlots->TEXT_FONT_INVENTORY != -1)
        {
            auto sym = parser->GetSymbol(quickSlots->TEXT_FONT_INVENTORY);
            if (sym)
            {
                sym->GetValue(fontName, 0);
            }
        }

        if (canPrintHotkey)
        {
            auto prevFont = textView->GetFontName();
            textView->SetFont(fontName);
            auto s = zSTRING{ static_cast<int>(hotkey > 10 ? hotkey - quickSlots->maxCellsInRow : hotkey) };
            int px = 1000;
            int py = 8192 / 2 - textView->FontY() / 2;
            textView->PrintTimed(px, py, s, -2, (hotkey > 10 ? &quickSlots->SlotNameColorBarAlt : &quickSlots->SlotNameColorBar));
            textView->SetFont(prevFont);
        }

        reg.eip = zSwitch(0x00667d4b, 0x00695858, 0x006aa89d, 0x0070838d);
    }
    //auto PartialHook_oCItemContainer_Draw_Weapon = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x00667B97, 0x006956AA, 0x006AA54D, 0x0070803D)), &oCItemContainer_Draw);
    auto PartialHook_oCItemContainer_Draw_Magic = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x00667aa4, 0x006955a8, 0x006aa3b6, 0x00707ea6)), &oCItemContainer_Draw);

    void __fastcall oCGame_HandleEvent(::Union::Registers& reg)
    {
        if (player->HasBodyStateModifier(BS_MOD_TRANSFORMED) || player->HasBodyStateModifier(BS_MOD_CONTROLLED) || player->HasBodyStateModifier(BS_MOD_CONTROLLING))
        {
            return;
        }

    #if ENGINE == Engine_G2
        int Key = reg.edi;
    #else
        int Key = reg.esi;
    #endif
        
        if (!quickSlots->HandleInput(Key, false))
        {
            return;
        }

        //reg.esp += 0x4;
        reg.eax = 0x1;
        reg.eip = zSwitch(0x0065ff3f, 0x0068c1c1, 0x0069f9ea, 0x006fd47f);
    }
    auto PartialHook_oCGame_HandleEvent = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x0065ef51, 0x0068a382, 0x0069e9f9, 0x006fc1ee)), &oCGame_HandleEvent);

    void __fastcall oCNpcInventory_HandleEvent(::Union::Registers& reg)
    {
        if (player->HasBodyStateModifier(BS_MOD_TRANSFORMED) || player->HasBodyStateModifier(BS_MOD_CONTROLLED) || player->HasBodyStateModifier(BS_MOD_CONTROLLING))
        {
            return;
        }

        if (player->inventory2.IsOpen() && player->inventory2.GetNextContainerLeft(&player->inventory2))
        {
            return;
        }

        int Key = zSwitch(reg.esi, reg.edi, reg.ebx, reg.ebx);
        
        if (!quickSlots->HandleInput(Key, true))
        {
            return;
        }

        reg.eax = 0x1;
        reg.eip = zSwitch(0x0066f6fe, 0x0069ce7d, 0x006b17e5, 0x0070f375);
    }
    auto PartialHook_oCNpcInventory_HandleEvent = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x0066e419, 0x0069bd16, 0x006b0673, 0x0070e193)), &oCNpcInventory_HandleEvent);

    void __fastcall oCNpc_CanUse(::Union::Registers& reg)
    {
        if (!CanUseHack)
        {
            return;
        }

        reg.eip = zSwitch(0x0068f1f1, 0x006bfe87, 0x006d37f1, 0x00731ca1);
    }
    auto PartialHook_oCNpc_CanUse_MagCircle = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x0068ef41, 0x006bfb91, 0x006d3541, 0x007319f1)), &oCNpc_CanUse);
    auto PartialHook_oCNpc_CanUse_Attributes = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x0068f0c9, 0x006bfd41, 0x006d36c9, 0x00731b79)), &oCNpc_CanUse);

    static zCOLOR originalFontColor;
    void __fastcall oCMag_Book__DoPerFrame_Before(::Union::Registers& reg)
    {
        if (player->HasBodyStateModifier(BS_MOD_TRANSFORMED) || player->HasBodyStateModifier(BS_MOD_CONTROLLED) || player->HasBodyStateModifier(BS_MOD_CONTROLLING))
        {
            return;
        }

        auto spell = reinterpret_cast<oCSpell*>(zSwitch(reg.edx, reg.eax, reg.edx, reg.edx));
        auto item = player->GetSpellBook()->GetSpellItem(spell);

        auto hotkey = quickSlots->GetSlotNumberFromItem(item);

        if (hotkey == -1)
        {
            return;
        }

        originalFontColor = screen->fontColor;

        auto& slotNr = *zSwitch(&reg.eax, &reg.ecx, &reg.esi, &reg.esi);
        slotNr = static_cast<int>(hotkey > 10 ? hotkey - quickSlots->maxCellsInRow : hotkey);
        screen->SetFontColor(hotkey > 10 ? quickSlots->SlotNameColorBarAlt : quickSlots->SlotNameColorBar);
    }
    auto PartialHook_oCMag_Book__DoPerFrame_Before = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x00472ac1, 0x0047bd3d, 0x004783e9, 0x00479789)), &oCMag_Book__DoPerFrame_Before);

    void __fastcall oCMag_Book__DoPerFrame_After(::Union::Registers& reg)
    {
        if (player->HasBodyStateModifier(BS_MOD_TRANSFORMED) || player->HasBodyStateModifier(BS_MOD_CONTROLLED) || player->HasBodyStateModifier(BS_MOD_CONTROLLING))
        {
            return;
        }

        screen->SetFontColor(originalFontColor);
    }
    auto PartialHook_oCMag_Book__DoPerFrame_After = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x00472b29, 0x0047bda7, 0x004784e7, 0x00479887)), &oCMag_Book__DoPerFrame_After);

    void __fastcall CGameManager_Run(::Union::Registers& reg)
	{
		pluginMenus.push_back(new oCMenu_ChgKeys(zSTRING { "QuickSlots:Menu_Opt_QuickSlots_Page1" }));
		pluginMenus.push_back(new oCMenu_ChgKeys(zSTRING { "QuickSlots:Menu_Opt_QuickSlots_Page2" }));
	}
	auto Partial_CGameManager_Run = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x00424b1d, 0x004275ff, 0x0042551d, 0x0042585d)), &CGameManager_Run);

    void __fastcall oCMenuChgKeys_InitText(::Union::Registers& reg)
    {
        auto menuItem = reinterpret_cast<zCMenuItem*>(reg.eax);
        if (!menuItem)
		{
			return;
		}

        if (menuItem->GetType() == MENU_ITEM_INPUT && menuItem->m_parOnChgSetOptionSection == zSTRING { "KEYS" })
		{
			return;
		}

        if (menuItem->GetName().Search("QUICKSLOTS:", 1U) == -1)
        {
            return;
        }

        if(menuItem->GetType() == MENU_ITEM_INPUT && !menuItem->m_parOnChgSetOption.IsEmpty() && !menuItem->m_parOnChgSetOptionSection.IsEmpty() && zoptions->EntryExists(menuItem->m_parOnChgSetOptionSection, menuItem->m_parOnChgSetOption))
        {
            auto text = zoptions->ReadString(menuItem->m_parOnChgSetOptionSection, menuItem->m_parOnChgSetOption, "");
            menuItem->SetText(text, 0, TRUE);
        }

        reg.eip = zSwitch(0x004742c7, 0x0047d7a9, 0x00479d77, 0x0047b1e7);
    }
    auto Partial_oCMenuChgKeys_InitText = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x00474059, 0x0047d507, 0x00479b09, 0x0047af79)), &oCMenuChgKeys_InitText);

	static bool IsSingleBindOption(const zSTRING& opt)
	{
		return opt == quickSlots->modifierHotkeyName || opt == quickSlots->switchVisibilityHotkeyName || opt == quickSlots->fistHotkeyName;
	}

	void __fastcall oCMenu_ChgKeys_ExecCommand(::Union::Registers& reg)
	{
		auto* item = reinterpret_cast<zCMenuItem*>(zSwitch(reg.esi, reg.edi, reg.edi, reg.edi));
		if (!item)
		{
			return;
		}

		if (!IsSingleBindOption(item->m_parOnChgSetOption))
		{
			return;
		}

		if (!item->m_parUserString[1].IsEmpty())
		{
			item->m_parUserString[0] = item->m_parUserString[1];
			item->m_parUserString[1].Clear();
		}
	}
	auto Partial_oCMenu_ChgKeys_ExecCommand = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x004738c0, 0x0047ccc1, 0x00479368, 0x0047A705)), &oCMenu_ChgKeys_ExecCommand);
}