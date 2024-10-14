#version 150
// ^ Change this to version 130 if you have compatibility issues

//This is a vertex shader. While it is called a "shader" due to outdated conventions, this file
//is used to apply matrix transformations to the arrays of vertex data passed to it.
//Since this code is run on your GPU, each vertex is transformed simultaneously.
//If it were run on your CPU, each vertex would have to be processed in a FOR loop, one at a time.
//This simultaneous transformation allows your program to run much faster, especially when rendering
//geometry with millions of vertices.


uniform mat4 u_Model;       // The matrix that defines the transformation of the
                            // object we're rendering. In this assignment,
                            // this will be the result of traversing your scene graph.

uniform mat4 u_ModelInvTr;  // The inverse transpose of the model matrix.
                            // This allows us to transform the object's normals properly
                            // if the object has been non-uniformly scaled.

uniform mat4 u_ViewProj;    // The matrix that defines the camera's transformation.
                            // We've written a static matrix for you to use for HW2,
                            // but in HW3 you'll have to generate one yourself

uniform mat4 u_ShadowViewProj;

uniform vec4 u_Color;       // When drawing the cube instance, we'll set our uniform color to represent different block types.
uniform int u_Time;

uniform float u_TimeElapsed;

in vec4 vs_Pos;             // The array of vertex positions passed to the shader

in vec4 vs_Nor;             // The array of vertex normals passed to the shader

in vec2 vs_UV;
in float vs_isAnimatable;

out vec4 fs_Pos;
out vec4 fs_ShadowCoord;
out vec4 fs_Nor;            // The array of normals that has been transformed by u_ModelInvTr. This is implicitly passed to the fragment shader.
out vec4 fs_LightVec;       // The direction in which our virtual light lies, relative to each vertex. This is implicitly passed to the fragment shader.
out vec2 fs_UV;
out float fs_isAnimatable;

vec4 getLightDir() {
    float time = mod(u_TimeElapsed, 216000.0) / 216000;
    vec3 p = vec3(0.0, 5000.0, 0.0);
    float rot = 6.28318530718 * time;
    vec3 curLocation = vec3(cos(rot) * p.x - sin(rot) * p.y,
                            sin(rot) * p.x + cos(rot) * p.y,
                            p.z);
    vec3 curDir = normalize(curLocation);
    vec3 nightLight = normalize(vec3(0.5, 1, 0.75));
    vec3 dir;
    if (time > 0.25 && time < 0.27) {
        dir = mix(curDir, nightLight, (time - 0.25) / 0.02);
    } else if (time >= 0.27 && time <= 0.73) {
        dir = nightLight;
    } else if (time > 0.73 && time < 0.75) {
        dir = mix(nightLight, curDir, (time - 0.73) / 0.02);
    } else {
        dir = curDir;
    }
    return vec4(normalize(dir), 0.0);
}

void main()
{
    fs_UV = vs_UV;
    //easy way to tell apart water and lava using uv, naive but works here
    //water wave
    if (vs_isAnimatable == 1.f && fs_UV.y >= 0.1875f){
        vec4 pos = vs_Pos;
        pos.y += 0.75f * sin(u_Time * 0.03f + pos.x * 6);
        //displace normal
        float nx = 1.f;
        float ny = 0.75f * cos(u_Time * 0.03f + pos.x * 6);
        vec2 newNor = vec2(-ny, nx);
        fs_Nor = vec4(normalize(newNor).xy, 0.f, 0.f);
        fs_Pos = pos;
    }else{
        fs_Pos = vs_Pos;
        fs_Nor = vs_Nor;
    }
    fs_isAnimatable = vs_isAnimatable;

    mat3 invTranspose = mat3(u_ModelInvTr);
    fs_Nor = vec4(invTranspose * vec3(fs_Nor), 0);          // Pass the vertex normals to the fragment shader for interpolation.
                                                            // Transform the geometry's normals by the inverse transpose of the
                                                            // model matrix. This is necessary to ensure the normals remain
                                                            // perpendicular to the surface after the surface is transformed by
                                                            // the model matrix.


    vec4 modelposition = u_Model * fs_Pos;   // Temporarily store the transformed vertex positions for use below

    fs_LightVec = getLightDir();  // Compute the direction in which the light source lies
    fs_ShadowCoord = u_ShadowViewProj * modelposition;
    mat4 biasMatrix = mat4(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
        );
    fs_ShadowCoord = biasMatrix * fs_ShadowCoord;
    gl_Position = u_ViewProj * modelposition;// gl_Position is a built-in variable of OpenGL which is
                                             // used to render the final positions of the geometry's vertices
}
