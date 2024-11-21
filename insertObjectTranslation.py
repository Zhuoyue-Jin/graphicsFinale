# import json
# from helper import create_ini_file
# from render_scene import run_raytracer
# # Initial JSON
# data = {
#     "name": "root",
#     "globalData": {
#         "ambientCoeff": 0.5,
#         "diffuseCoeff": 0.5,
#         "specularCoeff": 0.5,
#         "transparentCoeff": 0
#     },
#     "cameraData": {
#         "position": [-5.0, 1.0, 0.0],
#         "up": [0.0, 1.0, 0.0],
#         "focus": [0, 0, 0],
#         "heightAngle": 50.0
#     },
#     "groups": [
#         {
#             "lights": [
#                 {"type": "directional", "color": [1.0, 1.0, 1.0], "direction": [0.0, -1.0, 0.0]}
#             ]
#         },
#         {
#             "lights": [
#                 {"type": "directional", "color": [1.0, 1.0, 1.0], "direction": [1.0, 0.0, 0.0]}
#             ]
#         },
#         {
#             "lights": [
#                 {"type": "directional", "color": [1.0, 1.0, 1.0], "direction": [0.0, 0.0, 1.0]}
#             ]
#         },
#         {
#             "groups": [
#                 {
#                     "translate": [0.0, -0.6, 0.0],
#                     "scale": [6.0, 0.1, 6.0],
#                     "primitives": [
#                         {"type": "cube", "diffuse": [0.1, 0.4, 0.8]}
#                     ]
#                 },
#                 {
#                     "translate": [0.0, 0.0, 5.0],
#                     "scale": [6.0, 6.0, 6.0],
#                     "primitives": [
#                         {"type": "cube", "diffuse": [1.0, 0.0, 0.8]}
#                     ]
#                 },
#                 {
#                     "translate": [5.0, 0.0, 0.0],
#                     "scale": [6.0, 6.0, 6.0],
#                     "primitives": [
#                         {"type": "cube", "diffuse": [0.0, 1.0, 0.8]}
#                     ]
#                 }
#             ]
#         }
#     ]
# }

# # Collect translations for new objects
# translations = {}
# for primitive in ["cube", "cone", "sphere", "cylinder"]:
#     print(f"Enter translation values for {primitive} (as x, y, z):")
#     x, y, z = map(float, input().split(","))
#     translations[primitive] = [x, y, z]

# # Define new objects
# new_objects = [
#     {
#         "translate": translations["cylinder"],
#         "primitives": [
#             {
#                 "type": "cylinder",
#                 "diffuse": [0.8, 0.6, 0.6],
#                 "specular": [1.0, 1.0, 1.0],
#                 "shininess": 15.0
#             }
#         ]
#     },
#     {
#         "translate": translations["cone"],
#         "primitives": [
#             {
#                 "type": "cone",
#                 "diffuse": [0.6, 0.6, 0.8],
#                 "specular": [1.0, 1.0, 1.0],
#                 "shininess": 15.0
#             }
#         ]
#     },
#     {
#         "translate": translations["sphere"],
#         "primitives": [
#             {
#                 "type": "sphere",
#                 "diffuse": [0.6, 0.8, 0.6],
#                 "specular": [1.0, 1.0, 1.0],
#                 "shininess": 15.0
#             }
#         ]
#     },
#     {
#         "translate": translations["cube"],
#         "primitives": [
#             {
#                 "type": "cube",
#                 "diffuse": [0.7, 0.7, 0.7],
#                 "specular": [1.0, 1.0, 1.0],
#                 "shininess": 15.0
#             }
#         ]
#     }
# ]

# # Insert new objects into the JSON
# data["groups"][-1]["groups"].extend(new_objects)
# modified_json_file = "hello.json"
# # Print updated JSON
# with open(modified_json_file, 'w') as f:
#     json.dump(data, f, indent=2)
# create_ini_file(modified_json_file)
# run_raytracer("default.ini")



import json

def insert_surface_translation(translation_values):
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
    print(json.dumps(data, indent=2))

    return data