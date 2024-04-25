#pragma once
#include <windows.h>

#define AutoZeroMemory(a) ZeroMemory(&a, sizeof(decltype(a)));
#define AutoDelete(a) if(a == nullptr) { delete a; a = nullptr; }

#define NOT_SELECTED -1
#define NEAR_CLOSE 0.001f;

#define DEFAULT_CUBE_LEVEL		15.f

#define DRAG_DROP_EXR_KEY				"EXRFile"
#define DRAG_DROP_IBL_KEY				"IBLTextureFile"
#define DRAG_DROP_TEXTURE_KEY			"ImageFile"
#define DRAG_DROP_MESH_KEY				"MeshFile"
#define DRAG_DROP_BONE_KEY				"BoneFile"
#define DRAG_DROP_MATERIAL_KEY			"MaterialFile"
#define DRAG_DROP_ANIMATION_KEY			"AnimationFile"
