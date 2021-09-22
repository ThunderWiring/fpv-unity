//
// Created by bassam on 07/09/2021.
//

/**
 * Relies heavily on the example set by Unity in this repo:
 * https://docs.unity3d.com/Manual/NativePluginInterface.html
 * https://github.com/Unity-Technologies/NativeRenderingPlugin/blob/master/PluginSource/source/RenderingPlugin.cpp
 * */

#include "plugin/PlatformBase.h"
#include "plugin/RenderAPI.h"
#include "../macros.h"

#include <cassert>
#include <cmath>
#include <vector>
#include <thread>
#include "mapper_cv/opencv_reader.h"


// --------------------------------------------------------------------------
// SetTimeFromUnity, an example function we export which is called by one of the scripts.

static float g_Time;
static OpenCameraReader cameraReader;
static void *g_TextureHandle = nullptr;
static int g_TextureWidth = 0;
static int g_TextureHeight = 0;

unsigned char* getVrView();

EXPORTED void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetTimeFromUnity(float t) { g_Time = t; }

EXPORTED void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
SetTextureFromUnity(void *textureHandle, int w, int h) {
    LOGD("45rt66t", "SetTextureFromUnity Width: %d, height: %d", w, h);
    // A script calls this at initialization time; just remember the texture pointer here.
    // Will update texture pixels each frame from the plugin rendering event (texture update
    // needs to happen on the rendering thread).
    g_TextureHandle = textureHandle;
    g_TextureWidth = w;
    g_TextureHeight = h;
}

// --------------------------------------------------------------------------
// SetMeshBuffersFromUnity, an example function we export which is called by one of the scripts.

static void *g_VertexBufferHandle = nullptr;
static int g_VertexBufferVertexCount;

struct MeshVertex {
    float pos[3];
    float normal[3];
    float color[4];
    float uv[2];
};
static std::vector<MeshVertex> g_VertexSource;

// --------------------------------------------------------------------------
// UnitySetInterfaces

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType);

static IUnityInterfaces *s_UnityInterfaces = nullptr;
static IUnityGraphics *s_Graphics = nullptr;

EXPORTED void    UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
UnityPluginLoad(IUnityInterfaces *unityInterfaces) {
    s_UnityInterfaces = unityInterfaces;
    s_Graphics = s_UnityInterfaces->Get<IUnityGraphics>();
    s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);

#if SUPPORT_VULKAN
    if (s_Graphics->GetRenderer() == kUnityGfxRendererNull)
    {
        extern void RenderAPI_Vulkan_OnPluginLoad(IUnityInterfaces*);
        RenderAPI_Vulkan_OnPluginLoad(unityInterfaces);
    }
#endif // SUPPORT_VULKAN

    // Run OnGraphicsDeviceEvent(initialize) manually on plugin load
    OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload() {
    if (s_Graphics) {
        s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
    }
}

#if UNITY_WEBGL
typedef void	(UNITY_INTERFACE_API * PluginLoadFunc)(IUnityInterfaces* unityInterfaces);
typedef void	(UNITY_INTERFACE_API * PluginUnloadFunc)();

extern "C" void	UnityRegisterRenderingPlugin(PluginLoadFunc loadPlugin, PluginUnloadFunc unloadPlugin);

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API RegisterPlugin()
{
    LOGD("45rt66t", "RegisterPlugin");
    UnityRegisterRenderingPlugin(UnityPluginLoad, UnityPluginUnload);
}
#endif

// --------------------------------------------------------------------------
// GraphicsDeviceEvent


static RenderAPI *s_CurrentAPI = nullptr;
static UnityGfxRenderer s_DeviceType = kUnityGfxRendererNull;

EXPORTED void UNITY_INTERFACE_API SetVrMapView() {
    void *textureHandle = g_TextureHandle;
    int width = g_TextureWidth;
    int height = g_TextureHeight;
    if (!textureHandle) {
        LOGE("SetVrMapView", "SetVrMapView - textureHandle is null");
        return;
    }
    uchar * textureDataPtr = cameraReader.getLastImage();
    LOGD("SetVrMapView", "updating Unity texture - %p with textureDataPtr - %p",
            textureHandle, textureDataPtr);
    s_CurrentAPI->EndModifyTexture(textureHandle, width, height, textureDataPtr);
}

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType) {
    LOGD("45rt66t", "OnGraphicsDeviceEvent");
    // Create graphics API implementation upon initialization
    if (eventType == kUnityGfxDeviceEventInitialize) {
        assert(s_CurrentAPI == nullptr);
        s_DeviceType = s_Graphics->GetRenderer();
        s_CurrentAPI = CreateRenderAPI(s_DeviceType);
    }

    // Let the implementation process the device related events
    if (s_CurrentAPI) {
        s_CurrentAPI->ProcessDeviceEvent(eventType, s_UnityInterfaces);
    }

    // Cleanup graphics API implementation upon shutdown
    if (eventType == kUnityGfxDeviceEventShutdown) {
        delete s_CurrentAPI;
        s_CurrentAPI = nullptr;
        s_DeviceType = kUnityGfxRendererNull;
    }
}

static void ModifyTexturePixels() {
    LOGD("45rt66t", "ModifyTexturePixels");
    void *textureHandle = g_TextureHandle;
    int width = g_TextureWidth;
    int height = g_TextureHeight;
    if (!textureHandle) {
        LOGE("ModifyTexturePixels", "ModifyTexturePixels - textureHandle is null");
        return;
    }
    std::thread processingThread(
            [&]() {
                cameraReader.readImageFrame(
                        (uchar*)textureHandle, 4 * height * width, 4, height, width);
            }
    );
    processingThread.join();
}

// --------------------------------------------------------------------------
// OnRenderEvent
// This will be called for GL.IssuePluginEvent script calls; eventID will
// be the integer passed to IssuePluginEvent. In this example, we just ignore
// that value.
static void UNITY_INTERFACE_API OnRenderEvent(int eventID) {
    // Unknown / unsupported graphics device type? Do nothing
    if (s_CurrentAPI == nullptr) {
        extern RenderAPI *CreateRenderAPI_OpenGLCoreES(UnityGfxRenderer apiType);
        s_CurrentAPI = CreateRenderAPI_OpenGLCoreES(kUnityGfxRendererOpenGLES20);
    }
    ModifyTexturePixels();
}


// --------------------------------------------------------------------------
// GetRenderEventFunc, an example function we export which is used to get a rendering event callback function.
EXPORTED UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetRenderEventFunc() {
    return OnRenderEvent;
}
