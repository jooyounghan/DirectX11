#pragma once

#define AutoZeroMemory(a) ZeroMemory(&a, sizeof(decltype(a)));