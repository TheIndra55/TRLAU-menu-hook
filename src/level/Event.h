#pragma once

struct EventVar
{
    char* name;
    int variable;
};

struct UnsavedVar
{
    char* name;
    int* variable;
};

struct GlobalData
{
    char pad1[232];

    int* eventVars;
};

void* RELOC_GetProcAddress(void* peData, char* lpszName);