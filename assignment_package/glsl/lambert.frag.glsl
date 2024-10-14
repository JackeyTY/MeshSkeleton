#version 150
// ^ Change this to version 130 if you have compatibility issues

// This is a fragment shader. If you've opened this file first, please
// open and read lambert.vert.glsl before reading on.
// Unlike the vertex shader, the fragment shader actually does compute
// the shading of geometry. For every pixel in your program's output
// screen, the fragment shader is run for every bit of geometry that
// particular pixel overlaps. By implicitly interpolating the position
// data passed into the fragment shader by the vertex shader, the fragment shader
// can compute what color to apply to its pixel based on things like vertex
// position, light position, and vertex color.
uniform sampler2D u_Texture;
uniform sampler2D u_Depth;
uniform vec4 u_Color; // The color with which to render this instance of geometry.
uniform int u_Time;
uniform float u_TimeElapsed;
uniform vec3 u_CameraPos;
// These are the interpolated values out of the rasterizer, so you can't know
// their specific values without knowing the vertices that contributed to them
in vec4 fs_Pos;
in vec2 fs_UV;
in float fs_isAnimatable;
in vec4 fs_Nor;
in vec4 fs_LightVec;
in vec4 fs_Col;
in vec4 fs_ShadowCoord;

out vec4 out_Col; // This is the final output color that you will see on your
                  // screen for the pixel that is currently being processed.

float random1(vec3 p) {
    return fract(sin(dot(p,vec3(127.1, 311.7, 191.999)))
                 *43758.5453);
}

float mySmoothStep(float a, float b, float t) {
    t = smoothstep(0, 1, t);
    return mix(a, b, t);
}

float cubicTriMix(vec3 p) {
    vec3 pFract = fract(p);
    float llb = random1(floor(p) + vec3(0,0,0));
    float lrb = random1(floor(p) + vec3(1,0,0));
    float ulb = random1(floor(p) + vec3(0,1,0));
    float urb = random1(floor(p) + vec3(1,1,0));

    float llf = random1(floor(p) + vec3(0,0,1));
    float lrf = random1(floor(p) + vec3(1,0,1));
    float ulf = random1(floor(p) + vec3(0,1,1));
    float urf = random1(floor(p) + vec3(1,1,1));

    float mixLoBack = mySmoothStep(llb, lrb, pFract.x);
    float mixHiBack = mySmoothStep(ulb, urb, pFract.x);
    float mixLoFront = mySmoothStep(llf, lrf, pFract.x);
    float mixHiFront = mySmoothStep(ulf, urf, pFract.x);

    float mixLo = mySmoothStep(mixLoBack, mixLoFront, pFract.z);
    float mixHi = mySmoothStep(mixHiBack, mixHiFront, pFract.z);

    return mySmoothStep(mixLo, mixHi, pFract.y);
}

float fbm(vec3 p) {
    float amp = 0.5;
    float freq = 4.0;
    float sum = 0.0;
    for(int i = 0; i < 8; i++) {
        sum += cubicTriMix(p * freq) * amp;
        amp *= 0.5;
        freq *= 2.0;
    }
    return sum;
}

const float maxDist = 200.f;
const float minDist = 150.f;
float fogIntensity(float distance){
    if (distance > maxDist) return 1.f;
    if (distance < minDist) return 0.f;
    return 1 - (maxDist - distance)/(maxDist - minDist);
}

const vec3 fogColor = vec3(0.84f, 0.81f, 1.0f);

void main()
{
    // Material base color (before shading)
        vec4 diffuseColor;
        float specularIntensity = 0.f;
        if (fs_isAnimatable == 1.f){
            diffuseColor = texture(u_Texture, vec2(fs_UV.x + mod(u_Time * 0.0005, 0.125), fs_UV.y));
            //water wave specular
            if (fs_UV.y >= 0.1875f){
                vec4 viewVec = normalize(vec4(u_CameraPos, 1.f) - fs_Pos);
                vec4 H = normalize(viewVec + fs_LightVec);
                specularIntensity = max(pow(dot(H, normalize(fs_Nor)), 80.0f), 0);
            }

        }else{
            diffuseColor = texture(u_Texture, fs_UV);
        }
        //float alpha = diffuseColor.a;
//        diffuseColor = diffuseColor * (0.5 * fbm(fs_Pos.xyz) + 0.5);

        // Calculate the diffuse term for Lambert shading
        float diffuseTerm = dot(normalize(fs_Nor), normalize(fs_LightVec));
        // Avoid negative lighting values
        diffuseTerm = clamp(diffuseTerm, 0, 1);

        float ambientTerm = 0.2;
        float lightIntensity = diffuseTerm + ambientTerm;   //Add a small float value to the color multiplier
                                                            //to simulate ambient lighting. This ensures that faces that are not
                                                            //lit by our point light are not completely black.

        // distance fog
        float d = distance(u_CameraPos, fs_Pos.xyz);
        float t = fogIntensity(d);
        vec3 textureColor = diffuseColor.rgb * (lightIntensity + specularIntensity);
        vec3 mixedColor = mix(textureColor, fogColor, t);
        //need to account for alpha, if not then blocks far away are translucent with shapes (which looks weird)
        //if fogIntensity is 1, then block is far enough to be completely invisible
        //alpha - the further the camera is from a block, the more invisible the fog/block is
        float mixedAlpha = mix(diffuseColor.a, 1 - t, t);

        // apply shadow
        float time = mod(u_TimeElapsed, 216000.0) / 216000;
        float bias = max(0.005 * (1.0 - dot(fs_Nor.xyz, fs_LightVec.xyz)), 0.0005);
        float shadow;
        if (time >= 0.25 && time <= 0.75) {
            shadow = 0.3;
        } else {
            shadow = texture(u_Depth, fs_ShadowCoord.xy).r < (fs_ShadowCoord.z - bias) ? 0.5 : 1.0;
        }

        // Compute final shaded color (with distance fog)
        out_Col = vec4(mixedColor * shadow, mixedAlpha);
}
