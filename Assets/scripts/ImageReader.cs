using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using System;

#if PLATFORM_ANDROID
using UnityEngine.Android;
#endif

[RequireComponent(typeof(NativeAdapter))]
public class ImageReader : MonoBehaviour
{

    private Texture writableTexture;
    private NativeAdapter nativeAdapter;
    [SerializeField] private Material screenImageFromPlugin;
    [SerializeField] private Camera camera;

    private void Awake()
    {
        nativeAdapter = GetComponent<NativeAdapter>();
    }

    void Start()
    {
        CreateWritableTexture(
            1080,
            2340);
        SetPanoramaBackgroundImage(false, false);
    }

    void Update()
    {
        // handle back press
        if (Input.GetKey(KeyCode.Escape))
        {
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex - 1);
            return;
        }
        
        if (Input.touchCount == 1) {
            Touch screenTouch = Input.GetTouch(0);
            if (screenTouch.phase == TouchPhase.Moved) {
                float dx = (float)screenTouch.deltaPosition.x / 5;
                float dy = (float)screenTouch.deltaPosition.y / 5;
                camera.transform.Rotate(dy, dx, 0f);
            }
            if (screenTouch.phase == TouchPhase.Ended) { 
            
            }
        }
    }

    private void CreateWritableTexture(int width, int height)
    {
        writableTexture = new Texture2D(width, height, TextureFormat.RGBA32, false);

        nativeAdapter.PassViewTextureToPlugin(writableTexture);
        screenImageFromPlugin.EnableKeyword("_NORMALMAP");
        screenImageFromPlugin.SetTexture("_BumpMap", writableTexture);
    }

    private void SetPanoramaBackgroundImage(bool mirror, bool rotate)
    {
        nativeAdapter.GetImageFromPlugin();
    }


}
