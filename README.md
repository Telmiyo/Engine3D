# Amapola Engine
3D engine project created for "Videogame's Engine" subject at the Bachelor's degree in Video Game Design and Development in CITM.

## Members
 - David Montufo: ([Montuuh](https://github.com/Montuuh))
 - Telmo Beroiz: ([Telmiyo](https://github.com/Telmiyo))
 - Oscar Cuatrecasas: ([OCA99](https://github.com/OCA99))

## New features

- Drag & Drop resources to import them as a custom format in the resource manager folder (called Meshes)
- Object Selection my mouse.
- A camera component that uses frustum culling.
- A camera ca be created in game object imgui window
- First screen is an image with a play button. Afterwards, the scene is loaded
- Scenes can be serialized to a file that can be loaded.
- New User Interface resources (buttons, checkboxes, input text, images...)
- Press F1 for opening an options window with a vsync checkbox

## User Interface components
- A canvas parent must be created in order UI elements can be drawn
### Transform 2D
 - Every component has the transform 2d component
 - At the inspector, the user can modify the position, the rotation, the pivot, the size and the desired anchor
### Image
 - In order to have a ui component rendered, it must have a component image
 - You can drag & drop the desired texture to the inspector, or select it by the texture resources folder
 - A display at the inspector is shown
 - You can change the texture color palette at the inspector
### Button
 - Basic button function with basic states. Disabled if a button is not active, Idle, Focused and Pressed.
### Checkbox
 - Basic checkbox function. Check bool.
 - You can select different texture for both checked states. It is done in the texture resources folder.
### Text
 - We could not manage to implement text rendering!
### Options window
 - You are able to create an options window which can contain any UI element desired
 - In example, we delivered by default an options window with a vsync checkbox (Press F1)
## Camera controls
 - Right Click: Rotate the camera freely
 - WASD: Move the camera forward, left, back and right respectively
 - Shift: Duplicates the camera speed
 - Q: Move the camera upwards
 - E: Move the camera downwards
 - Mouse wheel: Zoom camera
 - F: Point the camera to the selected game object
 - ALT + Right click: Rotates the camera around the selected game object
## Windows
 - File: Manage engine's general options
    - Scene:
      - CTRL + S: Save current scene
      - CTRL + L: Load current scene 
    - ALT + F4: Exit engine 
 - GameObject:
    - Create empty Game Object
    - Create camera
    - Create 3D Primitives (Cube, Sphere, Cylinder)
    - Create UI resources (Canvas, Image, Button, Checkbox, Text, Option Window)
 - Window:
    - Examples: ImGui Tutorial
    - Workspace Style: Change Engine theme
    - Hierarchy: 
      - 4 buttons: "New Empty", "New Children", "Delete Selected", "Delete All"
      - Gameobjects hierarchy
    - Inspector: Show game object selected info
      -   Transform: Position, Rotation & Scale
      -   Material: Texture name, texture image, texture image size
      -   Mesh: Information about the mesh
    - Scene
    - Game
    - Console: Shows engine's console output
    - Textures: You can Drag & Drop Textures and assign them to the selected game object
    - Configuration: Engine's config (window, render, camera...)
 
## Download
  1. Open the following link. [Download](https://github.com/Telmiyo/Engine3D/releases)
  2. Go to the latest version uploaded, in this case v0.2, and download the .zip file
  3. Unzip the project and execute the .exe
