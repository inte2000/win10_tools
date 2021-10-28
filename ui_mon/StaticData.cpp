#include "stdafx.h"
#include "StaticData.h"



LANG_ITEM g_appLanguages[] =
{
    {_T("chs"), _T("Chinese Simplified")},
    {_T("cht"), _T("Chinese Traditional")},
    {_T("eng"), _T("English")}
};

int GetLanguagesCount()
{
    return sizeof(g_appLanguages) / sizeof(g_appLanguages[0]);
}
