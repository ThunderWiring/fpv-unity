using System.Collections;
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityEngine.Android;

[RequireComponent(typeof(NativeAdapter))]
public class PhoneCamera : MonoBehaviour
{
    private bool isCamAvailable;
    private bool stopRecording = false;
    private WebCamTexture backCam;
    private Texture defaultBackground;
    private NativeAdapter nativeAdapter;

    public RawImage background;
    public AspectRatioFitter fit;

    private void Awake()
    {
        nativeAdapter = GetComponent<NativeAdapter>();
#if PLATFORM_ANDROID
        if (!Permission.HasUserAuthorizedPermission(Permission.Camera))
        {
            Permission.RequestUserPermission(Permission.Camera);
        }
#endif
    }

    private void Start()
    {
#if UNITY_WEBGL && !UNITY_EDITOR
		RegisterPlugin();
#else
        Debug.Log("cannot register plugin");
#endif

        defaultBackground = background.texture;
        WebCamDevice[] devices = WebCamTexture.devices;

        if (devices.Length == 0) {
            Debug.Log("No Camera is available");
            isCamAvailable = false;
            return;
        }

        for (int i = 0; i < devices.Length; i++) {
            if (!devices[i].isFrontFacing) {
                backCam = new WebCamTexture(devices[i].name, Screen.width, Screen.height);
                break;
            }
        }

        if (backCam == null) {
            Debug.Log("cannot find back cam");
            return;
        }
        isCamAvailable = true;
        backCam.Play();
        background.texture = backCam;
    }

    public void onRecordingCompletes()
    {
        stopRecording = true;
        Debug.Log("onRecordingCompletes");
        
    }

    private void Update()
    {
        if (stopRecording) {
            backCam.Stop();
            // todo: call native function to prepare the panorama views
            SceneManager.LoadScene(2); // the vr tour-view scene
            return;
        }
        // handle back press
        if (Input.GetKey(KeyCode.Escape))
        {
            backCam.Stop();
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex - 1);
            return;
        } else if (!isCamAvailable) {
            return;
        }
        float ratio = (float)backCam.width / (float)backCam.height;
        fit.aspectRatio =  ratio;
        float scaleY = backCam.videoVerticallyMirrored ? -1f : 1f;
        background.rectTransform.localScale = new Vector3(1f, scaleY, 1f);

        int orient = -backCam.videoRotationAngle;
        background.rectTransform.localEulerAngles = new Vector3(0, 0, orient);

        nativeAdapter.PassViewTextureToPlugin(background.texture);
        nativeAdapter.StartOnRenderEvent();
    }
}
