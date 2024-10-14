#version 330
// noOp.vert.glsl:
// A fragment shader used for post-processing that simply reads the
// image produced in the first render pass by the surface shader
// and outputs it to the frame buffer

uniform int u_Time;
uniform ivec2 u_Dimensions;

in vec2 fs_UV;

out vec4 color;

uniform sampler2D u_RenderedTexture;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p, vec2(127.1, 311.7)),
                 dot(p, vec2(269.5,183.3))))
                 * 43758.5453);
}


float WorleyNoise(vec2 uv) {
    uv *= 5.0; // Now the space is 10x10 instead of 1x1. Change this to any number you want.
    vec2 uvInt = floor(uv);
    vec2 uvFract = fract(uv);
    float minDist = 1.0; // Minimum distance initialized to max.
    vec4 col;
    for(int y = -1; y <= 1; ++y) {
        for(int x = -1; x <= 1; ++x) {
            vec2 neighbor = vec2(float(x), float(y)); // Direction in which neighbor cell lies
            vec2 point = random2(uvInt + neighbor); // Get the Voronoi centerpoint for the neighboring cell
            vec2 diff = neighbor + 0.5f * (sin(50 * point + u_Time * 0.03f) /2.f + 1.f) - uvFract; // Distance between fragment coord and neighbor’s Voronoi point
            float dist = length(diff);
            minDist = min(minDist, dist);
            vec4 text = texture(u_RenderedTexture, point);
            col += 0.01 * text;
        }
    }
    return minDist;
}




void main()
{
    vec2 uv = fs_UV;
    uv.y += sin(u_Time * 0.02f + fs_UV.x * 8.0) * 0.02;
    float t = WorleyNoise(uv);
    vec4 c = texture(u_RenderedTexture, uv).rgba;
    c.b = min(1.f, c.b + 0.5f);
    color = mix(c, vec4(0.3f, 0.3f, 1.0f, c.a), t);
}
