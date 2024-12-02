#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in ve3 FragNormal;

uniform sampler2D texture0;

void main()
{
    //FragColor = texture(texture1, TexCoord);
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}