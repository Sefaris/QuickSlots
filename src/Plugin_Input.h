#pragma once

namespace GOTHIC_NAMESPACE 
{
#if ENGINE == Engine_G1
	struct MouseState {// 0=Left,1=Right,2=Middle,3=X1,4=X2,5=X3,6=X4,7=X5
		std::array<std::atomic<bool>, 8> buttonsPressed{};
		std::array<std::atomic<bool>, 8> buttonsToggled{};
	};
	inline MouseState mouseState;

	inline constexpr int ButtonMap[8] =
	{
		MOUSE_BUTTONLEFT,  MOUSE_BUTTONRIGHT, MOUSE_BUTTONMID, 
		MOUSE_XBUTTON1,   MOUSE_XBUTTON2,   MOUSE_XBUTTON3, 
		MOUSE_XBUTTON4,   MOUSE_XBUTTON5
	};
#endif

    #define ENGINE_INPUT_ARRAY_SIZE zSwitch(515,532,576,576)
    #define KEY_EVENT_TABLE_SIZE (JOY_BUTTON_32 + 1)

	int HexDigit(const char c);
	uint8_t HexByte(const std::string& s, const size_t pos);
	std::vector<uint16_t> HexToWordsLE(const std::string& hex);
	std::string WordsLEToHex(const std::vector<uint16_t>& words);
	uint16_t HexToWordLE(const std::string& hex);
	std::string WordLEToHex(const uint16_t w);

    class PluginInput
    {
        private:
            bool pluginKeyEvent[KEY_EVENT_TABLE_SIZE];
            bool pluginKeyToggle[KEY_EVENT_TABLE_SIZE];
			std::map<std::string, int> NameToLogicalKey;
			std::map<std::string, int> NameToKey;

        public:
			PluginInput();
            bool KeyPressed(const int keyID);
            bool KeyToggled(const int keyID);
            void ClearKeyBuffer();
            void UpdateFromEngine();
			int GetKeyRaw(const zSTRING& keyName, int keyDefault, const zSTRING& keyOverrideName);
            int GetKey(std::string keyName);
            std::string GetKeyName(int key);
            int GetLogicalKey(std::string keyName);
	};
	inline PluginInput pluginInput;
}
