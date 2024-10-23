#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D scene; // Original scene texture
uniform sampler2D bloomBlur; // Blurred bright areas

void main()
{
    vec3 sceneColor = texture(scene, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    FragColor = vec4(sceneColor + bloomColor, 1.0); // Add the bloom to the original scene
}
