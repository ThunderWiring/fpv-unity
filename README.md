
The app starts with a main menu scene which is basically one button that turns on the device camera (asks for permission if neccessary).

Each frame captured by the camera is fed to a C++ plugin that uses OpenCV to process the image.
After capturing is finished, the user clicks a button on the camera scene to view the processed image in another scene.

In this repo, the code for the plugin is under `AndroidPluginSrc` 
Currently the plugin doesn't do any processing but returns the last image captured.

**Currently i'm faced with an issue that the plugin is not able to update the texture to view the image on.**
