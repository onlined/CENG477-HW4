#version 410

in vec3 position;

// Data from CPU
uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec4 cameraPosition;
uniform float heightFactor;

// Texture-related data
uniform sampler2D rgbTexture;
uniform int widthTexture;
uniform int heightTexture;


// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;

vec3 pos = vec3(0, 0, 0);


vec4 getColor(vec2 coord)
{
  if (coord.x == -1 || coord.x == widthTexture+1 || coord.y == -1 || coord.y == heightTexture+1)
    return vec4(-1, -1, -1, -1);
  return texture(rgbTexture, coord);
}

float getY(vec4 textureColor)
{
  return (0.2126 * textureColor.x + 0.7152 * textureColor.y + 0.0722 * textureColor.z) * heightFactor;
}

vec3 getNeighbour(vec2 coord)
{
  vec4 color = getColor(coord);
  if (color.x == -1)
    return pos;
  return vec3(coord.x, getY(color), coord.y);
}

void main()
{
   vec3 lightPos = vec3(widthTexture/2, widthTexture+heightTexture, heightTexture/2);

   textureCoordinate = vec2(1-position.x/widthTexture, 1-position.z/heightTexture);
   vec4 textureColor = getColor(textureCoordinate);

   pos = vec3(position.x, getY(textureColor), position.z);

   ToLightVector = normalize(lightPos - pos);
   ToCameraVector = normalize(cameraPosition.xyz - pos);

   vec3 point1 = getNeighbour(vec2(pos.x-1, pos.z)) - pos;
   vec3 point2 = getNeighbour(vec2(pos.x, pos.z-1)) - pos;
   vec3 point3 = getNeighbour(vec2(pos.x+1, pos.z+1)) - pos;
   vec3 point4 = getNeighbour(vec2(pos.x+1, pos.z)) - pos;
   vec3 point5 = getNeighbour(vec2(pos.x, pos.z+1)) - pos;
   vec3 point6 = getNeighbour(vec2(pos.x-1, pos.z+1)) - pos;

   vec3 normal1 = cross(point2, point1);
   vec3 normal2 = cross(point3, point2);
   vec3 normal3 = cross(point4, point3);
   vec3 normal4 = cross(point5, point4);
   vec3 normal5 = cross(point6, point5);
   vec3 normal6 = cross(point1, point6);

   vec3 normalsum = normalize(normal1 + normal2 + normal3 + normal4 + normal5 + normal6);

   vertexNormal = normalize(inverse(transpose(mat3x3(MVP))) * normalsum);

   gl_Position = MVP * vec4(pos.xyz, 1.0);
}
