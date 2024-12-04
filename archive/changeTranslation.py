import json
from helper import create_ini_file

def update_scene_with_translations(json_file, output_file, translations):
    """
    Updates the scene JSON with translation values for specific primitives while keeping
    static objects and lights intact.

    Args:
        json_file (str): Path to the input JSON file.
        output_file (str): Path to save the updated JSON file.
        translations (dict): Dictionary with translations for specific primitive types.
                             E.g., {"cube": [1, 2, 3], "cone": [0, 1, 0], ...}
    """
    with open(json_file, 'r') as f:
        data = json.load(f)

    def is_static_object(group):
        """
        Determines if a group is a static object based on its scale.
        """
        return 'scale' in group and group['scale'] in [[6.0, 6.0, 6.0], [6.0, 1.0, 6.0]] and any(
            primitive.get('type') == 'cube' for primitive in group.get('primitives', [])
        )

    def update_translations(groups):
        """
        Recursively updates the translations of primitives in the scene.
        """
        for group in groups:
            # Skip static objects
            if is_static_object(group):
                continue

            # Update translation for primitives
            if 'primitives' in group:
                for primitive in group['primitives']:
                    primitive_type = primitive.get('type')
                    if primitive_type in translations:
                        group['translate'] = translations[primitive_type]

            # Recursively update subgroups
            if 'groups' in group:
                update_translations(group['groups'])

    # Update translations in the root groups
    update_translations(data['groups'])

    # Save the updated JSON
    with open(output_file, 'w') as f:
        json.dump(data, f, indent=2)

    print(f"Updated JSON saved to '{output_file}'.")

if __name__ == "__main__":
    import argparse

    # Input and output JSON files
    parser = argparse.ArgumentParser(description="Update translations for primitives in a scene.")
    parser.add_argument("input_json", help="Path to the input JSON file.")
    parser.add_argument("output_json", help="Path to save the updated JSON file.")
    args = parser.parse_args()

    # Predefined translation values using words
    translations = {
        "cube": [5, 5, 5],
        "cone": [2, 1, 2],
        "sphere": [-3, 2, 0],
        "cylinder": [0, 0, 4]
    }

    # Display translation options to the user
    print("You can customize translations for the following objects:")
    for primitive_type in translations:
        print(f"Current {primitive_type.capitalize()} translation: {translations[primitive_type]}")
        user_input = input(f"Enter new translation for {primitive_type} (e.g., x,y,z or press Enter to keep current): ")
        if user_input.strip():
            try:
                translations[primitive_type] = list(map(float, user_input.split(",")))
            except ValueError:
                print(f"Invalid input for {primitive_type}. Using default translation {translations[primitive_type]}.")

    # Update the scene
    update_scene_with_translations(args.input_json, args.output_json, translations)