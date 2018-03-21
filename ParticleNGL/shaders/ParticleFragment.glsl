#version 410 core

layout (location=0) out vec4 fragColour;
uniform sampler2D tex;

uniform vec4 Colour;
void main()
{
  vec4 colour=texture(tex,gl_PointCoord);
  if(colour.a == 0)
    discard;
  else
    fragColour=colour;
}
