#version 150

in vec2 fs_UV;

out vec4 color;

uniform sampler2D u_RenderedTexture;
uniform int u_Time;
uniform ivec2 u_Dimensions;

vec3 computeGradient(){
    float stepx = 1.0/u_Dimensions.x;
    float stepy = 1.0/u_Dimensions.y;
    //get the surrounding pixels
    vec4 top = texture(u_RenderedTexture, fs_UV +  vec2(0,stepy));
    vec4 bottom = texture(u_RenderedTexture, fs_UV +  vec2(0,-stepy));
    vec4 tLeft = texture(u_RenderedTexture, fs_UV + vec2(-stepx,stepy));
    vec4 left = texture(u_RenderedTexture, fs_UV + vec2(-stepx,0));
    vec4 bLeft = texture(u_RenderedTexture, fs_UV + vec2(-stepx,-stepy));
    vec4 tRight = texture(u_RenderedTexture, fs_UV +  vec2(stepx,stepy));
    vec4 right = texture(u_RenderedTexture, fs_UV + vec2(stepx,0));
    vec4 bRight = texture(u_RenderedTexture, fs_UV + vec2(stepx,-stepy));
    //multiply with kernals
    vec4 horizontal = 3*tLeft + 10.0*left + 3*bLeft -3 * tRight - 10.0*right - 3* bRight;
    vec4 vertical = 3* tLeft + 10.0*top + 3*tRight - 3* bLeft - 10.0 * bottom - 3* bRight;
    return sqrt(horizontal.rgb * horizontal.rgb + vertical.rgb * vertical.rgb);

}
void main()
{
    // TODO Homework 5
    vec4 c = texture(u_RenderedTexture, fs_UV);
    color = vec4(computeGradient(), c.a);
}
