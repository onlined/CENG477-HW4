#version 410

layout(location = 0) in vec3 position;

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

vec4 getColor(vec2 coord)
{
  return texture(rgbTexture, coord);
}

void main()
{

    // get texture value, compute height
    // compute normal vector using also the heights of neighbor vertices

    // compute toLight vector vertex coordinate in VCS
   // set gl_Position variable correctly to give the transformed vertex position
   vec3 lightPos = vec3(widthTexture/2, widthTexture+heightTexture, heightTexture/2);
   ToLightVector = normalize(lightPos - vec3(position));
	 ToCameraVector = normalize(cameraPosition.xyz - position);

   textureCoordinate = vec2(1-position.x/widthTexture, 1-position.z/heightTexture);
   vec4 textureColor = getColor(textureCoordinate);

   if (position.x == 0)
   {
     if (position.z == 0)
     {

     }
     else if (position.z == heightTexture)
     {

     }
     else
     {

     }
   }
   else if (position.x == widthTexture)
   {

     if (position.z == 0)
     {

     }
     else if (position.z == heightTexture)
     {

     }
     else
     {

     }
   }
   else
   {
     if (position.z == 0)
     {

     }
     else if (position.z == heightTexture)
     {

     }
     else
     {

     }
   }


   vertexNormal = normalize(inverse(transpose(mat3x3(MVP))) * vec3(0, 1, 0));
   vec4 pos = vec4(position.x, (0.2126 * textureColor.x + 0.7152 * textureColor.y + 0.0722 * textureColor.z) * heightFactor, position.z, 1.0);
   gl_Position = MVP * pos;
}
