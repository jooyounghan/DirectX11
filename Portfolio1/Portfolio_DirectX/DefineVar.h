#pragma once

#define AutoZeroMemory(a) ZeroMemory(&a, sizeof(decltype(a)));
#define AutoDelete(a) if(a == nullptr) { delete a; a = nullptr; }