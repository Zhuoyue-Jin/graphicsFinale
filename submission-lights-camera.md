## Project 5: Lights, Camera

Please fill this out for Lights, Camera only. The project handout can be found [here](https://cs1230.graphics/projects/realtime/1).

### Output Comparison

Run the program, open the specified `.json` file, follow the instructions to set the parameters, and save the image with the specified file name using the "Save image" button in the UI. It should automatically suggest the correct directory - again, be sure to follow the instructions in the left column to set the file name. Once you save the images, they will appear in the table below.

> If your program can't find certain files or you aren't seeing your output images appear, make sure to:<br/>
>
> 1. Set your working directory to the project directory
> 2. Clone the `scenefiles` submodule. If you forgot to do this when initially cloning this repository, run `git submodule update --init --recursive` in the project directory

> Note: once all images are filled in, the images will be the same size in the expected and student outputs.

|                                           File/Method To Produce Output                                            |                                                     Expected Output                                                     |                                                                     Your Output                                                                     |
| :----------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------------------------------------------: |
|                Input: `unit_cone.json`<br/>Output: `unit_cone.png`<br/>Parameters: (5, 5, 0.1, 100)                |      ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cone.png)      |            ![Place unit_cone.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cone.png)            |
|            Input: `unit_cone_cap.json`<br/>Output: `unit_cone_cap.png`<br/>Parameters: (5, 5, 0.1, 100)            |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cone_cap.png)    |        ![Place unit_cone_cap.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cone_cap.png)        |
|               Input: `unit_cube.json`<br/>Output: `unit_cube.png`<br/> Parameters: (5, 5, 0.1, 100)                |      ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cube.png)      |            ![Place unit_cube.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cube.png)            |
|            Input: `unit_cylinder.json`<br/>Output: `unit_cylinder.png`<br/>Parameters: (5, 5, 0.1, 100)            |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cylinder.png)    |        ![Place unit_cylinder.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cylinder.png)        |
|              Input: `unit_sphere.json`<br/>Output: `unit_sphere.png`<br/>Parameters: (5, 5, 0.1, 100)              |     ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_sphere.png)     |          ![Place unit_sphere.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_sphere.png)          |
|          Input: `unit_cone.json`<br/>Output: `unit_cone_min.png`<br/>Parameters: (**1**, **3**, 0.1, 100)          |      ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cone_min.png)      |        ![Place unit_cone_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cone_min.png)        |
|      Input: `unit_cone_cap.json`<br/>Output: `unit_cone_cap_min.png`<br/>Parameters: (**1**, **3**, 0.1, 100)      |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cone_cap_min.png)    |    ![Place unit_cone_cap_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cone_cap_min.png)    |
|          Input: `unit_cube.json`<br/>Output: `unit_cube_min.png`<br/>Parameters: (**1**, **1**, 0.1, 100)          |      ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cube_min.png)      |        ![Place unit_cube_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cube_min.png)        |
|      Input: `unit_cylinder.json`<br/>Output: `unit_cylinder_min.png`<br/>Parameters: (**1**, **3**, 0.1, 100)      |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cylinder_min.png)    |    ![Place unit_cylinder_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cylinder_min.png)    |
|        Input: `unit_sphere.json`<br/>Output: `unit_sphere_min.png`<br/>Parameters: (**2**, **3**, 0.1, 100)        |     ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_sphere_min.png)     |      ![Place unit_sphere_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_sphere_min.png)      |
|           Input: `parse_matrix.json`<br/>Output: `parse_matrix.png`<br/>Parameters: (**3**, 5, 0.1, 100)           |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/parse_matrix.png)     |         ![Place parse_matrix.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/parse_matrix.png)         |
|            Input: `ambient_total.json`<br/>Output: `ambient_total.png`<br/>Parameters: (5, 5, 0.1, 100)            |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/ambient_total.png)    |        ![Place ambient_total.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/ambient_total.png)        |
|            Input: `diffuse_total.json`<br/>Output: `diffuse_total.png`<br/>Parameters: (5, 5, 0.1, 100)            |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/diffuse_total.png)    |        ![Place diffuse_total.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/diffuse_total.png)        |
|           Input: `specular_total.json`<br/>Output: `specular_total.png`<br/>Parameters: (5, 5, 0.1, 100)           |   ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/specular_total.png)    |       ![Place specular_total.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/specular_total.png)       |
|              Input: `phong_total.json`<br/>Output: `phong_total.png`<br/>Parameters: (5, 5, 0.1, 100)              |     ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/phong_total.png)     |          ![Place phong_total.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/phong_total.png)          |
|      Input: `directional_light_1.json`<br/>Output: `directional_light_1.png`<br/>Parameters: (5, 5, 0.1, 100)      | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/directional_light_1.png) |  ![Place directional_light_1.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/directional_light_1.png)  |
| Input: `directional_light_2.json`<br/>Output: `directional_light_2.png`<br/>Parameters: (**10**, **10**, 0.1, 100) | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/directional_light_2.png) |  ![Place directional_light_2.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/directional_light_2.png)  |
|      Input: `phong_total.json`<br/>Output: `phong_total_near_far.png`<br/>Parameters: (5, 5, **9.5**, **12**)      | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/phong_total_near_far.png) | ![Place phong_total_near_far.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/phong_total_near_far.png) |
|      Input: `directional_light_1.json`<br/>Output: `directional_light_1_near_far.png`<br/>Parameters: (**25**, **25**, **8**, **10**)      | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/directional_light_1_near_far.png) | ![Place directional_light_1_near_far.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/directional_light_1_near_far.png) |

### Design Choices
In the SceneChanged or SettingsChanged functions of realtime.cpp, when a scene or settings file is updated, the program utilizes the SceneParser to load data from the JSON file into the RenderData object (data). Once this data is loaded, the FigureVertices function, defined in helper.cpp, is called to process the data and determine the types and quantities of objects described in the JSON.

The FigureVertices function identifies object types (e.g., cubes, spheres, cylinders, cones) and flags their presence in the vertices object. Based on this information, the number of objects and their types are used to adjust tessellation parameters (BaseParameter1 and BaseParameter2) as needed.

Subsequently, the ScanParameters function is called to generate vertex data for each object type, compute the vertex counts, and assign the vertex data to corresponding Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs). These buffers are then used to render the objects correctly during the paintGL function, where OpenGL shader programs and transformation matrices are applied to produce the desired visual output. The paintGL function uses a paint function stored in paint.h 

To save time and memory and minimize redundancy, I used a single VAO and VBO for each object type rather than creating separate ones for each object instance, although this is not the case when extra credit 2 is used ( more information below)



### Collaboration/References
N.A.
### Known Bugs
The lighting on the cone appears somewhat inverted when the tessellation level is very low. However, as the tessellation values increase beyond a certain threshold, this issue resolves, and the lighting transitions smoothly.




### Extra Credit

#### Extra Credit: Dynamic Tessellation Scaling 
##### Extra Credit 1 Button
To address performance concerns and reduce scene complexity when the number of objects becomes high, I implemented the `extraCredits1` function. This function dynamically adjusts tessellation parameters (`baseParam1` and `baseParam2`) based on the total number of objects (`param_count`). This optimization satisfies the extra credit prompt:  
*"Scenes with more objects have fewer triangles, to keep the scene from getting too complex."*

| **Extra Credit Status** | **Vertex Count** | **Image Preview**                                                                 |
|--------------------------|------------------|-----------------------------------------------------------------------------------|
| **Off**                 | 270,900         | <img src="student_outputs/lights-camera/required/primitive_salad_2.png" alt="Without Extra Credit" width="300"> |
| **On**                  | 97,740          | <img src="student_outputs/lights-camera/required/primitive_salad_2_extracredit.png" alt="With Extra Credit" width="300"> |

#### Extra Credit: Dynamic Tessellation Scaling for Depth
##### Extra Credit 2 Button

To address performance concerns and reduce scene complexity as depth of objects increase, I implemented the `adjustPrimitiveParameters` function. This function dynamically adjusts tessellation parameters (`baseParam1` and `baseParam2`) based on the distance of objects from the camera. This optimization satisfies the extra credit prompt:  
*"Objects farther from the camera should have fewer triangles than objects closer to the camera."*

| **Extra Credit Status** | **Image Preview**                                                                 |
|-------------------------|-----------------------------------------------------------------------------------|
| **Off**                 | <img src="student_outputs/lights-camera/required/recursive_sphere_6.png" alt="Without Extra Credit" width="300"> |
| **On**                  | <img src="student_outputs/lights-camera/required/recursive_sphere_6_extracredit.png" alt="With Extra Credit" width="300"> |



##### Extra Credit: Create your own scene

<img src="student_outputs/lights-camera/required/self.png" width="300">
<a href="crazy.json">Link to JSON</a>
