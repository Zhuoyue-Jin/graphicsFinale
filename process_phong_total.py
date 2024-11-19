import json

def process_phong_total(json_file):
    # Load the JSON data from file
    with open(json_file, 'r') as file:
        data = json.load(file)

    # Process the global data
    global_data = data.get("globalData", {})
    print("Global Data:")
    print(f"  Ambient Coeff: {global_data.get('ambientCoeff', 'N/A')}")
    print(f"  Diffuse Coeff: {global_data.get('diffuseCoeff', 'N/A')}")
    print(f"  Specular Coeff: {global_data.get('specularCoeff', 'N/A')}")
    print(f"  Transparent Coeff: {global_data.get('transparentCoeff', 'N/A')}")
    print()

    # Process the camera data
    camera_data = data.get("cameraData", {})
    print("Camera Data:")
    print(f"  Position: {camera_data.get('position', 'N/A')}")
    print(f"  Up Vector: {camera_data.get('up', 'N/A')}")
    print(f"  Focus Point: {camera_data.get('focus', 'N/A')}")
    print(f"  Height Angle: {camera_data.get('heightAngle', 'N/A')}")
    print()

    # Process the groups
    print("Groups:")
    for group in data.get("groups", []):
        process_group(group, indent=2)

def process_group(group, indent=0):
    # Process transformations
    translate = group.get("translate", [])
    scale = group.get("scale", [])
    lights = group.get("lights", [])
    primitives = group.get("primitives", [])
    subgroups = group.get("groups", [])

    print(" " * indent + "Group:")
    if translate:
        print(" " * (indent + 2) + f"Translate: {translate}")
    if scale:
        print(" " * (indent + 2) + f"Scale: {scale}")
    if lights:
        print(" " * (indent + 2) + "Lights:")
        for light in lights:
            print(" " * (indent + 4) + f"Type: {light.get('type', 'N/A')}")
            print(" " * (indent + 4) + f"Color: {light.get('color', 'N/A')}")
            print(" " * (indent + 4) + f"Attenuation Coeff: {light.get('attenuationCoeff', 'N/A')}")

    # Process primitives
    if primitives:
        print(" " * (indent + 2) + "Primitives:")
        for primitive in primitives:
            print(" " * (indent + 4) + f"Type: {primitive.get('type', 'N/A')}")
            print(" " * (indent + 4) + f"Diffuse: {primitive.get('diffuse', 'N/A')}")
            print(" " * (indent + 4) + f"Specular: {primitive.get('specular', 'N/A')}")
            print(" " * (indent + 4) + f"Shininess: {primitive.get('shininess', 'N/A')}")

    # Recursively process subgroups
    if subgroups:
        print(" " * (indent + 2) + "Subgroups:")
        for subgroup in subgroups:
            process_group(subgroup, indent=indent + 4)

# Main entry point
if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Process phong_total JSON.")
    parser.add_argument("json_file", help="Path to the JSON file to process.")
    args = parser.parse_args()

    process_phong_total(args.json_file)