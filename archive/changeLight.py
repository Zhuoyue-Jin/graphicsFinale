import os
import subprocess
import ast
from helper import create_ini_file,insert_lights_into_json


def run_projects_ray(json_file, output_file):
    """
    Prompts the user for light positions, modifies the JSON file, creates an INI file,
    and runs the `projects_ray` executable.
    """
    # Prompt the user for light positions
    print("Enter light positions as a list of coordinates (e.g., [1, 2, 3] or multiple lights [[1, 2, 3], [4, 5, 6]]):")
    light_positions_input = input("Light positions: ")

    try:
        # Convert the input to a valid list using ast.literal_eval for safety
        light_positions = ast.literal_eval(light_positions_input)
        if isinstance(light_positions[0], (int, float)):
            # Single light position
            light_positions = [light_positions]
        elif all(isinstance(pos, list) for pos in light_positions):
            # Multiple light positions
            pass
        else:
            raise ValueError("Invalid format for light positions.")
    except Exception as e:
        print(f"Error parsing light positions: {e}")
        return

    # Get the current directory
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # Path for the executable
    executable = os.path.join(script_dir, "projects_ray")

    # Check if the executable exists
    if not os.path.isfile(executable):
        print(f"Error: Executable '{executable}' not found.")
        return

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

if __name__ == "__main__":
    import argparse

    # Argument parser for JSON input file and custom output PNG file name
    parser = argparse.ArgumentParser(description="Run projects_ray on a JSON file with user-specified light positions.")
    parser.add_argument("json_file", help="Path to the input JSON file.")
    parser.add_argument("output_file", help="Name of the output PNG file to create.")
    args = parser.parse_args()

    # Ensure the JSON file exists
    if not os.path.isfile(args.json_file):
        print(f"Error: JSON file '{args.json_file}' does not exist.")
        exit(1)

    # Ensure the output file has a valid extension
    output_file = args.output_file
    if not output_file.endswith(".png"):
        output_file += ".png"

    # Run the process
    run_projects_ray(args.json_file, output_file)