from helper import run_projects_ray
import os

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