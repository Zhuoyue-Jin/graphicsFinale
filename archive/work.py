import os
import subprocess
from PIL import Image
import numpy as np
from scipy.optimize import minimize
from helper import (
    insert_lights_into_json,
    create_ini_file,
    insert_surface_translation,
)
from skimage.metrics import peak_signal_noise_ratio
import torch
import argparse

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
    rendered_image = rendered_image.resize((512, 512))
    rendered_array = np.array(rendered_image) / 255.0  # Normalize to [0, 1]
    return rendered_array

def compute_loss(rendered_image, ground_truth_image):
    """
    Computes the loss between the rendered image and the ground truth image.
    """
    import kornia
    loss = kornia.losses.ssim_loss(
        torch.Tensor(rendered_image).reshape(1, 3, 512, 512),
        torch.Tensor(ground_truth_image).reshape(1, 3, 512, 512),
        window_size=5,
    )
    return loss.detach().numpy().astype('float64') + np.mean((rendered_image - ground_truth_image) ** 2)

def objective_function_translation(
    object_positions,
    iteration_counter,
    iteration_images_dir,
    ground_truth_array,
    modified_json_file,
    executable,
):
    """
    Objective function for optimization. Computes the loss based on the difference
    between the rendered image and the ground truth image.
    """
    iteration_counter[0] += 1
    iteration = iteration_counter[0]

    # Ensure object_positions is in correct shape
    positions = np.reshape(object_positions, (-1)).tolist()

    # Update the JSON file with new object positions
    insert_surface_translation(positions, modified_json_file)

    # Update the output file name to include iteration number
    current_output_file = os.path.join(iteration_images_dir, f"render_{iteration:03d}.png")
    create_ini_file(modified_json_file, current_output_file)

    # Run the renderer
    run_renderer(executable, "default.ini")

    # Load the rendered image
    rendered_image = load_rendered_image(current_output_file)

    # Compute and return the loss
    PSNR = peak_signal_noise_ratio(ground_truth_array, rendered_image)
    loss = compute_loss(rendered_image, ground_truth_array)

    print(f"Iteration {iteration}, Loss: {loss}, PSNR: {PSNR}")

    return loss

def optimize_translation(json_file, output_file, ground_truth_image_path):
    """
    Optimizes the translations of surfaces to minimize the difference between
    the rendered image and the ground truth image.
    """
    # Paths
    script_dir = os.path.dirname(os.path.abspath(__file__))
    executable = os.path.join(script_dir, "projects_ray")
    modified_json_file = "modified_scene.json"

    # Load the ground truth image
    ground_truth_image = Image.open(ground_truth_image_path).convert('RGB')
    ground_truth_image = ground_truth_image.resize((512, 512))  # Ensure same size as rendered image
    ground_truth_array = np.array(ground_truth_image) / 255.0

    # Create a directory to save iteration images
    iteration_images_dir = "iteration_images"
    os.makedirs(iteration_images_dir, exist_ok=True)

    # Counter for iterations
    iteration_counter = [0]  # Use a mutable type to modify inside the nested function

    # Initial guess for object positions (12 values for 4 objects)
    initial_guess = np.random.uniform(0, 5, size=12)

    # Bounds for object positions
    bounds = [(0, 5)] * 12  # Assuming we allow translations between -5 and 5 units

    # Run the optimization
    result = minimize(
        objective_function_translation,
        initial_guess,
        args=(
            iteration_counter,
            iteration_images_dir,
            ground_truth_array,
            modified_json_file,
            executable,
        ),
        method='Nelder-Mead',
        bounds=bounds,
        options={'maxiter': 50, 'disp': True},
    )

    # Get the optimized positions
    optimized_positions = result.x
    # print("Optimized Positions:", optimized_positions)

    # Render the final image with optimized positions
    final_output_file = output_file  # Use the original output file name
    insert_surface_translation(optimized_positions.tolist(), modified_json_file)
    create_ini_file(modified_json_file, final_output_file)
    run_renderer(executable, "default.ini")

    print(f"Optimization completed. Final image saved to '{final_output_file}'.")

def optimize_light_positions(json_file, output_file, ground_truth_image_path):
    """
    Optimizes the light positions to minimize the difference between
    the rendered image and the ground truth image.
    """
    # Paths
    script_dir = os.path.dirname(os.path.abspath(__file__))
    executable = os.path.join(script_dir, "projects_ray")
    modified_json_file = "modified_scene.json"

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
    random_array = np.array([
        [
            np.random.uniform(0, 5),
            np.random.uniform(0, 5),
            np.random.uniform(-5, 0),
        ]
    ])
    initial_light_positions = random_array  # Adjust as needed
    initial_guess = initial_light_positions.flatten()

    # Bounds for light positions
    bounds = [(0, 5), (0, 5), (-5, 0)] * (len(initial_guess) // 3)

    # Run the optimization
    result = minimize(
        objective_function,
        initial_guess,
        args=(
            iteration_counter,
            iteration_images_dir,
            ground_truth_array,
            json_file,
            modified_json_file,
            executable,
        ),
        method='Nelder-Mead',
        bounds=bounds,
        options={'maxiter': 50, 'disp': True},
    )

    # Get the optimized light positions
    optimized_light_positions = np.reshape(result.x, (-1, 3))
    # print("Optimized Light Positions:", optimized_light_positions)

    # Render the final image with optimized light positions
    final_output_file = output_file  # Use the original output file name
    insert_lights_into_json(json_file, optimized_light_positions.tolist(), modified_json_file)
    create_ini_file(modified_json_file, final_output_file)
    run_renderer(executable, "default.ini")

    print(f"Optimization completed. Final image saved to '{final_output_file}'.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Optimize light positions or surface translations to match a ground truth image."
    )
    parser.add_argument("json_file", help="Path to the input JSON file.")
    parser.add_argument("output_file", help="Name of the output PNG file to create.")
    parser.add_argument("ground_truth_image", help="Path to the ground truth image.")
    parser.add_argument(
        "--translation",
        action="store_true",
        help="Optimize surface translations instead of light positions.",
    )
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
    if args.translation:
        optimize_translation(args.json_file, output_file, args.ground_truth_image)
    else:
        optimize_light_positions(args.json_file, output_file, args.ground_truth_image)

    # Path to the folder containing the images
    image_folder = "iteration_images"
    output_gif = "output.gif"

    # Get a sorted list of image file paths
    image_files = sorted(
        [
            os.path.join(image_folder, file)
            for file in os.listdir(image_folder)
            if file.endswith(("png", "jpg", "jpeg"))
        ]
    )

    # Load images
    images = [Image.open(img) for img in image_files]

    # Save images as a GIF
    images[0].save(
        output_gif,
        save_all=True,
        append_images=images[1:],  # Add the rest of the frames
        duration=300,  # Duration of each frame in milliseconds
        loop=0,  # Loop forever
    )

    print(f"GIF created and saved as {output_gif}")