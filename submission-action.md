## Project 6: Action!

Please fill this out for Action! only. The project handout can be found [here](https://cs1230.graphics/projects/realtime/2).

### Output Comparison

> [!IMPORTANT]
> Before generating expected outputs, make sure to:
>
> 1. Set your working directory to the project directory
> 2. From the project directory, run `git submodule update --recursive --remote` to update the `scenefiles` submodule.
> 3. Change all instances of `"lights-camera"` in `mainwindow.cpp` to `"action"` (there should be 2 instances, one in `MainWindow::onUploadFile` and one in `MainWindow::onSaveImage`).

Run the program, open the specified `.json` file and follow the instructions to set the parameters.

If the output you are generating is an image, save the image with the specified file name using the "Save image" button in the UI. Be sure to follow the instructions in the left column to set the file name. Once you save the images, they will appear in the table below.

If the output you are generating is a video, reference the [Screen Recording Guide](https://cs1230.graphics/docs/screen-recording) to capture a video of the specified interaction. Once you've recorded everything, navigate to this template file in Github, click edit, and either select or drag-and-drop each of your videos into the correct location. This will upload them to GitHub (but not include them in the repo) and automatically embed them into this Markdown file by providing a link. Make sure to double-check that they all show up properly in the preview.

We're **not** looking for your video to **exactly** match the expected output (i.e. draw the exact same shape). Just make sure to follow the instructions and verify that the recording fully demonstrates the functionality of that part of your project.

> [!NOTE]
> There may be issues with the image saving functionality, and the images may not save the exact same way as the expected outputs. This is acceptable, as long as your output demonstrates correct functionality.

#### Point and Spot Lights

|                                File/Method To Produce Output                                 |                                                Expected Output                                                 |                                                       Your Output                                                       |
| :------------------------------------------------------------------------------------------: | :------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------: |
| Input: `point_light_1.json`<br/>Output: `point_light_1.png`<br/>Parameters: (5, 5, 0.1, 100) | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/action/required_outputs/point_light_1.png) | ![Place point_light_1.png in student_outputs/action/required folder](student_outputs/action/required/point_light_1.png) |
| Input: `point_light_2.json`<br/>Output: `point_light_2.png`<br/>Parameters: (5, 5, 0.1, 100) | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/action/required_outputs/point_light_2.png) | ![Place point_light_2.png in student_outputs/action/required folder](student_outputs/action/required/point_light_2.png) |
| Input: `spot_light_1.json`<br/>Output: `spot_light_1.png`<br/> Parameters: (5, 5, 0.1, 100)  | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/action/required_outputs/spot_light_1.png)  |  ![Place spot_light_1.png in student_outputs/action/required folder](student_outputs/action/required/spot_light_1.png)  |
|  Input: `spot_light_2.json`<br/>Output: `spot_light_2.png`<br/>Parameters: (5, 5, 0.1, 100)  | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/action/required_outputs/spot_light_2.png)  |  ![Place spot_light_2.png in student_outputs/action/required folder](student_outputs/action/required/spot_light_2.png)  |

#### Invert

> [!NOTE]
> If you did not implement the invert filter, you can skip this section.

|                                                      File/Method To Produce Output                                                      |                                                      Expected Output                                                      |                                                                  Your Output                                                                  |
| :-------------------------------------------------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------------------------------------: |
| Input: `primitive_salad_1.json`<br/>**Apply invert filter**<br/>Output: `primitive_salad_1_invert.png`<br/>Parameters: (5, 5, 0.1, 100) | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/action/required_outputs/primitive_salad_1_invert.png) | ![Place primitive_salad_1_invert.png in student_outputs/action/required folder](student_outputs/action/required/primitive_salad_1_invert.png) |

#### Grayscale

> [!NOTE]
> If you did not implement the grayscale filter, you can skip this section.

|                                                         File/Method To Produce Output                                                         |                                                       Expected Output                                                        |                                                                     Your Output                                                                     |
| :-------------------------------------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------------------------------------------: |
| Input: `primitive_salad_1.json`<br/>**Apply grayscale filter**<br/>Output: `primitive_salad_1_grayscale.png`<br/>Parameters: (5, 5, 0.1, 100) | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/action/required_outputs/primitive_salad_1_grayscale.png) | ![Place primitive_salad_1_grayscale.png in student_outputs/action/required folder](student_outputs/action/required/primitive_salad_1_grayscale.png) |

> [!WARNING]
> Kernel-based filters might not work the same on different devices. This is explained in more detail in the project handout. If your output doesn't seem to quite match our output but you think your implementation is correct, be sure to let your mentor know.

#### Sharpen

> [!NOTE]
> If you did not implement the sharpen filter, you can skip this section.

|                                                        File/Method To Produce Output                                                        |                                                       Expected Output                                                       |                                                                    Your Output                                                                    |
| :-----------------------------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------------------------------------------------: |
| Input: `recursive_sphere_4.json`<br/>**Apply sharpen filter**<br/>Output: `recursive_sphere_4_sharpen.png`<br/>Parameters: (5, 5, 0.1, 100) | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/action/required_outputs/recursive_sphere_4_sharpen.png) | ![Place recursive_sphere_4_sharpen.png in student_outputs/action/required folder](student_outputs/action/required/recursive_sphere_4_sharpen.png) |

#### Blur

> [!NOTE]
> If you did not implement the blur filter, you can skip this section.

|                                                     File/Method To Produce Output                                                     |                                                     Expected Output                                                      |                                                                 Your Output                                                                 |
| :-----------------------------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------------------------------------------: |
| Input: `recursive_sphere_4.json`<br/>**Apply blur filter**<br/>Output: `recursive_sphere_4_blur.png`<br/>Parameters: (5, 5, 0.1, 100) | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/action/required_outputs/recursive_sphere_4_blur.png) | ![Place recursive_sphere_4_blur.png in student_outputs/action/required folder](student_outputs/action/required/recursive_sphere_4_blur.png) |

#### Camera Translation

_Instructions:_ Load `chess.json`. For about 1 second each in this order, press:

- <kbd>W</kbd>, <kbd>A</kbd>, <kbd>S</kbd>, <kbd>D</kbd> to move in each direction by itself
- <kbd>W+A</kbd> to move diagonally forward and to the left
- <kbd>S+D</kbd> to move diagonally backward and to the right
- <kbd>Space</kbd> to move up
- <kbd>Cmd/Ctrl</kbd> to move down

##### Expected Output

https://github.com/BrownCSCI1230/projects_realtime_template/assets/45575415/710ff8b4-6db4-445b-811d-f6c838741e67

##### Your Output


https://github.com/user-attachments/assets/f25bd00f-73c7-44ed-90cd-376bbff3af02


<!---
Paste your output on top of this comment!
-->

#### Camera Rotation

_Instructions:_ Load `chess.json`. Take a look around!

##### Expected Output

https://github.com/BrownCSCI1230/projects_realtime_template/assets/45575415/a14f4d32-88ee-4f5f-9843-74dd5c89b9dd

##### Your Output


https://github.com/user-attachments/assets/1d7bdc9a-9c0f-4a8c-84fe-74cb9569142d


<!---
Paste your output on top of this comment!
-->

### Design Choices

Two files namely  `helper.cpp` and `paint.h` handle almost all the neccesary functions for the project execution. The  `shapes` folder has all files neccessary for generation of vertex data, `helper.cpp` handles VBOs and VAOs and  `paint.h` handles drawing these objects, transferring them to FBOs and further post-processing.

#### Camera Interactability

The design of the `calcMatrices` function in `helper.cpp` emphasizes a highly interactive camera system, allowing both translation and rotation to provide a smooth and intuitive user experience in navigating 3D scenes. There are three `caclMatrices`, the first one for the vanilla rasterization while the other two for translation and rotation.

1. **Translation**:
   - Users can move the camera in all six degrees of freedom:
     - **Forward/Backward**: Controlled via `W` and `S` keys, moving the camera along its forward (`lookVec`) or backward direction.
     - **Left/Right**: Controlled via `A` and `D` keys, moving the camera along its right (`rightVec`) or left direction.
     - **Up/Down**: Controlled via `Space` (up) and `Control` (down), moving the camera along the world up or down direction.
   - Translation is implemented with a constant movement speed and scales with `deltaTime` for frame-independent motion.

2. **Rotation**:
   - **Yaw Rotation**:
     - Performed around the camera's **Y-axis** using horizontal mouse movements (`deltaX`).
     - Allows the camera to turn left or right, adjusting its `look` and `up` vectors using a yaw rotation matrix (`rotationY`).
   - **Pitch Rotation**:
     - Performed around the **Right Vector** using vertical mouse movements (`deltaY`).
     - Allows the camera to tilt up or down, modifying its `look` and `up` vectors using a pitch rotation matrix (`rotationX`).
   - Rotations are applied sequentially (yaw followed by pitch) and normalized to maintain numerical stability.

3. **Matrix Computation**:
   - The `calcMatrices` function computes a new **view matrix** (`m_view`) after each update to reflect the camera's updated position and orientation.
   - The **projection matrix** (`m_proj`) ensures consistent perspective rendering based on the camera's field of view, aspect ratio, and near/far planes.

4. **Smooth Control**:
   - All motion and rotations are scaled by delta values (`deltaTime`, `deltaX`, `deltaY`) to provide smooth and proportional interactions, minimizing abrupt or unnatural transitions.



#### Additional Lights 
The `paint.h` uses several vectors to send in neccessary colors, positions and details of the lights. These vectors are then used in   `default.frag` file to compute corresponding fragment color by matching each light detail with each light type. The details are passed using vectors like these : 

```cpp
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightDirections;
    std::vector<glm::vec3> lightFunctions;
    std::vector<glm::vec3> lightColors;
    std::vector<float> lightPenumbra;
    std::vector<float> lightAngle;
    std::vector<int> directional_or_not;
```

and accessed like this :
```c

uniform vec3 lightPositions[100];  // Array for light positions
uniform vec3 lightDirections[100];  // Array for light positions
uniform vec3 lightColors[100];     // Array for light colors
uniform vec3 lightFunctions[100];     // Array for light functions
uniform float lightAngle[100]; // array for light angles
uniform float lightPenumbra[100]; //array for light penumbra
uniform int dirOrNot[100];         // Array indicating if each light is directional (1), point (-1) , spot (0)
uniform int numLights;             // Number of actual lights passed

```

#### FBOs

The fullscreen quad data and the deault FBO value is defined in the `initializeGL()` function and then a  `makeFBO()` function is used to initialise the FBOs and FBO texture. Inside the `paint` function in `paintGL`, FBO texture is filled, and then post-processing takes places from the paintTexture function in `paint.h`. The `grayscale.frag` performs blurring as well converting the image to grayscale. 
 
### Collaboration/References
N.A.
### Known Bugs
N.A.
### Extra Credit
N.A.