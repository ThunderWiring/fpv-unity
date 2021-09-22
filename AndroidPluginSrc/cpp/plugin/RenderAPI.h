//
// Created by bassam on 07/09/2021.
//

#ifndef OPENNATIVEC_RENDERAPI_H
#define OPENNATIVEC_RENDERAPI_H

#include "unity/IUnityGraphics.h"
#include <stddef.h>

struct IUnityInterfaces;


// Super-simple "graphics abstraction". This is nothing like how a proper platform abstraction layer would look like;
// all this does is a base interface for whatever our plugin sample needs. Which is only "draw some triangles"
// and "modify a texture" at this point.
//
// There are implementations of this base class for D3D9, D3D11, OpenGL etc.; see individual RenderAPI_* files.
class RenderAPI
{
public:
    virtual ~RenderAPI() { }


    // Process general event like initialization, shutdown, device loss/reset etc.
    virtual void ProcessDeviceEvent(UnityGfxDeviceEventType type, IUnityInterfaces* interfaces) = 0;

    // Is the API using "reversed" (1.0 at near plane, 0.0 at far plane) depth buffer?
    // Reversed Z is used on modern platforms, and improves depth buffer precision.
    virtual bool GetUsesReverseZ() = 0;

    // End modifying texture data.
    virtual void EndModifyTexture(void* textureHandle, int textureWidth, int textureHeight, void* dataPtr) = 0;

    // End modifying vertex buffer data.
    virtual void EndModifyVertexBuffer(void* bufferHandle) = 0;
};


// Create a graphics API implementation instance for the given API type.
RenderAPI* CreateRenderAPI(UnityGfxRenderer apiType);

#endif //OPENNATIVEC_RENDERAPI_H
