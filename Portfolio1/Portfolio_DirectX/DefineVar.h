#pragma once
#include <windows.h>

#define AutoZeroMemory(a) ZeroMemory(&a, sizeof(decltype(a)));
#define AutoDelete(a) if(a == nullptr) { delete a; a = nullptr; }

#define NOT_SELECTED -1
#define NEAR_CLOSE 0.0001f;

#define DEFAULT_CUBE_LEVEL		15.f

#define DRAG_DROP_IBL_KEY		"CubeMap"
#define DRAG_DROP_TEXTURE_KEY	"Texture2D"
#define DRAG_DROP_MODEL_KEY		"ModelFile"
#define DRAG_DROP_SKELETAL_KEY	"SkeletalFile"