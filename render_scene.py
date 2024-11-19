import os
import json
import subprocess

def create_ini_file(json_file, output_file="output.png"):
    """
    Creates an INI file with settings pointing to the specified JSON file and output image.
    """
    ini_content = f"""[IO]
scene = {json_file}
output = {output_file}

[Canvas]
width = 800
height = 600

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
"""

    ini_file = "default.ini"
    with open(ini_file, "w") as file:
        file.write(ini_content)
    print(f"INI file '{ini_file}' created.")
    return ini_file

def run_raytracer(ini_file):
    """
    Runs the raytracer program with the given INI file.
    """
    executable = "./projects_ray"  # Assuming the executable is in the same directory
    if not os.path.isfile(executable):
        print(f"Error: Raytracer executable '{executable}' not found.")
        return

    try:
        subprocess.run([executable, ini_file], check=True)
        print(f"Raytracer ran successfully with '{ini_file}'.")
    except subprocess.CalledProcessError as e:
        print(f"Error running raytracer: {e}")

def main():
    import argparse

    parser = argparse.ArgumentParser(description="Render scene using JSON file and raytracer.")
    parser.add_argument("json_file", help="Path to the input JSON file.")
    parser.add_argument("output_file", nargs='?', default="output.png", help="Name of the output PNG file (default: output.png).")
    args = parser.parse_args()

    json_file = args.json_file
    output_file = args.output_file

    # Check if JSON file exists
    if not os.path.isfile(json_file):
        print(f"Error: JSON file '{json_file}' not found.")
        return

    # Create INI file
    ini_file = create_ini_file(json_file, output_file)

    # Run raytracer
    run_raytracer(ini_file)

if __name__ == "__main__":
    main()