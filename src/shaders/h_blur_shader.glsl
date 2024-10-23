#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform float blurAmount; // Can be set to control blur strength

void main()
{
    vec2 tex_offset = vec2(blurAmount, 0.0); // Blur horizontally
    vec3 result = vec3(0.0);
    result += texture(image, TexCoords - 4.0 * tex_offset).rgb * 0.05;
    result += texture(image, TexCoords - 3.0 * tex_offset).rgb * 0.09;
    result += texture(image, TexCoords - 2.0 * tex_offset).rgb * 0.12;
    result += texture(image, TexCoords - 1.0 * tex_offset).rgb * 0.15;
    result += texture(image, TexCoords).rgb * 0.16;
    result += texture(image, TexCoords + 1.0 * tex_offset).rgb * 0.15;
    result += texture(image, TexCoords + 2.0 * tex_offset).rgb * 0.12;
    result += texture(image, TexCoords + 3.0 * tex_offset).rgb * 0.09;
    result += texture(image, TexCoords + 4.0 * tex_offset).rgb * 0.05;

    FragColor = vec4(result, 1.0);
}
