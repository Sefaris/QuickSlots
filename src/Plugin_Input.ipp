namespace GOTHIC_NAMESPACE 
{
	int HexDigit(const char c)
	{
		if (c >= '0' && c <= '9')
		{
			return c - '0';
		}

		if (c >= 'a' && c <= 'f')
		{
			return 10 + c - 'a';
		}

		if (c >= 'A' && c <= 'F')
		{
			return 10 + c - 'A';
		}

		throw std::runtime_error("non-hex digit");
	}

	uint8_t HexByte(const std::string& s, const size_t pos)
	{
		return static_cast<uint8_t>((HexDigit(s[pos]) << 4) | HexDigit(s[pos + 1]));
	}

	std::vector<uint16_t> HexToWordsLE(const std::string& hex)
	{
		if ((hex.size() % 2) != 0)
		{
			throw std::runtime_error("odd hex length");
		}

		std::vector<uint8_t> bytes;
		bytes.reserve(hex.size() / 2);

		for (size_t i = 0; i < hex.size(); i += 2)
		{
			bytes.push_back(HexByte(hex, i));
		}

		std::vector<uint16_t> words;
		words.reserve(bytes.size() / 2);

		for (size_t i = 0; i + 1 < bytes.size(); i += 2)
		{
			words.push_back(static_cast<uint16_t>(bytes[i] | (bytes[i + 1] << 8))); // LE
		}

		return words;
	}

	std::string WordsLEToHex(const std::vector<uint16_t>& words)
	{
		std::string out;
		out.reserve(words.size() * 4);

		char buf[5] = {};
		for (const uint16_t w : words)
		{
			std::snprintf(buf, sizeof(buf), "%02x%02x", int(w & 0xFF), int((w >> 8) & 0xFF));
			out += buf;
		}

		return out;
	}

	uint16_t HexToWordLE(const std::string& hex)
	{
		return HexToWordsLE(hex).at(0);
	}

	std::string WordLEToHex(const uint16_t w)
	{
		return WordsLEToHex({ w });
	}

	PluginInput::PluginInput()
	{
		for (int i = 0; i < KEY_EVENT_TABLE_SIZE; i++)
		{
			this->pluginKeyEvent[i] = false;
			this->pluginKeyToggle[i] = false;
		}

		this->NameToKey =
		{
			{ "ESCAPE", KEY_ESCAPE },
			{ "1", KEY_1 },
			{ "2", KEY_2 },
			{ "3", KEY_3 },
			{ "4", KEY_4 },
			{ "5", KEY_5 },
			{ "6", KEY_6 },
			{ "7", KEY_7 },
			{ "8", KEY_8 },
			{ "9", KEY_9 },
			{ "0", KEY_0 },
			{ "MINUS", KEY_MINUS },
			{ "EQUALS", KEY_EQUALS },
			{ "BACK", KEY_BACK },
			{ "TAB", KEY_TAB },
			{ "Q", KEY_Q },
			{ "W", KEY_W },
			{ "E", KEY_E },
			{ "R", KEY_R },
			{ "T", KEY_T },
			{ "Y", KEY_Y },
			{ "U", KEY_U },
			{ "I", KEY_I },
			{ "O", KEY_O },
			{ "P", KEY_P },
			{ "LBRACKET", KEY_LBRACKET },
			{ "RBRACKET", KEY_RBRACKET },
			{ "RETURN", KEY_RETURN },
			{ "LCONTROL", KEY_LCONTROL },
			{ "A", KEY_A },
			{ "S", KEY_S },
			{ "D", KEY_D },
			{ "F", KEY_F },
			{ "G", KEY_G },
			{ "H", KEY_H },
			{ "J", KEY_J },
			{ "K", KEY_K },
			{ "L", KEY_L },
			{ "SEMICOLON", KEY_SEMICOLON },
			{ "APOSTROPHE", KEY_APOSTROPHE },
			{ "GRAVE", KEY_GRAVE },
			{ "LSHIFT", KEY_LSHIFT },
			{ "BACKSLASH", KEY_BACKSLASH },
			{ "Z", KEY_Z },
			{ "X", KEY_X },
			{ "C", KEY_C },
			{ "V", KEY_V },
			{ "B", KEY_B },
			{ "N", KEY_N },
			{ "M", KEY_M },
			{ "COMMA", KEY_COMMA },
			{ "PERIOD", KEY_PERIOD },
			{ "SLASH", KEY_SLASH },
			{ "RSHIFT", KEY_RSHIFT },
			{ "MULTIPLY", KEY_MULTIPLY },
			{ "LMENU", KEY_LMENU },
			{ "SPACE", KEY_SPACE },
			{ "CAPITAL", KEY_CAPITAL },
			{ "F1", KEY_F1 },
			{ "F2", KEY_F2 },
			{ "F3", KEY_F3 },
			{ "F4", KEY_F4 },
			{ "F5", KEY_F5 },
			{ "F6", KEY_F6 },
			{ "F7", KEY_F7 },
			{ "F8", KEY_F8 },
			{ "F9", KEY_F9 },
			{ "F10", KEY_F10 },
			{ "NUMLOCK", KEY_NUMLOCK },
			{ "SCROLL", KEY_SCROLL },
			{ "NUMPAD7", KEY_NUMPAD7 },
			{ "NUMPAD8", KEY_NUMPAD8 },
			{ "NUMPAD9", KEY_NUMPAD9 },
			{ "SUBTRACT", KEY_SUBTRACT },
			{ "NUMPAD4", KEY_NUMPAD4 },
			{ "NUMPAD5", KEY_NUMPAD5 },
			{ "NUMPAD6", KEY_NUMPAD6 },
			{ "ADD", KEY_ADD },
			{ "NUMPAD1", KEY_NUMPAD1 },
			{ "NUMPAD2", KEY_NUMPAD2 },
			{ "NUMPAD3", KEY_NUMPAD3 },
			{ "NUMPAD0", KEY_NUMPAD0 },
			{ "DECIMAL", KEY_DECIMAL },
			{ "OEM_102", KEY_OEM_102 },
			{ "F11", KEY_F11 },
			{ "F12", KEY_F12 },
			{ "F13", KEY_F13 },
			{ "F14", KEY_F14 },
			{ "F15", KEY_F15 },
			{ "KANA", KEY_KANA },
			{ "ABNT_C1", KEY_ABNT_C1 },
			{ "CONVERT", KEY_CONVERT },
			{ "NOCONVERT", KEY_NOCONVERT },
			{ "YEN", KEY_YEN },
			{ "ABNT_C2", KEY_ABNT_C2 },
			{ "NUMPADEQUALS", KEY_NUMPADEQUALS },
			{ "PREVTRACK", KEY_PREVTRACK },
			{ "AT", KEY_AT },
			{ "COLON", KEY_COLON },
			{ "UNDERLINE", KEY_UNDERLINE },
			{ "KANJI", KEY_KANJI },
			{ "STOP", KEY_STOP },
			{ "AX", KEY_AX },
			{ "UNLABELED", KEY_UNLABELED },
			{ "NEXTTRACK", KEY_NEXTTRACK },
			{ "NUMPADENTER", KEY_NUMPADENTER },
			{ "RCONTROL", KEY_RCONTROL },
			{ "MUTE", KEY_MUTE },
			{ "CALCULATOR", KEY_CALCULATOR },
			{ "PLAYPAUSE", KEY_PLAYPAUSE },
			{ "MEDIASTOP", KEY_MEDIASTOP },
			{ "VOLUMEDOWN", KEY_VOLUMEDOWN },
			{ "VOLUMEUP", KEY_VOLUMEUP },
			{ "WEBHOME", KEY_WEBHOME },
			{ "NUMPADCOMMA", KEY_NUMPADCOMMA },
			{ "DIVIDE", KEY_DIVIDE },
			{ "SYSRQ", KEY_SYSRQ },
			{ "RMENU", KEY_RMENU },
			{ "PAUSE", KEY_PAUSE },
			{ "HOME", KEY_HOME },
			{ "UP", KEY_UP },
			{ "PRIOR", KEY_PRIOR },
			{ "LEFT", KEY_LEFT },
			{ "RIGHT", KEY_RIGHT },
			{ "END", KEY_END },
			{ "DOWN", KEY_DOWN },
			{ "NEXT", KEY_NEXT },
			{ "INSERT", KEY_INSERT },
			{ "DELETE", KEY_DELETE },
			{ "LWIN", KEY_LWIN },
			{ "RWIN", KEY_RWIN },
			{ "APPS", KEY_APPS },
			{ "POWER", KEY_POWER },
			{ "SLEEP", KEY_SLEEP },
			{ "WAKE", KEY_WAKE },
			{ "WEBSEARCH", KEY_WEBSEARCH },
			{ "WEBFAVORITES", KEY_WEBFAVORITES },
			{ "WEBREFRESH", KEY_WEBREFRESH },
			{ "WEBSTOP", KEY_WEBSTOP },
			{ "WEBFORWARD", KEY_WEBFORWARD },
			{ "WEBBACK", KEY_WEBBACK },
			{ "MYCOMPUTER", KEY_MYCOMPUTER },
			{ "MAIL", KEY_MAIL },
			{ "MEDIASELECT", KEY_MEDIASELECT },
			{ "BACKSPACE", KEY_BACKSPACE },
			{ "NUMPADSTAR", KEY_NUMPADSTAR },
			{ "LALT", KEY_LALT },
			{ "CAPSLOCK", KEY_CAPSLOCK },
			{ "NUMPADMINUS", KEY_NUMPADMINUS },
			{ "NUMPADPLUS", KEY_NUMPADPLUS },
			{ "NUMPADPERIOD", KEY_NUMPADPERIOD },
			{ "NUMPADSLASH", KEY_NUMPADSLASH },
			{ "RALT", KEY_RALT },
			{ "UPARROW", KEY_UPARROW },
			{ "PGUP", KEY_PGUP },
			{ "LEFTARROW", KEY_LEFTARROW },
			{ "RIGHTARROW", KEY_RIGHTARROW },
			{ "DOWNARROW", KEY_DOWNARROW },
			{ "PGDN", KEY_PGDN },
			{ "CIRCUMFLEX", KEY_CIRCUMFLEX },
			{ "MOUSE_BUTTONLEFT", MOUSE_BUTTONLEFT },
			{ "MOUSE_BUTTONRIGHT", MOUSE_BUTTONRIGHT },
			{ "MOUSE_BUTTONMID", MOUSE_BUTTONMID },
			{ "MOUSE_XBUTTON1", MOUSE_XBUTTON1 },
			{ "MOUSE_XBUTTON2", MOUSE_XBUTTON2 },
			{ "MOUSE_XBUTTON3", MOUSE_XBUTTON3 },
			{ "MOUSE_XBUTTON4", MOUSE_XBUTTON4 },
			{ "MOUSE_XBUTTON5", MOUSE_XBUTTON5 },
			{ "MOUSE_WHEELUP", MOUSE_WHEELUP },
			{ "MOUSE_WHEELDOWN", MOUSE_WHEELDOWN }
		};

		this->NameToLogicalKey =
		{
			{ "GAME_LEFT", GAME_LEFT },
			{ "GAME_RIGHT", GAME_RIGHT },
			{ "GAME_UP", GAME_UP },
			{ "GAME_DOWN", GAME_DOWN },
			{ "GAME_ACTION", GAME_ACTION },
			{ "GAME_SLOW", GAME_SLOW },
			{ "GAME_ACTION2", GAME_ACTION2 },
			{ "GAME_WEAPON", GAME_WEAPON },
			{ "GAME_SMOVE", GAME_SMOVE },
			{ "GAME_SMOVE2", GAME_SMOVE2 },
			{ "GAME_SHIFT", GAME_SHIFT },
			{ "GAME_END", GAME_END },
			{ "GAME_INVENTORY", GAME_INVENTORY },
			{ "GAME_LOOK", GAME_LOOK },
			{ "GAME_SNEAK", GAME_SNEAK },
			{ "GAME_STRAFELEFT", GAME_STRAFELEFT },
			{ "GAME_STRAFERIGHT", GAME_STRAFERIGHT },
			{ "GAME_SCREEN_STATUS", GAME_SCREEN_STATUS },
			{ "GAME_SCREEN_LOG", GAME_SCREEN_LOG },
			{ "GAME_SCREEN_MAP", GAME_SCREEN_MAP },
			{ "GAME_LOOK_FP", GAME_LOOK_FP },
			{ "GAME_LOCK_TARGET", GAME_LOCK_TARGET },
			{ "GAME_PARADE", GAME_PARADE },
			{ "GAME_ACTIONLEFT", GAME_ACTIONLEFT },
			{ "GAME_ACTIONRIGHT", GAME_ACTIONRIGHT },
			{ "GAME_LAME_POTION", GAME_LAME_POTION },
			{ "GAME_LAME_HEAL", GAME_LAME_HEAL }
		};
	}

	bool PluginInput::KeyPressed(const int keyID)
	{
		if (keyID < 0 || keyID >= KEY_EVENT_TABLE_SIZE)
		{
			return false;
		}

	/*#if ENGINE == Engine_G1
		if (keyID >= MAX_MOUSE_BTNS)
		{  
			return zinput->KeyPressed(keyID);
		}
	#endif*/

		return this->pluginKeyEvent[keyID];
	}

	bool PluginInput::KeyToggled(const int keyID)
	{
		if (keyID < 0 || keyID >= KEY_EVENT_TABLE_SIZE)
		{
			return false;
		}

	/*#if ENGINE == Engine_G1
		if (keyID >= MAX_MOUSE_BTNS)
		{  
			return zinput->KeyToggled(keyID);
		}
	#endif*/

		auto res = this->pluginKeyToggle[keyID];
		this->pluginKeyToggle[keyID] = false;
		return res;
	}

	void PluginInput::ClearKeyBuffer()
	{
		for (int i = 0; i < KEY_EVENT_TABLE_SIZE; i++)
		{
			this->pluginKeyEvent[i] = false;
			this->pluginKeyToggle[i] = false;
		}
	}

	void PluginInput::UpdateFromEngine()
	{
		if(!zinput)
		{
			return;
		}

		bool* keyEvent = reinterpret_cast<bool*>(zSwitch(0x0086ccc8, 0x008b27c0, 0x008c3020, 0x008d1678));
		bool* keyToggle = reinterpret_cast<bool*>(zSwitch(0x0086ced0, 0x008b29d4, 0x008c3260, 0x008d18b8));
		
		for (int i = 0; i < ENGINE_INPUT_ARRAY_SIZE; i++)
		{
			this->pluginKeyEvent[i] = keyEvent[i];
			this->pluginKeyToggle[i] = keyToggle[i];
		}

	#if ENGINE == Engine_G1
		constexpr int VK_Buttons[8] = {
			VK_LBUTTON,  // 0
			VK_RBUTTON,  // 1  
			VK_MBUTTON,  // 2
			VK_XBUTTON1, // 3
			VK_XBUTTON2, // 4
			0,           // 5 - no VK code for X3
			0,           // 6 - no VK code for X4
			0            // 7 - no VK code for X5
		};

		for(int i = 0; i < 8; i++)
		{
			auto keyId = ButtonMap[i];
			this->pluginKeyToggle[keyId] = false;
			mouseState.buttonsToggled[i].store(false);
		}

		for(int i = 0; i < 5; i++)
		{
			auto keyId = ButtonMap[i];
			//keyToggle[keyId] = false;
			
			bool pressed = (GetAsyncKeyState(VK_Buttons[i]) & 0x8000) != 0;
			bool wasPressed = mouseState.buttonsPressed[i].load();
			
			mouseState.buttonsPressed[i].store(pressed);
			
			this->pluginKeyEvent[keyId] = pressed;
			if (pressed)
			{
				mouseState.buttonsToggled[i].store(wasPressed);
				this->pluginKeyToggle[keyId] = !wasPressed;
			}
		}
	#endif
	}

	int PluginInput::GetKeyRaw(const zSTRING& keyName, int keyDefault, const zSTRING& keyOverrideName)
	{
		bool keyOverride = false;
		if(zoptions->EntryExists(pluginName, keyOverrideName))
		{
			auto tmpHotkey = pluginInput.GetKey(zoptions->ReadString(pluginName, keyOverrideName, "").ToChar());
			if (tmpHotkey > 0)
			{
				keyDefault = tmpHotkey;
				keyOverride = true;
			}
			zoptions->WriteString(pluginName, keyOverrideName, "", FALSE);
		}

		if (keyOverride)
		{
			zoptions->WriteString("KEYS", keyName, WordLEToHex(static_cast<uint16_t>(keyDefault)).c_str(), FALSE);
		}

		auto rawString = zoptions->ReadString("KEYS", keyName, WordLEToHex(static_cast<uint16_t>(keyDefault)).c_str());

		return static_cast<int>(HexToWordLE(rawString.ToChar()));
	}

	int PluginInput::GetKey(std::string keyName)
	{
		std::transform(keyName.begin(), keyName.end(), keyName.begin(), ::toupper);
		
		std::string rep("KEY_");
		auto pos = keyName.find(rep);
		if(pos != std::string::npos)
		{
			keyName.replace(pos, rep.size(), "");
		}
		
		auto it = this->NameToKey.find(keyName);
		if (it != this->NameToKey.end())
		{
			return it->second;
		}

		return -1;
	}

	std::string PluginInput::GetKeyName(int key)
	{
		for(auto& keyPair : this->NameToKey)
		{
			if(keyPair.second == key)
			{
				return keyPair.first;
			}
		}

		return "";
	}

	int PluginInput::GetLogicalKey(std::string keyName)
	{
		if (auto it = this->NameToLogicalKey.find(keyName); it != this->NameToLogicalKey.end())
		{
			return it->second;
		}

		return -1;
	}

    void __fastcall PluginInput_UpdateDInput(::Union::Registers& reg)
    {
        pluginInput.UpdateFromEngine();
    }
    auto PartialHook_PluginInput_UpdateDInput = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x004c7f05, 0x004d7a50, 0x004d176a, 0x004d3d2a)), &PluginInput_UpdateDInput);

    void __fastcall PluginInput_ClearKeyBuffer(::Union::Registers& reg)
    {
        pluginInput.ClearKeyBuffer();
    }
    auto PartialHook_PluginInput_ClearKeyBuffer = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x004c8b94, 0x004d8cb0, 0x004d312e, 0x004d56ee)), &PluginInput_ClearKeyBuffer);
}