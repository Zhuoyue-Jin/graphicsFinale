import os
import subprocess
import json
from PIL import Image
import numpy as np
from scipy.optimize import minimize

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

    # Remove existing lights to avoid superimposing
    def remove_lights(group):
        group.pop('lights', None)
        if 'groups' in group:
            for subgroup in group['groups']:
                remove_lights(subgroup)

    remove_lights(data)

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
width = 512
height = 512

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
    return ini_file

def run_renderer(executable, ini_file):
    """
    Runs the renderer and waits for it to finish.
    """
    try:
        subprocess.run([executable, ini_file], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error occurred while running '{executable}': {e}")
        raise

def load_rendered_image(image_path):
    """
    Loads the rendered image and converts it to a numpy array.
    """
    rendered_image = Image.open(image_path).convert('RGB')
    rendered_array = np.array(rendered_image) / 255.0  # Normalize to [0, 1]
    return rendered_array

def compute_loss(rendered_image, ground_truth_image):
    """
    Computes the loss between the rendered image and the ground truth image.
    """
    loss = np.mean((rendered_image - ground_truth_image) ** 2)
    return loss

def optimize_light_positions(json_file, output_file, ground_truth_image_path):
    """
    Optimizes the light positions to minimize the difference between
    the rendered image and the ground truth image.
    """
    # Paths
    script_dir = os.path.dirname(os.path.abspath(__file__))
    executable = os.path.join(script_dir, "projects_ray")
    modified_json_file = "modified_scene.json"
    ini_file = create_ini_file(modified_json_file, output_file)

    # Load the ground truth image
    ground_truth_image = Image.open(ground_truth_image_path).convert('RGB')
    ground_truth_image = ground_truth_image.resize((512, 512))  # Ensure same size as rendered image
    ground_truth_array = np.array(ground_truth_image) / 255.0

    # Create a directory to save iteration images
    iteration_images_dir = "iteration_images"
    os.makedirs(iteration_images_dir, exist_ok=True)

    # Counter for iterations
    iteration_counter = [0]  # Use a mutable type to modify inside the nested function

    # Initial guess for light positions
    initial_light_positions = np.array([[0.0, 3.0, 0.0]])  # Adjust as needed
    initial_guess = initial_light_positions.flatten()

    # Bounds for light positions
    bounds = [(-10, 10), (-10, 10), (-10, 10)] * (len(initial_guess) // 3)

    # Objective function
    def objective_function(light_positions):
        # Update iteration counter
        iteration_counter[0] += 1
        iteration = iteration_counter[0]

        # Ensure light_positions is in correct shape
        light_positions = np.reshape(light_positions, (-1, 3)).tolist()

        # Update the JSON file with new light positions
        insert_lights_into_json(json_file, light_positions, modified_json_file)

        # Update the output file name to include iteration number
        current_output_file = os.path.join(iteration_images_dir, f"render_{iteration:03d}.png")
        create_ini_file(modified_json_file, current_output_file)

        # Run the renderer
        run_renderer(executable, ini_file)

        # Load the rendered image
        rendered_image = load_rendered_image(current_output_file)

        # Compute and return the loss
        loss = compute_loss(rendered_image, ground_truth_array)
        print(f"Iteration {iteration}, Loss: {loss}")

        return loss

    # Run the optimization
    result = minimize(
        objective_function,
        initial_guess,
        method='Nelder-Mead',
        bounds=bounds,
        options={'maxiter': 50, 'disp': True}  # Adjust the number of iterations as needed
    )

    # Get the optimized light positions
    optimized_light_positions = np.reshape(result.x, (-1, 3))
    print("Optimized Light Positions:", optimized_light_positions)

    # Render the final image with optimized light positions
    final_output_file = output_file  # Use the original output file name
    insert_lights_into_json(json_file, optimized_light_positions.tolist(), modified_json_file)
    create_ini_file(modified_json_file, final_output_file)
    run_renderer(executable, ini_file)

    print(f"Optimization completed. Final image saved to '{final_output_file}'.")

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Optimize light positions to match a ground truth image.")
    parser.add_argument("json_file", help="Path to the input JSON file.")
    parser.add_argument("output_file", help="Name of the output PNG file to create.")
    parser.add_argument("ground_truth_image", help="Path to the ground truth image.")
    args = parser.parse_args()

    # Ensure the JSON file exists
    if not os.path.isfile(args.json_file):
        print(f"Error: JSON file '{args.json_file}' does not exist.")
        exit(1)

    # Ensure the ground truth image exists
    if not os.path.isfile(args.ground_truth_image):
        print(f"Error: Ground truth image '{args.ground_truth_image}' does not exist.")
        exit(1)

    # Ensure the output file has a valid extension
    output_file = args.output_file
    if not output_file.endswith(".png"):
        output_file += ".png"

    # Run the optimization process
    optimize_light_positions(args.json_file, output_file, args.ground_truth_image)