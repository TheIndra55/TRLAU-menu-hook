#pragma once

// code related to the level event system which control the level flow

struct EventVar
{
    char* name;
    int offset;
};

struct UnsavedVar
{
    char* name;
    int* var;
};