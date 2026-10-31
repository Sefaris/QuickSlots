#include <Psapi.h>
#include <vector>

namespace GOTHIC_NAMESPACE 
{
	void CheckModules()
	{
		const HANDLE process = GetCurrentProcess();
		std::vector<HMODULE> modules(128);
		for (int attempt = 0; attempt < 4; ++attempt)
		{
			DWORD bytesNeeded = 0;
			const DWORD bufferSize = static_cast<DWORD>(modules.size() * sizeof(HMODULE));
			if (!EnumProcessModules(process, modules.data(), bufferSize, &bytesNeeded))
			{
				return;
			}

			if (bytesNeeded <= bufferSize)
			{
				modules.resize(bytesNeeded / sizeof(HMODULE));
				break;
			}

			if (attempt == 3)
			{
				return;
			}
			modules.resize(bytesNeeded / sizeof(HMODULE));
		}

		for (const auto module : modules)
		{
			char moduleName[MAX_PATH] = {};
			if (!GetModuleBaseNameA(process, module, moduleName, sizeof(moduleName) - 1))
			{
				continue;
			}
			const ::Union::String mod(moduleName);
			if(mod.Contains("SIMPLE_INVENTORY", ::Union::StringBase::Flags::IgnoreCase))
			{
				hedinsInvDetected = true;
				zoptions->WriteInt(zSTRING{"HEDIN_SIMPLE_INVENTORY"}, zSTRING{"activeItemRenderMode"}, 0, false);
				break;
			}
			else if(mod.Contains("ADV_INVENTORY", ::Union::StringBase::Flags::IgnoreCase))
			{
				hedinsInvDetected = true;
				zoptions->WriteInt(zSTRING{"ADV_INVENTORY"}, zSTRING{"activeItemRenderMode"}, 0, false);
				break;
			}
		}
	}

	void Game_ApplySettings()
	{
		CheckModules();
		quickSlots->LoadSettings();
	}

	void Game_Entry()
	{

	}

	void Game_Init()
	{
		CheckModules();
		quickSlots = std::make_unique<QuickSlots>();
		quickSlots->LoadSettings();
	}

	void Game_Exit()
	{
		quickSlots.release();
		for (auto& pluginMenu : pluginMenus)
		{
			if (pluginMenu)
			{
				delete pluginMenu;
				pluginMenu = nullptr;
			}
		}
		pluginMenus.clear();
	}

	void Game_PreLoop()
	{

	}

	void Game_Loop()
	{
		quickSlots->Loop();
	}

	void Game_PostLoop()
	{

	}

	void Game_MenuLoop()
	{

	}

	void Game_SaveBegin()
	{
		quickSlots->ClearSpellBook();
	}

	void Game_SaveEnd()
	{
		quickSlots->Archive();
		quickSlots->RestoreSpellBook();
	}

	void LoadBegin()
	{

	}

	void LoadEnd()
	{
		quickSlots->Unarchive();
		quickSlots->RefreshSpellBook();
	}

	void Game_LoadBegin_NewGame()
	{
		LoadBegin();
	}

	void Game_LoadEnd_NewGame()
	{
		LoadEnd();
	}

	void Game_LoadBegin_SaveGame()
	{
		LoadBegin();
	}

	void Game_LoadEnd_SaveGame()
	{
		LoadEnd();
	}

	void Game_LoadBegin_ChangeLevel()
	{
		LoadBegin();
	}

	void Game_LoadEnd_ChangeLevel()
	{
		LoadEnd();
	}

	void Game_LoadBegin_Trigger()
	{

	}

	void Game_LoadEnd_Trigger()
	{

	}

	void Game_Pause()
	{

	}

	void Game_Unpause()
	{

	}

	void Game_DefineExternals()
	{

	}

	void __fastcall Game_Loop_Hook()
	{
		Game_Loop();
	}
	auto PartialHook_Game_Loop = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x0063DC76, 0x0066498B, 0x0066BA76, 0x006C87EB)), &Game_Loop_Hook);

	void __fastcall Game_Init_Hook()
	{
		Game_Init();
	}
	auto PartialHook_Game_Init = ::Union::CreatePartialHook((void*)(zSwitch(0x00637BB6, 0x0065DFFA, 0x00665112, 0x006C1BFF)), &Game_Init_Hook);
#if ENGINE >= Engine_G2
	auto PartialHook_Game_Init2 = ::Union::CreatePartialHook((void*)(zSwitch(0x00000000, 0x00000000, 0x006650F6, 0x006C1C1B)), &Game_Init_Hook);
#endif

	void __fastcall Game_Entry_Hook()
	{
		slInfo.Slot = -3;
		slInfo.LoadReason = LoadReason::NONE;
		Game_Entry();
	}
	auto PartialHook_Game_Entry = ::Union::CreatePartialHook((void*)(zSwitch(0x004F3E10 + 8, 0x00506810 + 10, 0x005000F0 + 8, 0x00502D70 + 5)), &Game_Entry_Hook);

	void __fastcall Game_Exit_Hook()
	{
		Game_Exit();
	}
	auto PartialHook_Game_Exit = ::Union::CreatePartialHook((void*)(zSwitch(0x00424850 + 7, 0x00427310 + 7, 0x004251A0 + 7, 0x004254E0 + 7)), &Game_Exit_Hook);

	void __fastcall Game_PreLoop_Hook()
	{
		Game_PreLoop();
	}
	auto PartialHook_Game_PreLoop = ::Union::CreatePartialHook((void*)(zSwitch(0x0063DBE0 + 7, 0x006648F0 + 7, 0x0066B930 + 7, 0x006C86A0 + 7)), &Game_PreLoop_Hook);

	void __fastcall Game_PostLoop_Hook()
	{
		Game_PostLoop();
	}
	auto PartialHook_Game_PostLoop = ::Union::CreatePartialHook((void*)(zSwitch(0x0063DF40, 0x00664CB8, 0x0066BD43, 0x006C8760)), &Game_PostLoop_Hook);
#if ENGINE >= Engine_G2
	auto PartialHook_Game_PostLoop2 = ::Union::CreatePartialHook((void*)(zSwitch(0x00000000, 0x00000000, 0x0066B9F0, 0x006C8AB8)), &Game_PostLoop_Hook);
#endif

	void __fastcall Game_MenuLoop_Hook()
	{
		Game_MenuLoop();
	}
	auto PartialHook_Game_MenuLoop = ::Union::CreatePartialHook((void*)(zSwitch(0x004D0E63, 0x004E15A8, 0x004DB349, 0x004DDCF9)), &Game_MenuLoop_Hook);
	auto PartialHook_Game_MenuLoop2 = ::Union::CreatePartialHook((void*)(zSwitch(0x004D0DF1, 0x004E1536, 0x004DB2D7, 0x004DDC87)), &Game_MenuLoop_Hook);

	void __fastcall Game_ApplyOptions_Hook()
	{
		Game_ApplySettings();
	}
	auto PartialHook_Game_ApplyOptions = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x00427993, 0x0042A48F, 0x00428EE4, 0x00429272)), &Game_ApplyOptions_Hook);

	void __fastcall Game_Pause_Hook()
	{
		Game_Pause();
	}
	auto PartialHook_Game_Pause = ::Union::CreatePartialHook((void*)(zSwitch(0x0063E1C2, 0x00664F66, 0x0066BFCA, 0x006C8D4A)), &Game_Pause_Hook);

	void __fastcall Game_Unpause_Hook()
	{
		Game_Unpause();
	}
	auto PartialHook_Game_Unpause = ::Union::CreatePartialHook((void*)(zSwitch(0x0063E2EE, 0x006650A9, 0x0066C0EE, 0x006C8E6E)), &Game_Unpause_Hook);

	void __fastcall Game_SaveBegin_Hook()
	{
		Game_SaveBegin();
	}
	auto PartialHook_Game_SaveBegin = ::Union::CreatePartialHook((void*)(zSwitch(0x0063AD80 + 8, 0x00661680 + 8, 0x006685D0 + 8, 0x006C5250 + 8)), &Game_SaveBegin_Hook);

	void __fastcall Game_SaveEnd_Hook()
	{
		Game_SaveEnd();
	}
	auto PartialHook_Game_SaveEnd = ::Union::CreatePartialHook((void*)(zSwitch(0x0063B668, 0x0066208D, 0x00668EC1, 0x006C5B41)), &Game_SaveEnd_Hook);

	void __fastcall Game_LoadBegin_NewGame_Hook()
	{
		Game_LoadBegin_NewGame();
	}
	auto PartialHook_Game_LoadBegin_NewGame = ::Union::CreatePartialHook((void*)(zSwitch(0x0063C070 + 9, 0x00662B20 + 9, 0x00669970 + 9, 0x006C65A0 + 9)), &Game_LoadBegin_NewGame_Hook);

	void __fastcall Game_LoadEnd_NewGame_Hook()
	{
		slInfo.Slot = -2;
		slInfo.LoadReason = LoadReason::NEW_GAME;
		Game_LoadEnd_NewGame();
	}
	auto PartialHook_Game_LoadEnd_NewGame = ::Union::CreatePartialHook((void*)(zSwitch(0x0063C28C, 0x00662D5A, 0x00669B8C, 0x006C67BC)), &Game_LoadEnd_NewGame_Hook);

	void __fastcall Game_LoadBegin_SaveGame_Hook()
	{
		Game_LoadBegin_SaveGame();
	}
	auto PartialHook_Game_LoadBegin_SaveGame = ::Union::CreatePartialHook((void*)(zSwitch(0x0063C2A0 + 7, 0x00662D60 + 7, 0x00669BA0 + 7, 0x006C67D0 + 7)), &Game_LoadBegin_SaveGame_Hook);

	void __fastcall Game_LoadEnd_SaveGame_Hook()
	{
		slInfo.LoadReason = LoadReason::LOAD_SAVE;
		Game_LoadEnd_SaveGame();
	}
	auto PartialHook_Game_LoadEnd_SaveGame = ::Union::CreatePartialHook((void*)(zSwitch(0x0063CD50, 0x0066393C, 0x0066A653, 0x006C7283)), &Game_LoadEnd_SaveGame_Hook);

	void __fastcall Game_LoadBegin_ChangeLevel_Hook()
	{
		Game_LoadBegin_ChangeLevel();
	}
	auto PartialHook_Game_LoadBegin_ChangeLevel = ::Union::CreatePartialHook((void*)(zSwitch(0x0063CD60 + 7, 0x00663950 + 7, 0x0066A660 + 7, 0x006C7290 + 7)), &Game_LoadBegin_ChangeLevel_Hook);

	void __fastcall Game_LoadEnd_ChangeLevel_Hook()
	{
		slInfo.LoadReason = LoadReason::WORLD_CHANGE;
		Game_LoadEnd_ChangeLevel();
	}
	auto PartialHook_Game_LoadEnd_ChangeLevel = ::Union::CreatePartialHook((void*)(zSwitch(0x0063D46B, 0x006640F0, 0x0066AD6B, 0x006C7ADD)), &Game_LoadEnd_ChangeLevel_Hook);

	void __fastcall Game_LoadBegin_Trigger_Hook()
	{
		Game_LoadBegin_Trigger();
	}
	auto PartialHook_Game_LoadBegin_Trigger = ::Union::CreatePartialHook((void*)(zSwitch(0x0063D480 + 10, 0x00664100 + 5, 0x0066AD80 + 10, 0x006C7AF0 + 10)), &Game_LoadBegin_Trigger_Hook);

	void __fastcall Game_LoadEnd_Trigger_Hook()
	{
		Game_LoadEnd_Trigger();
	}
	auto PartialHook_Game_LoadEnd_Trigger = ::Union::CreatePartialHook((void*)(zSwitch(0x0063D6BE, 0x0066433E, 0x0066AFBE, 0x006C7D2E)), &Game_LoadEnd_Trigger_Hook);

	void __fastcall Game_DefineExternals_Hook()
	{
		Game_DefineExternals();
	}
	auto PartialHook_Game_DefineExternals = ::Union::CreatePartialHook((void*)(zSwitch(0x0064E3C0, 0x00676CAF, 0x0067CC6A, 0x006D9B70)), &Game_DefineExternals_Hook);

	void __fastcall GetLastSaveSlot(::Union::Registers& reg)
	{
		int slotId = reg.ebp;

		if (slotId >= 0)
		{
			auto saveInfo = ogame->savegameManager->GetSavegame(slotId);
			if (!saveInfo || saveInfo->m_notCompatible || !saveInfo->DoesSavegameExist())
			{
				return;
			}
		}

		slInfo.Slot = slotId;
	}
	auto PartialHook_GetLastSaveSlot = ::Union::CreatePartialHook((void*)(zSwitch(0x0063AE90, 0x006617A4, 0x006686E0, 0x006C5360)), &GetLastSaveSlot);

	void __fastcall GetLastLoadSlot(::Union::Registers& reg)
	{
#if ENGINE == Engine_G1
		int slotId = reg.esi;
#else
		int slotId = reg.edi;
#endif

		if (slotId >= 0)
		{
			auto saveInfo = ogame->savegameManager->GetSavegame(slotId);
			if (!saveInfo || saveInfo->m_notCompatible || !saveInfo->DoesSavegameExist())
			{
				return;
			}
		}

		slInfo.Slot = slotId;
	}
	auto PartialHook_GetLastLoadSlot = ::Union::CreatePartialHook((void*)(zSwitch(0x00428B77, 0x0042AFDD, 0x00429D48, 0x0042A068)), &GetLastLoadSlot);
}
