#version 410 core

layout (location=0) out vec4 fragColour;
uniform sampler2D tex;

uniform vec4 Colour;
void main()
{

  fragColour=texture(tex,gl_PointCoord);
}
