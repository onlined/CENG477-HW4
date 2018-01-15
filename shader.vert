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

vec4 pos = vec4(0, 0, 0, 0);

vec4 getColor(vec2 coord)
{
  if (coord.x == -1 || coord.x == widthTexture+1 || coord.y == -1 || coord.y == heightTexture+1)
    return vec4(pos);
  return texture(rgbTexture, coord);
}

float getY(vec4 textureColor)
{
  return (0.2126 * textureColor.x + 0.7152 * textureColor.y + 0.0722 * textureColor.z) * heightFactor;
}

void main()
{

    // get texture value, compute height
    // compute normal vector using also the heights of neighbor vertices

    // compute toLight vector vertex coordinate in VCS
   // set gl_Position variable correctly to give the transformed vertex position
   vec3 lightPos = vec3(widthTexture/2, widthTexture+heightTexture, heightTexture/2);

   textureCoordinate = vec2(1-position.x/widthTexture, 1-position.z/heightTexture);
   vec4 textureColor = getColor(textureCoordinate);


   pos = vec4(position.x, getY(textureColor), position.z, 1.0);

   ToLightVector = normalize(lightPos - position);
   ToCameraVector = normalize(cameraPosition.xyz - position);

   vec3 point1 = vec3(position.x-1, getY(getColor(vec2(position.x-1, position.z))), position.z) - position;
   vec3 point2 = vec3(position.x, getY(getColor(vec2(position.x, position.z-1))), position.z-1) - position;
   vec3 point3 = vec3(position.x+1, getY(getColor(vec2(position.x+1, position.z+1))), position.z+1) - position;
   vec3 point4 = vec3(position.x+1, getY(getColor(vec2(position.x+1, position.z))), position.z) - position;
   vec3 point5 = vec3(position.x, getY(getColor(vec2(position.x, position.z+1))), position.z+1) - position;
   vec3 point6 = vec3(position.x-1, getY(getColor(vec2(position.x-1, position.z+1))), position.z+1) - position;

   vec3 normal1 = cross(point2, point1);
   vec3 normal2 = cross(point3, point2);
   vec3 normal3 = cross(point4, point3);
   vec3 normal4 = cross(point5, point4);
   vec3 normal5 = cross(point6, point5);
   vec3 normal6 = cross(point1, point6);

   vec3 normalsum = normalize(normal1 + normal2 + normal3 + normal4 + normal5 + normal6);

   vertexNormal = normalize(inverse(transpose(mat3x3(MVP))) * normalsum);

   gl_Position = MVP * pos;
}
