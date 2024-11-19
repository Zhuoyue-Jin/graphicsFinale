import os
import subprocess
import json

def insert_lights_into_json(json_file, light_positions, modified_json_file):
    """
    Reads the JSON file, inserts lights at the specified positions,
    and saves to modified_json_file.
    """
    with open(json_file, 'r') as f:
        data = json.load(f)
    
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

def run_projects_ray(json_file, output_file):
    """
    Prompts the user for light positions, modifies the JSON file, creates an INI file,
    and runs the `projects_ray` executable.
    """
    # Prompt the user for light positions
    print("Enter light positions as a list of coordinates (e.g., [1, 2, 3] or multiple lights [[1, 2, 3], [4, 5, 6]]):")
    light_positions_input = input("Light positions: ")

    try:
        # Convert the input to a valid list
        light_positions = eval(light_positions_input)  # Be cautious with eval
        if isinstance(light_positions[0], (int, float)):
            # Single light position
            light_positions = [light_positions]
        elif all(isinstance(pos, list) for pos in light_positions):
            # Multiple light positions
            pass
        else:
            raise ValueError
    except Exception as e:
        print(f"Error parsing light positions: {e}")
        return

    # Get the current directory
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # Path for the executable
    executable = os.path.join(script_dir, "projects_ray")

    # Create a modified JSON file with the lights
    modified_json_file = "modified_scene.json"
    insert_lights_into_json(json_file, light_positions, modified_json_file)

    # Create the INI file pointing to the modified JSON file
    ini_file = create_ini_file(modified_json_file, output_file)

    # Run the executable with the INI file
    try:
        subprocess.run([executable, ini_file], check=True)
        print(f"Successfully ran '{executable}' with '{ini_file}'.")
    except subprocess.CalledProcessError as e:
        print(f"Error occurred while running '{executable}': {e}")
        raise
