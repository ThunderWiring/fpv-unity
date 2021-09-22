using System.Collections;
using System;
using UnityEngine;
using System.Runtime.InteropServices;

public class NativeAdapter : MonoBehaviour
{
    // We'll also pass native pointer to a texture in Unity.
    // The plugin will fill texture data from native code.
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("NativeAndroidPlugin")]
#endif
    private static extern void SetTextureFromUnity(IntPtr texture, int w, int h);

#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("NativeAndroidPlugin")]
#endif
    private static extern IntPtr GetRenderEventFunc();
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("NativeAndroidPlugin")]
#endif
    private static extern IntPtr SetVrMapView();

#if UNITY_WEBGL && !UNITY_EDITOR
	[DllImport ("__Internal")]
	private static extern void RegisterPlugin();
#endif

#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("NativeAndroidPlugin")]
    private static extern void RecieveImage(byte[] bytes, int width, int height);
#endif

    public void StartOnRenderEvent()
    {
        //StartCoroutine(CallPluginAtEndOfFrames());
        GL.IssuePluginEvent(GetRenderEventFunc(), 1);
        GC.Collect();
    }

    /** Calling this function from StartOnRenderEvent makes
     * the frames render extremly slowly*/
    IEnumerator CallPluginAtEndOfFrames()
    {
        yield return new WaitForSeconds(1f);
        while (true)
        {
            // Wait until all frame rendering is done
            yield return new WaitForEndOfFrame();
            GL.IssuePluginEvent(GetRenderEventFunc(), 1);
            GC.Collect();
        }
    }

    public void PassViewTextureToPlugin(Texture tex)
    {
        // Pass texture pointer to the plugin
        SetTextureFromUnity(tex.GetNativeTexturePtr(), tex.width, tex.height);
    }

    public void GetImageFromPlugin()
    {
        Debug.Log("SetVrMapView");
        SetVrMapView();
    }

}
