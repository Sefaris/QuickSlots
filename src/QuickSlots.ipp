namespace GOTHIC_NAMESPACE 
{
    QuickSlots::QuickSlots()
    {
        this->modifierHotkeyName = zSTRING{ "keyQSModifierHotkey" };
        this->switchVisibilityHotkeyName = zSTRING{ "keyQSSwitchVisibilityHotkey" };
        this->fistHotkeyName = zSTRING{ "keyQSFistHotkey" };

        this->backView = new zCView(0, 0, 8192, 8192);
        this->backView->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
        this->backView->SetTransparency(255);
        this->itemView = new zCView(0, 0, 0, 0);
        this->itemView->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
        this->itemView->SetTransparency(255);
        this->itemViewEquipped = new zCView(0, 0, 0, 0);
        this->itemViewEquipped->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
        this->itemViewEquipped->SetTransparency(255);
        this->itemViewActive = new zCView(0, 0, 0, 0);
        this->itemViewActive->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
        this->itemViewActive->SetTransparency(255);
        this->textView = new zCView(0, 0, 0, 0);
        this->textView->SetTransparency(255);
        this->textView->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);

        this->itemWorld = zfactory->CreateWorld();
    #if ENGINE > Engine_G2
        this->itemWorld->m_bIsInventoryWorld = TRUE;
    #endif
    }

    QuickSlots::~QuickSlots()
    {
        this->Clear();

        if (this->backView)
        {
            screen->RemoveItem(this->backView);
            delete this->backView;
        }

        if (this->textView)
        {
            delete this->textView;
        }

        if (this->itemView)
        {
            delete this->itemView;
        }

        if (this->itemViewEquipped)
        {
            delete this->itemViewEquipped;
        }

        if (this->itemViewActive)
        {
            delete this->itemViewActive;
        }

        if (this->itemWorld)
        {
            this->itemWorld->Release();
            this->itemWorld = nullptr;
        }

        this->prevPresetSwitchTimestamp = 0;
    }

    void QuickSlots::LoadSettings()
    {
        const auto invSlotDefault = zSTRING{"INV_SLOT"};
        const auto invSlotEquippedDefault = zSTRING{"INV_SLOT_EQUIPPED"};
        const auto invSlotActiveDefault = zSTRING{"INV_SLOT_HIGHLIGHTED"};

        this->enableFistDraw = zoptions->ReadBool(pluginName, "QSEnableFistDraw", false);
        this->drawWeaponOnSwitch = zoptions->ReadBool(pluginName, "QSDrawWeaponOnSwitch", true);
        this->altBarAlwaysVisible = zoptions->ReadBool(pluginName, "QSAltBarAlwaysVisible", false);
        this->hideEmptySlots = zoptions->ReadInt(pluginName, "QSHideEmptySlots", 0);
        this->showEquippedSpells = zoptions->ReadBool(pluginName, "QSShowEquippedSpells", false);
        this->showInfoInInventory = zoptions->ReadBool(pluginName, "QSShowInfoInInventory", true);
        this->switchPosY = zoptions->ReadBool(pluginName, "QSSwitchPosY", false);
        this->hideBar = zoptions->ReadBool(pluginName, "QSHideBar", false);
        this->rotateUsedItem = zoptions->ReadBool(pluginName, "QSRotateUsedItem", false);

        this->modifierHotkey = pluginInput.GetKeyRaw(this->modifierHotkeyName, this->modifierHotkey, "QSModifierHotkey");
        this->switchVisibilityHotkey = pluginInput.GetKeyRaw(this->switchVisibilityHotkeyName, this->switchVisibilityHotkey, "QSSwitchVisibilityHotkey");
        this->fistHotkey = pluginInput.GetKeyRaw(this->fistHotkeyName, this->fistHotkey, "QSFistHotkey");

        this->cellSize = zoptions->ReadInt(pluginName, "QSCellSize", 70);
        int maxCellSize = zrenderer->vid_xdim / 2 / this->maxCellsInRow;

        this->cellSize = std::clamp(this->cellSize, 35, maxCellSize);
        zoptions->WriteInt(pluginName, "QSCellSize", this->cellSize, FALSE);

        this->cellSizeX = screen->anx(this->cellSize);
        this->cellSizeY = screen->any(this->cellSize);

        auto invSlot = zoptions->ReadString(pluginName, "QSInventorySlotTexture", invSlotDefault);
        auto invSlotEquipped = zoptions->ReadString(pluginName, "QSInventorySlotEquippedTexture", invSlotEquippedDefault);
        auto invSlotActive = zoptions->ReadString(pluginName, "QSInventorySlotActiveTexture", invSlotActiveDefault);

        if (invSlot.IsEmpty())
        {
            invSlot = invSlotDefault;
            zoptions->WriteString(pluginName, "QSInventorySlotTexture", invSlot, FALSE);
        }

        if (invSlotEquipped.IsEmpty())
        {
            invSlotEquipped = invSlotEquippedDefault;
            zoptions->WriteString(pluginName, "QSInventorySlotEquippedTexture", invSlotEquipped, FALSE);
        }

        if (invSlotActive.IsEmpty())
        {
            invSlotActive = invSlotActiveDefault;
            zoptions->WriteString(pluginName, "QSInventorySlotActiveTexture", invSlotActive, FALSE);
        }

        if (this->itemView)
        {
            this->itemView->InsertBack(invSlot);
        }

        if (this->itemViewEquipped)
        {
            this->itemViewEquipped->InsertBack(invSlotEquipped);
        }

        if (this->itemViewActive)
        {
            this->itemViewActive->InsertBack(invSlotActive);
        }

        this->TEXT_FONT_INVENTORY = parser->GetIndex("TEXT_FONT_INVENTORY");
        this->SlotNameColorBar = zCOLOR(255, 255, 255);
        this->SlotNameColorBarAlt = zCOLOR(255, 128, 128);
    }

    void QuickSlots::Archive()
    {
        if (slInfo.Slot < 0)
        {
            return;
        }

        zCArchiver* ar = zarcFactory->CreateArchiverWrite(::Union::String::Format("{0}\\QuickSlots.sav", GetArchivePath()).ToChar(), zARC_MODE_ASCII, 0, 0);
        if (!ar)
        {
            return;
        }

        ar->WriteInt("QSCount", this->CurrentSlots.size());
        for (auto& slot : this->CurrentSlots)
        {
            ar->WriteInt("QS", slot.first);
            ar->WriteString("QS", slot.second.c_str());
        }

        ar->WriteInt("PresetsCount", this->Presets.size());
        for (auto& PresetIt : this->Presets)
        {
            ar->WriteInt("PresetID", PresetIt.first);
            ar->WriteInt("PresetQSCount", PresetIt.second.Slots.size());
            for (auto& slot : PresetIt.second.Slots)
            {
                ar->WriteInt("PresetQS", slot.first);
                ar->WriteString("PresetQS", slot.second.c_str());
            }
            ar->WriteInt("PresetEquippedCount", PresetIt.second.EquippedItems.size());
            for (auto& equipped : PresetIt.second.EquippedItems)
            {
                ar->WriteString("PresetEquipped", equipped.c_str());
            }
        }

        ar->Close();
        ar->Release();
    }

    void QuickSlots::Unarchive()
    {
        if (slInfo.LoadReason == LoadReason::NEW_GAME)
        {
            this->Clear();
            return;
        }

        if (slInfo.Slot < 0)
        {
            return;
        }

        this->Clear();

        zCArchiver* ar = zarcFactory->CreateArchiverRead(::Union::String::Format("{0}\\QuickSlots.sav", GetArchivePath()).ToChar(), 0);
        if (!ar)
        {
            this->FillDefaultSlots();
            return;
        }

        auto QSCount = ar->ReadIntSafe("QSCount");

        for (int i = 0; i < QSCount; i++)
        {
            auto slotNr = ar->ReadInt("QS");
            auto slotInstanceName = ar->ReadString("QS");

            if (!slotInstanceName.IsEmpty() && slotNr > 0)
            {
                this->CurrentSlots.insert({ slotNr, slotInstanceName.ToChar() });
            }
        }

        this->SyncPlaceholdersWithCurrentSlots();

        auto PresetsCount = ar->ReadIntSafe("PresetsCount");
        for (int i = 0; i < PresetsCount; i++)
        {
            auto PresetID = ar->ReadInt("PresetID");

            auto PresetQSCount = ar->ReadInt("PresetQSCount");
            std::map<size_t, std::string> Slots;
            for (auto ii = 0; ii < PresetQSCount; ii++)
            {
                auto slotNr = ar->ReadInt("PresetQS");
                auto slotInstanceName = ar->ReadString("PresetQS");
                Slots.insert({ slotNr,slotInstanceName.ToChar() });
            }

            auto PresetEquippedCount = ar->ReadIntSafe("PresetEquippedCount");
            std::vector<std::string> Equipped;
            for (auto ii = 0; ii < PresetEquippedCount; ii++)
            {
                Equipped.push_back(ar->ReadString("PresetEquipped").ToChar());
            }

            this->Presets.insert({ PresetID, QSPreset(Slots, Equipped) });
        }

        ar->Close();
        ar->Release();
    }

    bool QuickSlots::IsConsoleOpen()
    {
        for (int i = 0; i < zCConsole::active_consoles.GetNumInList(); i++)
        {
            if (zCConsole::active_consoles[i]->IsVisible())
            {
                return true;
            }
        }
        return false;
    }

    bool QuickSlots::IsValidState(bool print)
    {
        if (player->IsUnconscious()
        || player->attribute[NPC_ATR_HITPOINTS] <= 0
        || this->IsConsoleOpen()
        || player->GetBodyState() == BS_MOBINTERACT
        || player->GetBodyState() == BS_MOBINTERACT_INTERRUPT
        || (ogame->GetWorld() && ogame->GetWorld()->csPlayer && ogame->GetWorld()->csPlayer->GetPlayingGlobalCutscene())
        || !oCInformationManager::GetInformationManager().HasFinished()
        || player->HasBodyStateModifier(BS_MOD_TRANSFORMED) || player->HasBodyStateModifier(BS_MOD_CONTROLLED) || player->HasBodyStateModifier(BS_MOD_CONTROLLING)
        || zCCSCamera::playing != 0
        )
        {
            return false;
        }

        if (CheckScriptFlag("ISINCAMERA") || CheckScriptFlag("ISINFINALBOARDS"))
        {
            return false;
        }

        /*auto playerAniCtrl = player->anictrl;
        auto playerModel = playerAniCtrl->GetModel();
        if (playerModel->IsAniActive(playerModel->GetAniFromAniID(playerAniCtrl->_t_stand_2_jumpup))
        || playerModel->IsAniActive(playerModel->GetAniFromAniID(playerAniCtrl->t_stand_2_jumpupmid))
        //|| playerModel->IsAniActive(playerModel->GetAniFromAniID(playerAniCtrl->t_stand_2_jumpuplow))
        //|| playerModel->IsAniActive(playerModel->GetAniFromAniID(playerAniCtrl->_t_stand_2_jump))
        )
        {
            return false;
        }*/

        if (!print && (
        //|| !player->GetAnictrl()->IsStanding()
        /*|| player->GetBodyState() == BS_SWIM
        || player->GetBodyState() == BS_DIVE
        || player->GetBodyState() == BS_JUMP
        || player->GetBodyState() == BS_CLIMB
        || player->GetBodyState() == BS_FALL
        || */(player->GetBodyState() != BS_RUN && player->GetBodyState() != BS_WALK && player->GetBodyState() != BS_STAND && player->GetBodyState() != BS_SNEAK)
        || player->interactItemCurrentState != -1
        || player->interactItem != nullptr
        || player->GetBodyState() == BS_ITEMINTERACT
        ))
        {
            return false;
        }

        /*if(print && (
            //idk yet
        ))
        {
            return false;
        }*/

        return true;
    }

    int QuickSlots::GetModifierHotkey()
    {
        return this->modifierHotkey;
    }

    int QuickSlots::GetSlotNumberFromKey(int Key)
    {
        auto SlotNr = Key - 1;

        if (SlotNr >= 1 && SlotNr <= this->maxSlots)
        {
            return SlotNr;
        }

        return -1;
    }

    int QuickSlots::GetPresetNumberFromKey(int Key)
    {
        auto PresetNr = Key - 58;

        if (PresetNr >= 1 && PresetNr <= 4)
        {
            return PresetNr;
        }

        return -1;
    }

    int QuickSlots::GetSpellNoByItem(oCItem* Item)
    {
        auto playerSpellBook = player->GetSpellBook();
        if(!playerSpellBook)
        {
            return 0;
        }

        for(int i = 0; i < playerSpellBook->spells.GetNumInList(); i++)
        {
            if(playerSpellBook->spellitems[i]->GetInstanceName() == Item->GetInstanceName())
            {
                return playerSpellBook->spells[i]->keyNo;
            }
        }

        return 0;
    }

    bool QuickSlots::HandleInput(int Key, bool Inv)
    {
        /*if (ogame->game_testmode)
        {
            return false;
        }*/
        bool modifierHotkeyPressed = pluginInput.KeyPressed(this->GetModifierHotkey());

        if (!Inv && modifierHotkeyPressed)
        {
            if (Key == this->switchVisibilityHotkey)
            {
                this->hideBar = !this->hideBar;
                zoptions->WriteBool(pluginName, "QSHideBar", this->hideBar, false);
                return true;
            }
        }

        #if ENGINE >= Engine_G2
        if (Key == this->fistHotkey)
        {
            if (this->enableFistDraw && player->CanDrawWeapon())
            {
                this->DrawFist();

                return true;
            }
            return false;
        }
    #endif

        switch (Key)
        {
            case KEY_F1:
            case KEY_F2:
            case KEY_F3:
            case KEY_F4:
            {
                auto PresetNr = this->GetPresetNumberFromKey(Key);
                if (modifierHotkeyPressed && PresetNr != -1)
                {
                    if(!pluginInput.KeyToggled(Key))
                    {
                        return true;
                    }

                    if (Inv)
                    {
                        this->RegisterPreset(PresetNr);
                    }
                    else
                    {
                        this->SwitchPreset(PresetNr);
                    }
                    return true;
                }
                break;
            }

            case KEY_1:
            case KEY_2:
            case KEY_3:
            case KEY_4:
            case KEY_5:
            case KEY_6:
            case KEY_7:
            case KEY_8:
            case KEY_9:
            case KEY_0:
            {
                auto SlotNr = this->GetSlotNumberFromKey(Key);

                if(!pluginInput.KeyToggled(Key))
                {
                    return true;
                }

                if (modifierHotkeyPressed)
                {
                    SlotNr += this->maxCellsInRow;
                }

                if (Inv)
                {
                    this->ClearSpellBook();
                    this->RegisterItem(player->inventory2.GetSelectedItem(), SlotNr);
                    this->SyncPlaceholdersWithCurrentSlots();
                    this->RestoreSpellBook();
                }
                else
                {
                    this->UseItemFromSlot(SlotNr);
                }

                return true;
            }
        }

        return false;
    }

    void QuickSlots::Loop()
    {
        this->PrintQuickSlots();
    }

    size_t QuickSlots::GetSlotNumberFromItem(oCItem* item)
    {
        for (auto& it : this->CurrentSlots)
        {
            std::string instanceName = item->GetInstanceName().ToChar();
            if (it.second == instanceName)
            {
                return it.first;
            }
        }

        return -1;
    }

    size_t QuickSlots::IsRegistered(oCItem* Item)
    {
        std::string instanceName = Item->GetInstanceName().ToChar();
        for (auto& slot : this->CurrentSlots)
        {
            if (slot.second == instanceName)
            {
                return slot.first;
            }
        }

        return 0;
    }

    void QuickSlots::RegisterItem(oCItem* Item, size_t SlotNr)
    {
        if (!Item)
        {
            return;
        }

        if (SlotNr == 0)
        {
            return;
        }

        auto itemCat = player->inventory2.GetCategory(Item);
        auto itemActive = Item->HasFlag(ITM_FLAG_ACTIVE);
        std::string itemInstanceName = Item->GetInstanceName().ToChar();

        auto it = this->CurrentSlots.find(SlotNr);
        if (it != this->CurrentSlots.end())
        {
            auto slotInstanceName = it->second;

            this->CurrentSlots.erase(it);

            if (slotInstanceName == itemInstanceName)
            {
                return;
            }
        }

        std::erase_if (this->CurrentSlots, [SlotNr, itemInstanceName](const auto& slot)
        {
            auto const& [key, value] = slot;
            return key == SlotNr || value == itemInstanceName;
        });

        this->CurrentSlots.insert({ SlotNr, itemInstanceName });

        /*if (itemCat == INV_RUNE)
        {
            // If this rune was registered on another slot, deregister it from mag_book
            if (player && player->GetSpellBook())
            {
                auto playerSpellBook = player->GetSpellBook();
                // Check all slots 1 - this->maxSlots (def 20) for this spell
                for (size_t slot = 1; slot <= this->maxSlots; slot++)
                {
                    if (slot == SlotNr) continue; // Skip target slot
                    
                    auto spell = playerSpellBook->GetSpellByKey(slot);
                    if (spell && playerSpellBook->GetSpellItem(spell) && playerSpellBook->GetSpellItem(spell)->instanz == Item->instanz)
                    {
                        this->DeRegisterSpell(slot);
                        break;
                    }
                }
            }
            
            this->RegisterSpell(Item, SlotNr);
        }*/
    }

    void QuickSlots::RegisterSpell(oCItem* Item, size_t SlotNr)
    {
        auto playerSpellBook = player->GetSpellBook();
        
        if (!playerSpellBook)
        {
            return;
        }

        this->DeRegisterSpell(SlotNr);

        if (!this->CanUseItem(Item, true))
        {
            return;
        }

        playerSpellBook->NextRegisterAt(SlotNr);
        playerSpellBook->Register(Item, 1);
    }

    void QuickSlots::DeRegisterSpell(size_t SlotNr)
    {
        if (!player)
        {
            return;
        }

        auto playerSpellBook = player->GetSpellBook();

        if (!playerSpellBook)
        {
            return;
        }

        auto spell = playerSpellBook->GetSpellByKey(SlotNr);
        if (spell && playerSpellBook->GetSpellItem(spell))
        {
            playerSpellBook->DeRegister(playerSpellBook->GetSpellItem(spell));
        }
    }

    size_t QuickSlots::GetBestFreeSlot()
    {
        for (size_t pos = 1; pos <= this->maxSlots; pos++)
        {
            auto it = this->CurrentSlots.find(pos);
            if (it == this->CurrentSlots.end())
            {
                return pos;
            }
        }
        return 1;
    }

    void QuickSlots::ClearSpellBook()
    {
        if (!player)
        {
            return;
        }

        this->UnequipSpellItems();
        for (size_t pos = 1; pos <= this->maxSlots; pos++)
        {
            this->DeRegisterSpell(pos);
        }
    }

    void QuickSlots::RestoreSpellBook()
    {
        if (!player)
        {
            return;
        }

        for (size_t pos = 1; pos <= this->maxSlots; pos++)
        {
            auto it = this->CurrentSlots.find(pos);
            if (it == this->CurrentSlots.end())
            {
                continue;
            }

            auto itemIndex = parser->GetIndex(zSTRING{ it->second.c_str() });
            if (itemIndex < 0)
            {
                continue;
            }

            auto item = player->IsInInv(itemIndex, 1);
            if (!item)
            {
                continue;
            }

            if (item->amount <= 0)
            {
                continue;
            }

            auto itemCat = player->inventory2.GetCategory(item);

            if (itemCat == INV_RUNE)
            {
                this->RegisterSpell(item, pos);
            }
        }
    }

    void QuickSlots::RefreshSpellBook()
    {
        if (!player)
        {
            return;
        }

        this->ClearSpellBook();
        this->RestoreSpellBook();
    }

    oCItem* QuickSlots::CreatePlaceholderForSlot(size_t slot, int itemIndex, int amount)
    {
        auto it = this->SlotPlaceholders.find(slot);
        if (it != this->SlotPlaceholders.end() && it->second)
        {
            it->second->Release();
            it->second = nullptr;
            this->SlotPlaceholders.erase(it);
        }

        if (itemIndex < 0)
        {
            return nullptr;
        }

        oCItem* ph = new oCItem();
        ph->InitByScript(itemIndex, amount);

        this->SlotPlaceholders.insert({ slot, ph });
        return ph;
    }

    void QuickSlots::SyncPlaceholdersWithCurrentSlots()
    {
        for (auto& p : this->SlotPlaceholders)
        {
            if (p.second)
            {
                p.second->Release();
                p.second = nullptr;
            }
        }
        this->SlotPlaceholders.clear();

        for (auto& slot : this->CurrentSlots)
        {
            int itemIndex = parser->GetIndex(zSTRING{ slot.second.c_str() });
            if (itemIndex <= 0)
            {
                continue;
            }

            this->CreatePlaceholderForSlot(slot.first, itemIndex, 1);
        }
    }

    void QuickSlots::Clear()
    {
        this->ClearPresets();
        this->ClearSlots();
    }

    void QuickSlots::ClearSlots()
    {
        for (auto &p : this->SlotPlaceholders)
        {
            if (p.second)
            {
                p.second->Release();
                p.second = nullptr;
            }
        }
        this->SlotPlaceholders.clear();
        this->CurrentSlots.clear();
    }

    void QuickSlots::ClearPresets()
    {
        this->Presets.clear();
    }

    void QuickSlots::DrawFist()
    {
        if (player->GetSpellBook())
        {
            player->GetSpellBook()->Close(TRUE);
        }

        if (player->inventory2.IsOpen())
        {
            player->inventory2.Close();
        }

        if (auto container = zDYNAMIC_CAST<oCMobContainer>(player->GetInteractMob()))
        {
            container->Close(player);
            container->Reset();
        }

        switch (player->GetWeaponMode())
        {
            case NPC_WEAPON_NONE:
                break;
            case NPC_WEAPON_FIST:
            {
                this->HideWeapon();
            }
            return;
            default:
            {
                this->HideWeapon();
            }
            break;
        }

        this->DrawWeapon(NPC_WEAPON_FIST, 1);
    }

    void QuickSlots::PrintQuickSlots()
    {
        if (!ogame || ogame->singleStep || !ogame->game_drawall || !ogame->GetShowPlayerStatus())
        {
            return;
        }

        if (!this->backView || !this->itemView || !this->itemViewEquipped || !this->itemViewActive || !this->itemWorld || !this->textView)
        {
            return;
        }

        auto invIsOpen = player->inventory2.IsOpen();

        if (invIsOpen && player->inventory2.GetNextContainerLeft(&player->inventory2))
        {
            return;
        }

        if (!invIsOpen && this->hideBar)
        {
            return;
        }

        if (!this->IsValidState(true))
        {
            return;
        }

        // Professional fix for not being able to climb ledges while quickslots are drawn
        {
            player->human_ai->GetLedgeInfo();
        }

        bool modifierHotkeyPressed = pluginInput.KeyPressed(this->GetModifierHotkey());

        std::vector<QS> qs;
        qs.reserve(this->maxSlots);

        size_t minPos = 1;
        size_t maxPos = this->maxCellsInRow;
        
        size_t row1Count = 0;
        size_t row2Count = 0;

        if (modifierHotkeyPressed)
        {
            minPos += this->maxCellsInRow;
            maxPos += this->maxCellsInRow;
        }

        if (invIsOpen || this->altBarAlwaysVisible)
        {
            minPos = 1;
            maxPos = this->maxSlots;
        }

        if (maxPos > this->maxSlots)
        {
            maxPos = this->maxSlots;
        }

        for (size_t pos = minPos; pos <= maxPos; pos++)
        {
            auto it = this->CurrentSlots.find(pos);
            if (it == this->CurrentSlots.end())
            {
                if (!this->hideEmptySlots || invIsOpen)
                {
                    qs.push_back(QS(pos, nullptr, 0, 0, false));
                }
                continue;
            }

            bool itemInInv = false;
            int itemAmount = 1;
            bool itemEquipped = false;
            bool itemCanUse = true;
            auto itemIndex = parser->GetIndex(zSTRING{ it->second.c_str() });
            if (itemIndex < 0)
            {
                if (!this->hideEmptySlots || invIsOpen)
                {
                    qs.push_back(QS(pos, nullptr, 0, 0, false));
                }
                continue;
            }

            oCItem* item = nullptr;
            auto itm = player->IsInInv(itemIndex, itemAmount);
            if (itm)
            {
                itemAmount = itm->amount;
                itemInInv = true;
                itemEquipped = itm->HasFlag(ITM_FLAG_ACTIVE);
                itemCanUse = this->CanUseItem(itm, true);
                //item = itm; // reuse existing inventory item for rendering
            }

            auto phIt = this->SlotPlaceholders.find(pos);
            if (phIt != this->SlotPlaceholders.end() && phIt->second && phIt->second->instanz == itemIndex)
            {
                item = phIt->second;
            }
            else
            {
                item = this->CreatePlaceholderForSlot(pos, itemIndex, 1);
            }

            if (this->hideEmptySlots == 2 && (!itemInInv || !itemCanUse) && !invIsOpen)
            {
                if (invIsOpen)
                {
                    qs.push_back(QS(pos, (item ? item : nullptr), 0, 0, false));
                }
                continue;
            }

            auto _showEquippedSpells = this->showEquippedSpells;

            auto itemActive = [item, itemCanUse, itemEquipped, itemInInv, _showEquippedSpells]() -> int
            {
                if (!item)
                {
                    return 0;
                }

                if (!itemCanUse)
                {
                    return 0;
                }

                if (player->interactItem && player->interactItem->instanz == item->instanz)
                {
                    return 2;
                }

                if (!itemInInv)
                {
                    return 0;
                }

                auto leftHand = player->GetLeftHand();
                if (leftHand && leftHand->type == zVOB_TYPE_ITEM && static_cast<oCItem*>(leftHand)->instanz == item->instanz)
                {
                    return 2;
                }

                auto rightHand = player->GetRightHand();
                if (rightHand && rightHand->type == zVOB_TYPE_ITEM && static_cast<oCItem*>(rightHand)->instanz == item->instanz)
                {
                    return 2;
                }

                if (item->mainflag == ITM_CAT_MUN)
                {
                    if (player->GetEquippedRangedWeapon() && player->GetEquippedRangedWeapon()->munition == item->instanz)
                    {
                        return 1;
                    }

                    if (player->GetWeapon() && player->GetWeapon()->munition == item->instanz)
                    {
                        return 2;
                    }
                }
                else
                {
                    if (player->GetWeapon() && player->GetWeapon()->instanz == item->instanz)
                    {
                        return 2;
                    }

                    auto itemCat = player->inventory2.GetCategory(item);
                    if (itemCat == INV_RUNE)
                    {
                        if (player->HasMagic())
                        {
                            if (player->GetWeaponMode() == NPC_WEAPON_MAG && player->GetSpellBook()->GetSpellItem(player->GetSpellBook()->GetSelectedSpell())->instanz == item->instanz)
                            {
                                return 2;
                            }
                            
                            return _showEquippedSpells ? 1 : 0;
                        }
                    }

                    if (itemEquipped)
                    {
                        return 1;
                    }
                }

                return 0;
            }();

            if (item)
            {
                qs.push_back(QS(it->first, item, itemActive, (itemInInv ? itemAmount : 0), itemCanUse));
                if (it->first > this->maxCellsInRow)
                {
                    row2Count++;
                }
                else
                {
                    row1Count++;
                }
            }
            else if (!this->hideEmptySlots || invIsOpen)
            {
                qs.push_back(QS(pos, nullptr, 0, 0, false));
            }
        }

        if (!qs.size())
        {
            return;
        }

        if (!this->hideEmptySlots || invIsOpen)
        {
            row1Count = this->maxCellsInRow;
            row2Count = this->maxCellsInRow;
        }

        size_t cellsInRow = row1Count;
        int startPosX;
        int startPosY;
        int inverse;

        if (invIsOpen)
        {
            int InvPosX, InvPosY, InvSizeX, InvSizeY;
        #if ENGINE >= Engine_G2
            if(player->inventory2.viewBack)
            {
                player->inventory2.viewBack->GetPos(InvPosX, InvPosY);
                player->inventory2.viewBack->GetSize(InvSizeX, InvSizeY);
            }
            else
        #endif
            {
                player->inventory2.GetPosition(InvPosX, InvPosY);
                player->inventory2.GetSize(InvSizeX, InvSizeY);
            }

            if (hedinsInvDetected)
            {
                startPosX = 8192 / 2;
            }
            else
            {
                startPosX = (InvPosX - this->cellSizeX) / 2;
            }
            startPosY = this->cellSizeY;
            inverse = 1;
        }
        else
        {
            startPosX = 8192 / 2;
            if (this->switchPosY)
            {
                startPosY = this->cellSizeY;
                inverse = 1;
            }
            else
            {
                startPosY = 8192 - (this->cellSizeY * 2);
                inverse = -1;
            }
        }

        if (invIsOpen && this->showInfoInInventory)
        {
            auto screenFontColor = screen->fontColor;

            auto modifierName = zCInput::GetNameByControlValue(this->GetModifierHotkey());
            if (modifierName.IsEmpty())
            {
                modifierName = zSTRING{ pluginInput.GetKeyName(this->GetModifierHotkey()).c_str() };
            }
            auto altQSText = zSTRING{ "Przytrzymaj "} + modifierName + zSTRING{ ", aby skorzysta� z alternatywnych slot�w." };
            auto altQSTextSize = screen->FontSize(altQSText);
            auto altQSTextPosX = startPosX - altQSTextSize / 2;
            auto altQSTextPosY = startPosY + (screen->FontY() / 2) + (this->cellSizeY * 2);
            
            screen->SetFontColor(this->SlotNameColorBarAlt);
            screen->Print(altQSTextPosX, altQSTextPosY, altQSText);

            screen->SetFontColor(GFX_WHITE);
            auto presetsText = zSTRING{ "Przytrzymaj "} + modifierName + zSTRING{ " i naci�nij F1-F4"} + zSTRING{ ", aby zapisa� preset." };
            auto presetsTextSize = screen->FontSize(presetsText);
            auto presetsTextPosX = startPosX - presetsTextSize / 2;
            auto presetsTextPosY = altQSTextPosY + screen->FontY();
            screen->Print(presetsTextPosX, presetsTextPosY, presetsText);

            screen->SetFontColor(screenFontColor);
        }

        int iCell = 0;
        int iRow = 0;
        bool iRowChanged = false;

        screen->InsertItem(this->backView);
    #if ENGINE >= Engine_G2
        this->backView->m_bFillZ = TRUE;
    #endif

        this->backView->Blit();

        for (auto&it : qs)
        {
            if (!iRowChanged && it.slot > this->maxCellsInRow && (invIsOpen || this->altBarAlwaysVisible))
            {
                iRowChanged = true;
                iCell = 0;
                iRow++;
            }

            cellsInRow = it.slot > this->maxCellsInRow ? row2Count : row1Count;

            int rowStartPosX = startPosX - ((cellsInRow * this->cellSizeX) / 2);
            int rowStartPosY = startPosY + (iRow * this->cellSizeY) * inverse;

            int curPosX = rowStartPosX + (iCell * this->cellSizeX);

            iCell++;

            zCView* currentView = nullptr;
            if (it.item)
            {
                switch(it.inv)
                {
                    case 1:
                        currentView = this->itemViewEquipped;
                        break;
                    case 2:
                        currentView = this->itemViewActive;
                        break;
                    default:
                        currentView = this->itemView;
                }
            }
            else
            {
                currentView = this->itemView;
            }

            // +++ ITEM +++
            this->backView->InsertItem(currentView);
            currentView->SetPos(curPosX, rowStartPosY);
            currentView->SetSize(this->cellSizeX, this->cellSizeY);
            currentView->Blit();

            if (it.item && IsValidItemViewport(currentView))
            {
                auto rotateItem = 0.0f;
                if (it.inv == 2 && this->rotateUsedItem)
                {
                    rotateItem = 1.0f;
                }
                else
                {
                    it.item->RotateForInventory(1);
                }
                auto prevLighting = oCItem::GetLightingSwell();
                oCItem::SetLightingSwell(it.amount > 0 ? TRUE : FALSE);
                it.item->RenderItem(this->itemWorld, currentView, rotateItem);
                oCItem::SetLightingSwell(prevLighting);
            }

            this->backView->RemoveItem(currentView);
            // --- ITEM ---

            // +++ TEXT +++
            this->backView->InsertItem(this->textView);
            this->textView->SetPos(curPosX, rowStartPosY);
            this->textView->SetSize(this->cellSizeX, this->cellSizeY);
            this->textView->ClrPrintwin();

            this->textView->SetFontColor((it.slot > this->maxCellsInRow ? this->SlotNameColorBarAlt : this->SlotNameColorBar));
            auto slotNr = zSTRING{ static_cast<int>(it.slot > this->maxCellsInRow ? it.slot - this->maxCellsInRow : it.slot) };
            this->textView->Print(750, 750, slotNr);

            if (it.item && it.item->mainflag != ITM_CAT_NF && it.item->mainflag != ITM_CAT_FF && it.item->HasFlag(ITM_FLAG_MULTI))
            {
                this->textView->SetFontColor(it.amount > 0 ? GFX_WHITE : GFX_WARMGREY);
                auto itemAmount = zSTRING{ (it.amount) };
                this->textView->Print(8192 - this->textView->FontSize(itemAmount) - 750, 8192 - this->textView->FontY() - 750, itemAmount);
            }

            if (it.item && (it.amount == 0 || !it.canUse))
            {
                this->textView->SetFontColor(GFX_RED);
                auto emptyText = zSTRING{ "X" };
                this->textView->PrintCXY(emptyText);
            }

            this->textView->Blit();
            this->backView->RemoveItem(this->textView);
            // --- TEXT ---
        }

        screen->RemoveItem(this->backView);

        // Cleanup memory
        /*for (auto& q : qs)
        {
            // Only release items we created temporarily
            if (q.item && q.created)
            {
                q.item->Release();
                q.item = nullptr;
            }
        }*/
        qs.clear();

        int x, y, sx, sy;
        screen->GetViewport(x, y, sx, sy);
        zrenderer->SetViewport(x, y, sx, sy);
    }

    void QuickSlots::FillDefaultSlots()
    {
        auto meleeWeapon = player->GetEquippedMeleeWeapon();
        if (meleeWeapon)
        {
            this->RegisterItem(meleeWeapon, 1);
        }

        auto rangedWeapon = player->GetEquippedRangedWeapon();
        if (rangedWeapon)
        {
            this->RegisterItem(rangedWeapon, 2);

            if (rangedWeapon->munition > 0)
            {
                auto munition = player->IsInInv(rangedWeapon->munition, 1);
                if (munition)
                {
                    this->RegisterItem(munition, 3);
                }
            }
        }

        auto spellBook = player->GetSpellBook();
        if (spellBook && spellBook->spells.GetNumInList() > 0)
        {
            for (int i = 0; i < spellBook->spells.GetNumInList(); i++)
            {
                auto spell = spellBook->spells[i];
                if (!spell)
                {
                    continue;
                }

                auto spellItem = spellBook->GetSpellItem(spell);
                if (!spellItem)
                {
                    continue;
                }

                auto keyNo = static_cast<size_t>(spell->keyNo + 3);
                
                this->RegisterItem(spellItem, keyNo);
            }
        }
    }

    void QuickSlots::DrawWeapon(int Mode, int Force)
    {
        player->GetEM()->OnMessage(new oCMsgWeapon(oCMsgWeapon::EV_DRAWWEAPON, Mode, Force), player);
    }

    void QuickSlots::HideWeapon()
    {
        if (player->GetWeaponMode() != NPC_WEAPON_NONE)
        {
            player->GetEM()->OnMessage(new oCMsgWeapon(oCMsgWeapon::EV_REMOVEWEAPON, 0, 0), player);
        }

        if (player->GetSpellBook())
        {
            player->GetSpellBook()->Close(TRUE);
        }
    }

    void QuickSlots::StandUp()
    {
        auto msg = new oCMsgMovement(oCMsgMovement::EV_STANDUP, 0);
        msg->targetMode = 0;
        player->GetEM()->OnMessage(msg, player);
    }

    void QuickSlots::RegisterPreset(size_t PresetNr)
    {
        std::vector<std::string> presetItems;
        std::vector<std::string> presetItemsArmor;

        player->inventory2.UnpackAllItems();

    #if ENGINE <= Engine_G1A
        for (int cat = 0; cat < INV_MAX; cat++)
        {
            auto playerInv = player->inventory2.inventory[cat].next;
    #else
            auto playerInv = player->inventory2.inventory.next;
    #endif
            while (playerInv)
            {
                auto item = playerInv->GetData();
                playerInv = playerInv->next;

                if (item->instanz < 0)
                {
                    continue;
                }

                if (!item->HasFlag(ITM_FLAG_ACTIVE))
                {
                    continue;
                }

                if (item->HasFlag(ITM_CAT_MUN))
                {
                    continue;
                }

                std::string itemInstance = item->GetInstanceName().ToChar();

                if (item->mainflag == ITM_CAT_ARMOR)
                {
                    presetItemsArmor.push_back(itemInstance);
                    continue;
                }

                if (item->mainflag == ITM_CAT_NF || item->mainflag == ITM_CAT_FF)
                {
                    presetItems.insert(presetItems.begin(), itemInstance);
                }
                else
                {
                    presetItems.push_back(itemInstance);
                }
            }
    #if ENGINE <= Engine_G1A
        }
    #endif

        auto msg = zSTRING{ "Saving preset: " } + zSTRING{ static_cast<int>(PresetNr) };
        ogame->GetTextView()->Printwin(msg);

        for (auto it = presetItemsArmor.rbegin(); it != presetItemsArmor.rend(); ++it)
        {
            presetItems.push_back(*it);
        }

        auto preset = QSPreset(CurrentSlots, presetItems);

        auto it = this->Presets.find(PresetNr);
        if (it == this->Presets.end())
        {
            this->Presets.insert({ PresetNr, preset });
            return;
        }

        it->second = preset;
    }

    void QuickSlots::SwitchPreset(size_t PresetNr)
    {
        if (!this->IsValidState())
        {
            return;
        }

        if (player->GetWeaponMode() != NPC_WEAPON_NONE)
        {
            return;
        }

        if (!player->GetEM()->IsEmpty(TRUE))
        {
            return;
        }

        if (ztimer->totalTime - this->prevPresetSwitchTimestamp < 1000)
        {
            return;
        }

        this->prevPresetSwitchTimestamp = ztimer->totalTime;

        auto it = this->Presets.find(PresetNr);
        if (it == this->Presets.end())
        {
            auto msg = zSTRING{ "Preset " } + zSTRING{ static_cast<int>(PresetNr) } + zSTRING{ " not found!" };
            ogame->GetTextView()->Printwin(msg);
            return;
        }

        this->StandUp();

        this->UnequipAll();
        this->ClearSpellBook();
        this->CurrentSlots.clear();

        {
            zSTRING msg = zSTRING{ "Switching preset: " } + zSTRING{ static_cast<int>(PresetNr) };
            ogame->GetTextView()->Printwin(msg);
        }

        zList<oCItem> itemsToBeEquipped;
        for (auto& presetItem : it->second.EquippedItems)
        {
            //auto item = player->IsInInv(zSTRING{ presetItem.c_str() }, 1);
            auto item = player->inventory2.IsInInvNotEquipped(zSTRING{ presetItem.c_str() }, 1, itemsToBeEquipped);
            if (item)
            {
                this->EquipItem(item, true);
                itemsToBeEquipped.Insert(item);
                //::Union::String(item->name).StdPrintLine();
            }
        }
        //itemsToBeEquipped.Clear();

        this->CurrentSlots = it->second.Slots;
        this->SyncPlaceholdersWithCurrentSlots();
        this->RestoreSpellBook();
    }

    void QuickSlots::UnequipSpellItems()
    {
        if (!player)
        {
            return;
        }

    #if ENGINE <= Engine_G1A
        player->inventory2.UnpackCategory(INV_RUNE);
        auto playerInv = player->inventory2.inventory[INV_RUNE].next;
    #else
        player->inventory2.UnpackAllItems();
        auto playerInv = player->inventory2.inventory.next;
    #endif
        while (playerInv)
        {
            auto item = playerInv->GetData();
            playerInv = playerInv->next;
            if (!item)
            {
                continue;
            }

            if (item->instanz < 0)
            {
                continue;
            }

            if (!item->HasFlag(ITM_FLAG_ACTIVE))
            {
                continue;
            }

            auto itemCat = player->inventory2.GetCategory(item);

            if (itemCat != INV_RUNE)
            {
                continue;
            }

            item->ClearFlag(ITM_FLAG_ACTIVE);
        }
    }

    void QuickSlots::UnequipAll()
    {
        player->inventory2.UnpackAllItems();
    #if ENGINE <= Engine_G1A
        for (int cat = 0; cat < INV_MAX; cat++)
        {
            auto playerInv = player->inventory2.inventory[cat].next;
    #else
            auto playerInv = player->inventory2.inventory.next;
    #endif
            while (playerInv)
            {
                auto item = playerInv->GetData();
                playerInv = playerInv->next;

                if (item->instanz < 0)
                {
                    continue;
                }

                if (!item->HasFlag(ITM_FLAG_ACTIVE))
                {
                    continue;
                }

                //this->EquipItem(item);
                player->UnequipItem(item);
            }
    #if ENGINE <= Engine_G1A
        }
    #endif
    }

    void QuickSlots::EquipItem(oCItem* item, bool forceEquip)
    {
        if (forceEquip)
        {
            player->Equip(item);
        }
        else
        {
            player->GetEM()->OnMessage(new oCMsgManipulate(oCMsgManipulate::EV_EQUIPITEM, item), item);
        }
    }

    void QuickSlots::UseItemFromSlot(size_t SlotNr)
    {
        if (!this->IsValidState())
        {
            return;
        }

        auto it = this->CurrentSlots.find(SlotNr);
        if (it == this->CurrentSlots.end())
        {
            return;
        }

        auto item = player->IsInInv(zSTRING{ it->second.c_str() }, 1);
        if (!item || item->amount <= 0)
        {
            return;
        }

        if (!this->CanUseItem(item))
        {
            player->DisplayCannotUse();
            return;
        }

        auto weaponMode = player->GetWeaponMode();
        auto playerSpellBook = player->GetSpellBook();
        auto currentWeapon = player->GetWeapon();
        //auto currentSpell = playerSpellBook && weaponMode == NPC_WEAPON_MAG ? playerSpellBook->GetSpellItem(playerSpellBook->GetSelectedSpell()) : nullptr;

        auto itemCat = player->inventory2.GetCategory(item);
        auto itemFlag = item->mainflag;
        auto itemIsActive = item->HasFlag(ITM_FLAG_ACTIVE);

        this->RefreshSpellBook();

        auto spellKey = this->GetSpellNoByItem(item);

        if (player->HasMagic() && weaponMode == NPC_WEAPON_MAG && spellKey > 0)
        {
            auto selectedSpell = playerSpellBook->GetSelectedSpell();
            
            if (selectedSpell)
            {
                auto selectedSpellItem = playerSpellBook->GetSpellItem(selectedSpell);
                if (selectedSpellItem && selectedSpellItem->instanz == item->instanz)
                {
                    this->HideWeapon();
                    return;
                }
            }
            
            auto spellNo = playerSpellBook->GetNoOfSpellByKey(spellKey);
            playerSpellBook->SetFrontSpell(spellNo - 1);
            if (auto spell = playerSpellBook->GetSelectedSpell())
            {
                zCView::stdoutput->PrintCXY(spell->GetName());
                zCView::stdoutput->PrintTimedCXY(spell->GetName(), 1000.0f, 0);
            }
            return;
        }

        auto itemScemeName = item->GetSchemeName();
        if (!itemScemeName.IsEmpty())
        {
            if ((player->GetLeftHand() || player->GetRightHand()) && weaponMode != NPC_WEAPON_NONE)
            {
                this->HideWeapon();
            }
            else if (player->GetLeftHand() || player->GetRightHand())
            {
                return;
            }

            this->StandUp();
            this->UseItemToState(item);
            return;
        }
        /* Fighting with torch? maybe one day
        if (item->GetInstanceName().Search({ "ITLSTORCH" }, 1U) >= 0 && ((!player->GetLeftHand() && weaponMode <= NPC_WEAPON_1HS) || (player->GetLeftHand() && player->GetLeftHand()->GetInstanceName().Search({ "ITLSTORCH" }, 1U) >= 0)))
        {
            this->StandUp();
            this->EquipItem(item);
            return;
        }
        */
        this->HideWeapon();

        if (itemCat == INV_COMBAT)
        {
            if (currentWeapon && currentWeapon == item)
            {
                return;
            }

            if (!itemIsActive)
            {
                this->StandUp();
                this->EquipItem(item);

                if (!this->drawWeaponOnSwitch)
                {
                    return;
                }
            }

            if (itemFlag == ITM_CAT_FF && player->IsMunitionAvailable(item))
            {
                this->DrawWeapon(NPC_WEAPON_BOW);
            }
            else if (itemFlag == ITM_CAT_NF)
            {
                this->DrawWeapon(NPC_WEAPON_FIST);
            }
        }
        else if (itemCat == INV_RUNE)
        {
            if (spellKey > 0)
            {
                auto spellNo = playerSpellBook->GetNoOfSpellByKey(spellKey);
                playerSpellBook->SetFrontSpell(spellNo - 1);
                this->DrawWeapon(NPC_WEAPON_MAG);
                if (auto spell = playerSpellBook->GetSelectedSpell())
                {
                    zCView::stdoutput->PrintCXY(spell->GetName());
                    zCView::stdoutput->PrintTimedCXY(spell->GetName(), 1000.0f, 0);
                }
            }
        }
        else
        {
            this->StandUp();
            this->EquipItem(item);
        }
    }

    void QuickSlots::UseItemToState(oCItem* item)
    {
        player->GetEM()->OnMessage(new oCMsgManipulate(oCMsgManipulate::EV_USEITEMTOSTATE, item, player->interactItemCurrentState + 1), item);
    }

    bool QuickSlots::CanUseItem(oCItem* item, bool hack)
    {
        CanUseHack = hack;
        auto result = player->CanUse(item);
        CanUseHack = false;
        return result;
    }
}