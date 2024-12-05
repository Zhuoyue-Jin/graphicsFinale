#version 330 core

// Input UV coordinates
in vec2 fragTexCoord;

// Sampler for the texture
uniform sampler2D txture;

uniform bool processing;
uniform bool blur;

// Texture size (to compute offsets for neighboring pixels)
uniform vec2 texSize;


out vec4 fragColor;

void main()
{
    vec4 texColor = texture(txture, fragTexCoord);

    if (blur) {

        float kernel[25] = float[25](
            1.0/25, 1.0/25, 1.0/25, 1.0/25, 1.0/25,
            1.0/25, 1.0/25, 1.0/25, 1.0/25, 1.0/25,
            1.0/25, 1.0/25, 1.0/25, 1.0/25, 1.0/25,
            1.0/25, 1.0/25, 1.0/25, 1.0/25, 1.0/25,
            1.0/25, 1.0/25, 1.0/25, 1.0/25, 1.0/25
        );


        vec4 blurColor = vec4(0.0);

        vec2 texelOffset = 1.0 / texSize;
        for (int i = -2; i <= 2; ++i) {
            for (int j = -2; j <= 2; ++j) {
                vec2 offset = vec2(i, j) * texelOffset;
                int kernelIndex = (i + 2) * 5 + (j + 2);
                blurColor += texture(txture, fragTexCoord + offset) * kernel[kernelIndex];
            }
        }

        texColor = blurColor;
    }

    if (processing) {

        float luminance = dot(texColor.rgb, vec3(0.2126, 0.7152, 0.0722));
        texColor = vec4(luminance, luminance, luminance, texColor.a);
    }


    fragColor = texColor;
}
