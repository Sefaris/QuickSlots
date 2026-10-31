META
{
  Parser = Menu;
  After = zUnionMenu.d;
  Namespace = QuickSlots;
};

// Namespace = QuickSlots
// Important !!!
// Supporting Unicode symbols.
// File encoding: UTF-8 (without BOM).

// ------ Constants ------
const int Start_PY  = 500;
const int Title_PY  = 450;
const int Menu_DY   = 550;
// Text
const int Text_PX   = 800;  // Position at left side
const int Text_SX   = 8000; // Size X
const int Text_SY   = 750;  // Size Y
const int Text_DY   = 0;  // Delta alignment
// Slider
const int Slider_PX = 4100; // Position at left side
const int Slider_SX = 3400; // Size X
const int Slider_SY = 600;  // Size Y
const int Slider_DY = 0;    // Delta alignment
// Choice
const int Choice_PX = 4100; // Position at left side
const int Choice_SX = 3400; // Size X
const int Choice_SY = 350;  // Size Y
const int Choice_DY = 0;  // Delta alignment

const string MenuBackPic   = "UnionMenu_BackPic.tga";
const string ItemBackPic   = "";
const string ChoiceBackPic = "MENU_CHOICE_BACK.TGA";
const string SliderBackPic = "MENU_SLIDER_BACK.TGA";
const string SliderPosPic  = "MENU_SLIDER_POS.TGA";
const string FontSmall     = "font_old_10_white.tga";
const string FontBig       = "font_old_20_white.tga";

var int CurrentMenuItem_PY;

// ------ Prototypes ------
func void InitializeBackPicturesAndFonts()
{
  MenuBackPic   = MENU_BACK_PIC;
  ItemBackPic   = MENU_ITEM_BACK_PIC;
  ChoiceBackPic = MENU_CHOICE_BACK_PIC;
  SliderBackPic = MENU_SLIDER_BACK_PIC;
  SliderPosPic  = MENU_SLIDER_POS_PIC;
  FontSmall     = MENU_FONT_SMALL;
  FontBig       = MENU_FONT_DEFAULT;
};

prototype C_EMPTY_MENU_DEF(C_MENU)
{
  InitializeBackPicturesAndFonts();
  C_MENU_DEF();
  backpic    = MenuBackPic;
  items[0]   = "";
  items[100] = "Union_menuitem_back";
  flags      = flags | MENU_SHOW_INFO;
};

instance C_MENU_ITEM_TEXT_BASE(C_MENU_ITEM_DEF)
{
  backpic        = ItemBackPic;
  posx           = Text_PX;
  posy           = Start_PY;
  dimx           = Text_SX;
  dimy           = Text_SY;
  flags          = flags | IT_EFFECTS_NEXT;
  onselaction[0] = SEL_ACTION_UNDEF;
};

instance C_MENUITEM_CHOICE_BASE(C_MENU_ITEM_DEF)
{
  backpic  = ChoiceBackPic;
  type     = MENU_ITEM_CHOICEBOX;
  fontname = FontSmall;
  posx     = Choice_PX;
  posy     = Start_PY + Choice_DY;
  dimx     = Choice_SX;
  dimy     = Choice_SY;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
};

instance C_MENUITEM_SLIDER_BASE(C_MENU_ITEM_DEF)
{
  backpic       = SliderBackPic;
  type          = MENU_ITEM_SLIDER;
  fontname      = FontSmall;
  posx          = Slider_PX;
  posy          = Start_PY + Slider_DY;
  dimx          = Slider_SX;
  dimy          = Slider_SY;
  flags         = flags & ~IT_SELECTABLE;
  flags         = flags | IT_TXT_CENTER;
  userstring[0] = SliderPosPic;
};

instance MenuItem_Opt_Headline(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = Title_PY;
  dimx    = 8100;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = Str_GetLocalizedString(
    "НАСТРОЙКИ QUICKSLOTS",
    "QUICKSLOTS SETTINGS",
    "QUICKSLOTS EINSTELLUNGEN",
    "USTAWIENIA QUICKSLOTS"
  );
};

// ------ Menu ------
instance MenuItem_Union_Auto_QuickSlots(C_MENU_ITEM_UNION_DEF)
{
  text[0]          = "QuickSlots";
  text[1] = Str_GetLocalizedString(
    "Наѝтройте параметры QuickSlots",
    "Configure QuickSlots settings",
    "QuickSlots-Einstellungen konfigurieren",
    "Zmień ustawienia QuickSlots"
  );
  onselaction[0]   = SEL_ACTION_STARTMENU;
  onselaction_s[0] = "QuickSlots:Menu_Opt_QuickSlots_Main";
};

instance Menu_Opt_QuickSlots_Main(C_EMPTY_MENU_DEF)
{
  Menu_SearchItems("QuickSlots:MENUITEM_OPT_QuickSlots_Main_*");
};

instance Menu_Opt_QuickSlots_Page1(C_EMPTY_MENU_DEF)
{
  Menu_SearchItems("QuickSlots:MENUITEM_OPT_QuickSlots_Page1_*");
};

instance Menu_Opt_QuickSlots_Page2(C_EMPTY_MENU_DEF)
{
  Menu_SearchItems("QuickSlots:MENUITEM_OPT_QuickSlots_Page2_*");
};

instance MenuItem_Opt_QuickSlots_Main_Headline(C_MENU_ITEM)
{
  MenuItem_Opt_Headline();
};

instance MenuItem_Opt_QuickSlots_Page1_Headline(C_MENU_ITEM)
{
  MenuItem_Opt_Headline();
};

instance MenuItem_Opt_QuickSlots_Page2_Headline(C_MENU_ITEM)
{
  MenuItem_Opt_Headline();
};

instance MenuItem_Opt_QuickSlots_Main_Page1(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 1;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;

  posx = 0;
  dimx = 8192;
  flags = flags | IT_TXT_CENTER;

  text[0] = "Settings - Page 1";

  onselaction[0]   = SEL_ACTION_STARTMENU;
  onselaction_s[0] = "QuickSlots:Menu_Opt_QuickSlots_Page1";
};

instance MenuItem_Opt_QuickSlots_Main_Page2(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 2;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;

  posx = 0;
  dimx = 8192;
  flags = flags | IT_TXT_CENTER;

  text[0] = "Settings - Page 2";

  onselaction[0]   = SEL_ACTION_STARTMENU;
  onselaction_s[0] = "QuickSlots:Menu_Opt_QuickSlots_Page2";
};

instance MenuItem_Opt_QuickSlots_Page1_QSEnableFistDraw(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 1;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "Fist draw";
  text[1] = "Enable fist draw on configured key.";
};

instance MenuItem_Opt_QuickSlots_Page1_QSEnableFistDraw_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "QSEnableFistDraw";
  onchgsetoptionsection = "QuickSlots";
  text[0]               = "OFF|ON";
};

instance MenuItem_Opt_QuickSlots_Page1_QSDrawWeaponOnSwitch(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 2;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "Draw weapon on switch";
  text[1] = "Automatically draw weapon when switching with QS.";

};

instance MenuItem_Opt_QuickSlots_Page1_QSDrawWeaponOnSwitch_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "QSDrawWeaponOnSwitch";
  onchgsetoptionsection = "QuickSlots";
  text[0]               = "OFF|ON";
};

instance MenuItem_Opt_QuickSlots_Page1_QSAltBarAlwaysVisible(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 3;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "Always visible alt bar";
  text[1] = "Alternative bar is alway visible.";
};

instance MenuItem_Opt_QuickSlots_Page1_QSAltBarAlwaysVisible_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "QSAltBarAlwaysVisible";
  onchgsetoptionsection = "QuickSlots";
  text[0]               = "OFF|ON";
};

instance MenuItem_Opt_QuickSlots_Page1_QSHideEmptySlots(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 4;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "Hide empty slots";
  text[1] = "Hide empty slots outside of inventory";
};

instance MenuItem_Opt_QuickSlots_Page1_QSHideEmptySlots_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "QSHideEmptySlots";
  onchgsetoptionsection = "QuickSlots";
  text[0]               = "OFF|ON|ON + Unusable Items";
};

instance MenuItem_Opt_QuickSlots_Page1_QSShowEquippedSpells(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 5;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "Show spells as equipped";
  text[1] = "Show spells on QS bar with equipped item background.";
};

instance MenuItem_Opt_QuickSlots_Page1_QSShowEquippedSpells_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "QSShowEquippedSpells";
  onchgsetoptionsection = "QuickSlots";
  text[0]               = "OFF|ON";
};

instance MenuItem_Opt_QuickSlots_Page1_QSShowInfoInInventory(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 6;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "Show info in inventory";
  text[1] = "Show control info in inventory.";
};

instance MenuItem_Opt_QuickSlots_Page1_QSShowInfoInInventory_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "QSShowInfoInInventory";
  onchgsetoptionsection = "QuickSlots";
  text[0]               = "OFF|ON";
};

instance MenuItem_Opt_QuickSlots_Page1_QSSwitchPosY(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 7;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "Switch Y position";
  text[1] = "Switch Y position of the QS bar outside the inventory.";
};

instance MenuItem_Opt_QuickSlots_Page1_QSSwitchPosY_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "QSSwitchPosY";
  onchgsetoptionsection = "QuickSlots";
  text[0]               = "OFF|ON";
};

instance MenuItem_Opt_QuickSlots_Page1_QSHideBar(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 8;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "Hide QS bar";
  text[1] = "Hide QS bar completely.";
};

instance MenuItem_Opt_QuickSlots_Page1_QSHideBar_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "QSHideBar";
  onchgsetoptionsection = "QuickSlots";
  text[0]               = "OFF|ON";
};

instance MenuItem_Opt_QuickSlots_Page1_QSRotateUsedItem(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 9;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "Rotate used item";
  text[1] = "Rotate item that is currently being used.";
};

instance MenuItem_Opt_QuickSlots_Page1_QSRotateUsedItem_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "QSRotateUsedItem";
  onchgsetoptionsection = "QuickSlots";
  text[0]               = "OFF|ON";
};

instance MenuItem_Opt_QuickSlots_Page2_QSCellSize(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 1;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;

  text[0] = "Cell size";
  text[1] = "Size of the item cell size on QS bar.";

  onSelAction[0]	=	SEL_ACTION_STARTITEM;
  onSelAction_S[0]=	"QUICKSLOTS:MENUITEM_OPT_QUICKSLOTS_PAGE2_QSCELLSIZE_CHOICE";
  onSelAction[1]	= 	SEL_ACTION_PLAY_SOUND;
  onSelAction_S[1]=	"MENU_SELECT";
};

instance MenuItem_Opt_QuickSlots_Page2_QSCellSize_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;

  text[1] 	=   "";
  type		= 	MENU_ITEM_INPUT;

  onChgSetOption 			= "QSCellSize";
  onChgSetOptionSection 	= "QuickSlots";
  flags		=	flags & ~IT_SELECTABLE;
};

instance MenuItem_Opt_QuickSlots_Page2_QSModifierHotkey(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 2;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;

  text[0] = "Modifier hotkey";
  text[1] = "press DEL key to empty slot and RETURN to define";

  onSelAction[0]	=	SEL_ACTION_EXECCOMMANDS;
  onSelAction_S[0]=	"RUN QUICKSLOTS:MENUITEM_OPT_QUICKSLOTS_PAGE2_QSMODIFIERHOTKEY_CHOICE";
};

instance MenuItem_Opt_QuickSlots_Page2_QSModifierHotkey_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;

  text[1] 	=   "Please press the desired key for this action.";
  type		= 	MENU_ITEM_INPUT;

  onChgSetOption 			= "keyQSModifierHotkey";
  onChgSetOptionSection 	= "KEYS";
  flags		=	flags & ~IT_SELECTABLE;
};

instance MenuItem_Opt_QuickSlots_Page2_QSSwitchVisibilityHotkey(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 3;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;

  text[0] = "Bar visibility hotkey";
  text[1] = "press DEL key to empty slot and RETURN to define";

  onSelAction[0]	=	SEL_ACTION_EXECCOMMANDS;
  onSelAction_S[0]=	"RUN QUICKSLOTS:MENUITEM_OPT_QUICKSLOTS_PAGE2_QSSWITCHVISIBILITYHOTKEY_CHOICE";
};

instance MenuItem_Opt_QuickSlots_Page2_QSSwitchVisibilityHotkey_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;

  text[1] 	=   "Please press the desired key for this action.";
  type		= 	MENU_ITEM_INPUT;

  onChgSetOption 			= "keyQSSwitchVisibilityHotkey";
  onChgSetOptionSection 	= "KEYS";
  flags		=	flags & ~IT_SELECTABLE;
};

instance MenuItem_Opt_QuickSlots_Page2_QSFistHotkey(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 4;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;

  text[0] = "Draw fist hotkey";
  text[1] = "press DEL key to empty slot and RETURN to define";

  onSelAction[0]	=	SEL_ACTION_EXECCOMMANDS;
  onSelAction_S[0]=	"RUN QUICKSLOTS:MENUITEM_OPT_QUICKSLOTS_PAGE2_QSFISTHOTKEY_CHOICE";
};

instance MenuItem_Opt_QuickSlots_Page2_QSFistHotkey_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;

  text[1] 	=   "Please press the desired key for this action.";
  type		= 	MENU_ITEM_INPUT;

  onChgSetOption 			= "keyQSFistHotkey";
  onChgSetOptionSection 	= "KEYS";
  flags		=	flags & ~IT_SELECTABLE;
};

instance MenuItem_Opt_QuickSlots_Page2_QSInventorySlotTexture(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 5;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;

  text[0] = "Item texture";
  text[1] = "Standard QS background texture for item.";

  onSelAction[0]	=	SEL_ACTION_STARTITEM;
  onSelAction_S[0]=	"QUICKSLOTS:MENUITEM_OPT_QUICKSLOTS_PAGE2_QSINVENTORYSLOTTEXTURE_CHOICE";
  onSelAction[1]	= 	SEL_ACTION_PLAY_SOUND;
  onSelAction_S[1]=	"MENU_SELECT";
};

instance MenuItem_Opt_QuickSlots_Page2_QSInventorySlotTexture_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;

  text[1] 	=   "";
  type		= 	MENU_ITEM_INPUT;

  onChgSetOption 			= "QSInventorySlotTexture";
  onChgSetOptionSection 	= "QuickSlots";
  flags		=	flags & ~IT_SELECTABLE;
};

instance MenuItem_Opt_QuickSlots_Page2_QSInventorySlotEquippedTexture(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 6;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;

  text[0] = "Equipped item texture";
  text[1] = "QS background texture for equipped item.";

  onSelAction[0]	=	SEL_ACTION_STARTITEM;
  onSelAction_S[0]=	"QUICKSLOTS:MENUITEM_OPT_QUICKSLOTS_PAGE2_QSINVENTORYSLOTEQUIPPEDTEXTURE_CHOICE";
  onSelAction[1]	= 	SEL_ACTION_PLAY_SOUND;
  onSelAction_S[1]=	"MENU_SELECT";
};

instance MenuItem_Opt_QuickSlots_Page2_QSInventorySlotEquippedTexture_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;

  text[1] 	=   "";
  type		= 	MENU_ITEM_INPUT;

  onChgSetOption 			= "QSInventorySlotEquippedTexture";
  onChgSetOptionSection 	= "QuickSlots";
  flags		=	flags & ~IT_SELECTABLE;
};

instance MenuItem_Opt_QuickSlots_Page2_QSInventorySlotActiveTexture(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 7;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;

  text[0] = "Active item texture";
  text[1] = "QS background texture for currently active item.";

  onSelAction[0]	=	SEL_ACTION_STARTITEM;
  onSelAction_S[0]=	"QUICKSLOTS:MENUITEM_OPT_QUICKSLOTS_PAGE2_QSINVENTORYSLOTACTIVETEXTURE_CHOICE";
  onSelAction[1]	= 	SEL_ACTION_PLAY_SOUND;
  onSelAction_S[1]=	"MENU_SELECT";
};

instance MenuItem_Opt_QuickSlots_Page2_QSInventorySlotActiveTexture_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;

  text[1] 	=   "";
  type		= 	MENU_ITEM_INPUT;

  onChgSetOption 			= "QSInventorySlotActiveTexture";
  onChgSetOptionSection 	= "QuickSlots";
  flags		=	flags & ~IT_SELECTABLE;
};
