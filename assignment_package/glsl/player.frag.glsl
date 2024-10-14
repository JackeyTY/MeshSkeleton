#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments

uniform sampler2D u_Texture;
uniform int u_Animation;
uniform int u_CurrFrame;
uniform vec3 u_CameraPos;

in vec4 fs_Col;
in vec4 fs_Pos;
in vec4 fs_Nor;
in vec4 fs_LightVec;
in vec2 fs_UV;
in float fs_Part;

out vec4 out_Col;

const vec3 a = vec3(0.5, 0.5, 0.5);
const vec3 b = vec3(0.5, 0.5, 0.5);
const vec3 c = vec3(1.0, 1.0, 1.0);
const vec3 d = vec3(0.00, 0.33, 0.67);

void main()
{
    // Copy the color; there is no shading.
    vec4 diffuseColor = texture(u_Texture, fs_UV);
    float diffuseTerm = dot(normalize(fs_Nor), normalize(fs_LightVec));
    // Avoid negative lighting values
    diffuseTerm = clamp(diffuseTerm, 0, 1);

    float ambientTerm = 0.2;

    float lightIntensity = diffuseTerm + ambientTerm;
    out_Col = vec4(diffuseColor.xyz * lightIntensity, 1.f);

    if (u_Animation == 3)
    {
        if (fs_Part == 1.f || fs_Part == 2.f){
            if (u_CurrFrame >=25 && u_CurrFrame <= 40){
                float t = (u_CurrFrame - 25)/15.f;
                vec3 col = a + b * cos(2 * 3.1415926 * (c * (cos(t * 10)/2+0.5) + d));
                out_Col = vec4(vec3(abs(fs_Nor.x) * col.x, abs(fs_Nor.y) * col.y, abs(fs_Nor.z) * col.z), 1.f);
            }
        }
    }
}
