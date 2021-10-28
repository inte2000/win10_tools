#pragma once


typedef struct
{
    LPCTSTR langname;
    LPCTSTR langDisplayName;
}LANG_ITEM;

extern LANG_ITEM g_appLanguages[];
int GetLanguagesCount();