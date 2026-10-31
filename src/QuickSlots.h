#pragma once

namespace GOTHIC_NAMESPACE 
{
	inline bool CanUseHack = false;
	inline bool hedinsInvDetected = false;
	inline std::vector<zCMenu*> pluginMenus;
    inline auto pluginName = zSTRING { "QuickSlots" };

    struct QSPreset
    {
        std::map<size_t, std::string> Slots;
        std::vector<std::string> EquippedItems;

        QSPreset(std::map<size_t, std::string> s, std::vector<std::string> ei) :
            Slots(s), EquippedItems(ei) {};
    };

    struct QS
    {
        size_t slot;
        oCItem* item;
        int inv;
        int amount;
        bool canUse;

        QS(size_t sl, oCItem* it, int ac, int am, bool cu) :
            slot(sl), item(it), inv(ac), amount(am), canUse(cu) {};
    };
    
    class QuickSlots
    {
    public:
        QuickSlots();
        ~QuickSlots();
        void LoadSettings();
        void Archive();
        void Unarchive();
        bool IsConsoleOpen();
        bool IsValidState(bool print = false);
        int GetModifierHotkey();
        int GetSlotNumberFromKey(int Key);
        int GetPresetNumberFromKey(int Key);
        int GetSpellNoByItem(oCItem* Item);
        bool HandleInput(int Key, bool Inv);
        void Loop();
        size_t GetSlotNumberFromItem(oCItem* item);
        size_t IsRegistered(oCItem* Item);
        void RegisterItem(oCItem* Item, size_t SlotNr);
        void RegisterSpell(oCItem* Item, size_t SlotNr);
        void DeRegisterSpell(size_t SlotNr);
        size_t GetBestFreeSlot();
        void ClearSpellBook();
        void RestoreSpellBook();
        void RefreshSpellBook();

        int TEXT_FONT_INVENTORY;
        size_t maxCellsInRow = 10;
        zCOLOR SlotNameColorBar;
        zCOLOR SlotNameColorBarAlt;
        zCOLOR SlotNameColorInv;
        zSTRING modifierHotkeyName;
        zSTRING switchVisibilityHotkeyName;
        zSTRING fistHotkeyName;
    private:
        oCItem* CreatePlaceholderForSlot(size_t slot, int itemIndex, int amount);
        void SyncPlaceholdersWithCurrentSlots();
        void Clear();
        void ClearSlots();
        void ClearPresets();
        void DrawFist();
        void PrintQuickSlots();
        void FillDefaultSlots();
        void DrawWeapon(int Mode, int Force = 0);
        void HideWeapon();
        void StandUp();
        void RegisterPreset(size_t PresetNr);
        void SwitchPreset(size_t PresetNr);
        void UnequipSpellItems();
        void UnequipAll();
        void EquipItem(oCItem* item, bool forceEquip = false);
        void UseItemFromSlot(size_t SlotNr);
        void UseItemToState(oCItem* item);
        bool CanUseItem(oCItem* item, bool hack = false);

        int cellSize = 70;
        int cellSizeX;
        int cellSizeY;
        unsigned long prevPresetSwitchTimestamp = 0;
        int hideEmptySlots;
        bool drawWeaponOnSwitch;
        bool switchPosY;
        bool hideBar;
        bool altBarAlwaysVisible;
        bool showEquippedSpells;
        bool enableFistDraw;
        bool showInfoInInventory;
        bool rotateUsedItem;
        size_t maxSlots = 20;
        int modifierHotkey = MOUSE_XBUTTON1;
        int switchVisibilityHotkey = KEY_I;
        int fistHotkey = KEY_GRAVE;
        std::map<size_t, std::string> CurrentSlots;
        std::map<size_t, QSPreset> Presets;
        std::map<size_t, oCItem*> SlotPlaceholders; // persistent fake items for registered slots
        zCView* textView;
        zCView* backView;
        zCView* itemView;
        zCView* itemViewEquipped;
        zCView* itemViewActive;
        zCWorld* itemWorld;
    };
	inline std::unique_ptr<QuickSlots> quickSlots;
}
