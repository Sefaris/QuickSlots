int ReadIntSafe(char const* text)
{
    zFILE* file = this->GetFile();
    if (!file)
    {
        return -1;
    }

    const long file_pos = file->Pos();
    const bool notValid = !file->SeekText(text).Length();

    file->Seek(file_pos);

    if (notValid)
    {
        return -1;
    }

    return this->ReadInt(text);
}