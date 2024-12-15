Custom C++ Engine created as the first assignment for the UPC master course "Advanced programming for AAA video games"

This engine serves as the first step to create an engine for the final master project. 
Its functionalities include:
- Loading gltf files and textures (png and .dds)
- Moving around the viewport
- Zoom, orbit and focus of loaded meshes
- LOG display
- FPS graph

Controls
- W/A/S/D + left mouse click to move around
- LAlt + left mouse click to orbit the loaded mesh
- Mouse wheel for zooming
- F to focus the loaded mesh
- Shift increases the movement speed
- Drag and Drop for gltf files and textures to load them into the engine
- ImGui menus to see different statistics and to change parameters such as fov and near / far clipping planes


Github project link
https://github.com/LukasGanter/UPCCustomEngine

Known issues:
- DirectXTex must be build manually before running the engine. The solution file can be found under Source/DirectXTex-main/DirectXTex_Desktop_2022. For the debug engine build it must we build with the Debug profile, for release with the Release profile
- The working directory must be set manually in the VS options under Configuration/Debugging. The needed value is $(ProjectDir)/../Game
- tinyGltf contains a memory leak
