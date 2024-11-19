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
    print(f"INI file '{ini_file}' created with scene '{json_file}' and output '{output_file}'.")
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
    
    print(f"Modified JSON file '{modified_json_file}' created with new lights.")
    return modified_json_file