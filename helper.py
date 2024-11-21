import json

def create_ini_file(json_file, output_file="output.png"):
    """
    Creates a default INI file pointing to the specified JSON file and output image.
    """
    ini_file = "default.ini"

    with open(ini_file, "w") as file:
        file.write(f"""[IO]
scene = {json_file}
output = {output_file}

[Canvas]
width = 1024
height = 768

[Feature]
shadows = true
reflect = true
refract = false
texture = true
parallel = false
super-sample = false
num-samples = 1
post-process = false
acceleration = false
depthoffield = false

[Settings]
maximum-recursive-depth = 4
""")
    #print(f"INI file '{ini_file}' created with scene '{json_file}' and output '{output_file}'.")
    return ini_file



def insert_lights_into_json(json_file, light_positions, modified_json_file):
    """
    Reads the JSON file, removes existing lights, inserts new lights at the specified positions,
    and saves to modified_json_file.
    """
    with open(json_file, 'r') as f:
        data = json.load(f)
    
    # Function to recursively remove lights from the scene
    def remove_lights(group):
        if 'lights' in group:
            del group['lights']
        if 'groups' in group:
            for subgroup in group['groups']:
                remove_lights(subgroup)
    
    # Remove existing lights from the scene
    remove_lights(data)
    
    # Ensure 'groups' exists in the JSON data
    if 'groups' not in data:
        data['groups'] = []
    
    # Create light groups for each light position
    for pos in light_positions:
        light_group = {
            "translate": pos,
            "lights": [
                {
                    "type": "point",
                    "color": [1.0, 1.0, 1.0],
                    "attenuationCoeff": [0.8, 0.2, 0.0]
                }
            ]
        }
        data['groups'].append(light_group)
    
    # Save the modified JSON data
    with open(modified_json_file, 'w') as f:
        json.dump(data, f, indent=2)
    
    #print(f"Modified JSON file '{modified_json_file}' created with new lights.")
    return modified_json_file



def insert_rotation_into_json(json_file, rotations, modified_json_file):
    """
    Reads the JSON file, inserts rotation for each object, and saves to modified_json_file.
    
    Args:
        json_file (str): Path to the input JSON file.
        rotations (list of list): List of rotation values (e.g., [[30, 0, 0], [0, 45, 0], [0, 0, 90]]).
        modified_json_file (str): Path to save the modified JSON file.

    Returns:
        dict: The modified JSON object.
    """
    with open(json_file, 'r') as f:
        data = json.load(f)
    
    rotation_index = 0  # To cycle through the provided rotations

    def apply_rotation(group):
        nonlocal rotation_index
        # Apply rotation to primitives
        if 'primitives' in group:
            for primitive in group['primitives']:
                if 'rotate' not in primitive:
                    primitive['rotate'] = rotations[rotation_index % len(rotations)]
                    rotation_index += 1
        
        # Apply rotation to subgroups recursively
        if 'groups' in group:
            for subgroup in group['groups']:
                apply_rotation(subgroup)

    # Apply rotation to the root-level groups
    # for group in data.get('groups', []):
    #     apply_rotation(group)

    # Save the modified JSON data
    with open(modified_json_file, 'w') as f:
        json.dump(data, f, indent=2)
    
    print(f"Modified JSON file with rotations saved to '{modified_json_file}'.")
    
    return modified_json_file  # Return the modified JSON object



def insert_surface_translation(translation_values,modified_json_file):
    # Initial JSON
    data = {
        "name": "root",
        "globalData": {
            "ambientCoeff": 0.5,
            "diffuseCoeff": 0.5,
            "specularCoeff": 0.5,
            "transparentCoeff": 0
        },
        "cameraData": {
            "position": [-5.0, 1.0, 0.0],
            "up": [0.0, 1.0, 0.0],
            "focus": [0, 0, 0],
            "heightAngle": 50.0
        },
        "groups": [
            {
                "lights": [
                    {"type": "directional", "color": [1.0, 1.0, 1.0], "direction": [0.0, -1.0, 0.0]}
                ]
            },
            {
                "lights": [
                    {"type": "directional", "color": [1.0, 1.0, 1.0], "direction": [1.0, 0.0, 0.0]}
                ]
            },
            {
                "lights": [
                    {"type": "directional", "color": [1.0, 1.0, 1.0], "direction": [0.0, 0.0, 1.0]}
                ]
            },
            {
                "groups": [
                    {
                        "translate": [0.0, -0.6, 0.0],
                        "scale": [6.0, 0.1, 6.0],
                        "primitives": [
                            {"type": "cube", "diffuse": [0.1, 0.4, 0.8]}
                        ]
                    },
                    {
                        "translate": [0.0, 0.0, 5.0],
                        "scale": [6.0, 6.0, 6.0],
                        "primitives": [
                            {"type": "cube", "diffuse": [1.0, 0.0, 0.8]}
                        ]
                    },
                    {
                        "translate": [5.0, 0.0, 0.0],
                        "scale": [6.0, 6.0, 6.0],
                        "primitives": [
                            {"type": "cube", "diffuse": [0.0, 1.0, 0.8]}
                        ]
                    }
                ]
            }
        ]
    }

    # Prompt for a single list of 12 values
    # print("Enter 12 translation values (x, y, z for each primitive) separated by commas:")
    # translation_values = list(map(float, input().split(",")))

    # Ensure 12 values are provided
    if len(translation_values) != 12:
        raise ValueError("You must enter exactly 12 values.")

    # Split the 12 values into groups of 3
    translations = {
        "cube": translation_values[0:3],
        "cone": translation_values[3:6],
        "sphere": translation_values[6:9],
        "cylinder": translation_values[9:12]
    }

    # Predefined objects with maintained properties
    new_objects = [
        {
            "translate": translations["cylinder"],
            "primitives": [
                {
                    "type": "cylinder",
                    "diffuse": [0.8, 0.6, 0.6],
                    "specular": [1.0, 1.0, 1.0],
                    "shininess": 15.0
                }
            ]
        },
        {
            "translate": translations["cone"],
            "primitives": [
                {
                    "type": "cone",
                    "diffuse": [0.6, 0.6, 0.8],
                    "specular": [1.0, 1.0, 1.0],
                    "shininess": 15.0
                }
            ]
        },
        {
            "translate": translations["sphere"],
            "primitives": [
                {
                    "type": "sphere",
                    "diffuse": [0.6, 0.8, 0.6],
                    "specular": [1.0, 1.0, 1.0],
                    "shininess": 15.0
                }
            ]
        },
        {
            "translate": translations["cube"],
            "primitives": [
                {
                    "type": "cube",
                    "diffuse": [0.7, 0.7, 0.7],
                    "specular": [1.0, 1.0, 1.0],
                    "shininess": 15.0
                }
            ]
        }
    ]

    # Insert new objects into the JSON
    data["groups"][-1]["groups"].extend(new_objects)

    # Print updated JSON

    with open(modified_json_file, 'w') as f:
        json.dump(data, f, indent=2)
    
    #print(f"Modified JSON file with rotations saved to '{modified_json_file}'.")
    
    return modified_json_file  # Return the modified JSON object