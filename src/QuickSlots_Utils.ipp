namespace GOTHIC_NAMESPACE 
{
    bool CheckScriptFlag(const zSTRING& varName)
    {
        auto sym = parser->GetSymbol(varName);
        if (!sym || sym->type != zPAR_TYPE_INT)
        {
            return false;
        }

        int val = 0;
        sym->GetValue(val, 0);
        
        return val != 0;
    }

    bool IsValidItemViewport(zCViewBase* view)
    {
        if (!view || !zrenderer)
        {
            return false;
        }

        int x, y, width, height;
        view->GetViewport(x, y, width, height);
        const int screenWidth = zrenderer->vid_xdim;
        const int screenHeight = zrenderer->vid_ydim;

        return x >= 0 && y >= 0 && width > 0 && height > 0 &&
            screenWidth > 0 && screenHeight > 0 &&
            width <= screenWidth && height <= screenHeight &&
            x <= screenWidth - width && y <= screenHeight - height;
    }

    ::Union::String GetArchivePath()
    {
        return ::Union::String::Format("{0}{1}", zoptions->GetDirString(zTOptionPaths::DIR_SAVEGAMES).ToChar(), slInfo.GetSaveSlotName(slInfo.Slot));
    }
}
