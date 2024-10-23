#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D scene;
uniform float brightnessThreshold;

void main()
{
    vec3 color = texture(scene, TexCoords).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722)); // Calculate brightness
    if (brightness > brightnessThreshold) {
        FragColor = vec4(color, 1.0); // Keep bright areas
    } else {
        FragColor = vec4(0.0); // Discard dark areas
    }
}
