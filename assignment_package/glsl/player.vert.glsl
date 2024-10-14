#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments

uniform mat4 u_Model;
uniform mat4 u_ViewProj;
uniform int u_Animation;
uniform int u_CurrFrame;
uniform mat4 u_ModelInvTr;
uniform float u_TimeElapsed;

in vec4 vs_Nor;
in vec4 vs_Pos;
in vec4 vs_Col;
in vec2 vs_UV;
in float vs_Part;
out vec4 fs_Col;
out vec4 fs_Nor;
out vec4 fs_Pos;

out vec4 fs_LightVec;
out vec2 fs_UV;
out float fs_Part;

const vec3 xaxis = vec3(1, 0, 0);
const vec3 yaxis = vec3(0, 1, 0);
const vec3 zaxis = vec3(0, 0, 1);
const vec3 shoulderL = vec3(-0.375f, 1.5f, 0.f);
const vec3 shoulderR = vec3(0.375f, 1.5f, 0.f);
const vec3 hipL = vec3(-0.125f, 0.75f, 0.f);
const vec3 hipR = vec3(0.125f, 0.75f, 0.f);
const vec3 hip = vec3(0.f, 0.75f, 0.f);
const vec3 neck = vec3(0.f, 1.5f, 0.f);
const vec3 center = vec3(0.f, 1.0f, 0.f);

const float PI = 3.1415926;

vec3 rotateAxis(vec3 p, vec3 axis, float angle) {
    return mix(dot(axis, p)*axis, p, cos(angle)) + cross(axis,p)*sin(angle);
}

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
    fs_Part = vs_Part;
    mat3 invTranspose = mat3(u_ModelInvTr);
    vec4 vsNor = vec4(invTranspose * vec3(vs_Nor), 0);
    vec4 newNor = vsNor;
    fs_Col = vs_Col;
    fs_UV = vs_UV;
    vec4 newPos = vs_Pos;
    if (u_Animation == 0){
        if (u_CurrFrame <= 6){
            if (vs_Part == 1.f){
                float t = u_CurrFrame/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderL.x, shoulderL.y, shoulderL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderL.x, -shoulderL.y, -shoulderL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 1.f);
            }
            if (vs_Part == 2.f){
                float t = u_CurrFrame/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderR.x, shoulderR.y, shoulderR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;

                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 3.f){
                float t = u_CurrFrame/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = u_CurrFrame/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 1.f);
            }
        } else if (u_CurrFrame <= 12){
            if (vs_Part == 1.f){
                float t = (u_CurrFrame - 6)/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderL.x, shoulderL.y, shoulderL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderL.x, -shoulderL.y, -shoulderL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 1.f);
            }
            if (vs_Part == 2.f){
                float t = (u_CurrFrame - 6)/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderR.x, shoulderR.y, shoulderR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 1.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 1.f);
            }
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 6.f)/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 6)/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
        } else if (u_CurrFrame <= 18){
            if (vs_Part == 1.f){
                float t = (u_CurrFrame - 12)/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderL.x, shoulderL.y, shoulderL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderL.x, -shoulderL.y, -shoulderL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 2.f){
                float t = (u_CurrFrame - 12)/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderR.x, shoulderR.y, shoulderR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 12.f)/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 12)/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
        } else if (u_CurrFrame <= 24){
            if (vs_Part == 1.f){
                float t = (u_CurrFrame - 18)/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderL.x, shoulderL.y, shoulderL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderL.x, -shoulderL.y, -shoulderL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
            if (vs_Part == 2.f){
                float t = (u_CurrFrame - 18)/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderR.x, shoulderR.y, shoulderR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1-t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1-t), 0.f);
            }
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 18)/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 18)/6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1-t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1-t), 0.f);
            }
        }

    }
    else if (u_Animation == 1){
        //135 degree pos
        vec4 raisedArm = newPos;
        vec4 raisedArmNor = newNor;
        mat4 trans = mat4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          shoulderR.x, shoulderR.y, shoulderR.z, 1);
        mat4 transinv = mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
        raisedArm = transinv * raisedArm;
        raisedArm = vec4(rotateAxis(raisedArm.xyz, xaxis, 3 * PI/4.f), 1.f);
        raisedArm = trans * raisedArm;
        raisedArmNor = transinv * raisedArmNor;
        raisedArmNor = vec4(rotateAxis(raisedArmNor.xyz, xaxis, 3 * PI/4.f), 0.f);
        raisedArmNor = trans * raisedArmNor;
        if (u_CurrFrame <= 6){
            if (vs_Part == 2.f){
                float t = u_CurrFrame/6.f;
                newPos = vec4(mix(vs_Pos.xyz, raisedArm.xyz, t), 1.f);
                newNor = vec4(mix(newNor.xyz, raisedArmNor.xyz, t), 0.f);
            }
        } else if (u_CurrFrame <= 12)
        {
            if (vs_Part == 2.f){

                float t = (u_CurrFrame - 6) /6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderR.x, shoulderR.y, shoulderR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/4.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/4.f),0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(raisedArm.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(raisedArmNor.xyz, newNor.xyz, t), 0.f);
            }

        } else if (u_CurrFrame <= 18)
        {
            if (vs_Part == 2.f){
                float t = (u_CurrFrame - 12) /6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderR.x, shoulderR.y, shoulderR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/4.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/4.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(raisedArm.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(raisedArmNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
        }else if (u_CurrFrame <= 24)
        {
            if (vs_Part == 2.f){
                float t = (u_CurrFrame - 18) /6.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderR.x, shoulderR.y, shoulderR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/4.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/4.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(raisedArm.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(raisedArmNor.xyz, newNor.xyz, t), 0.f);
            }
        }
    }    else if (u_Animation == 2){
        vec4 raisedArm = newPos;
        vec4 raisedArmNor = newNor;
        mat4 trans = mat4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          shoulderR.x, shoulderR.y, shoulderR.z, 1);
        mat4 transinv = mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
        raisedArm = transinv * raisedArm;
        raisedArmNor = transinv * newNor;
        raisedArm = vec4(rotateAxis(raisedArm.xyz, xaxis, 5.f/6.f * PI), 1.f);
        raisedArmNor = vec4(rotateAxis(raisedArmNor.xyz, xaxis, 5.f/6.f * PI), 0.f);
        raisedArm = trans * raisedArm;
        raisedArmNor = trans * raisedArmNor;
        raisedArmNor = normalize(raisedArmNor);
        vec4 leftRaised = raisedArm;
        vec4 leftRaisedNor = raisedArmNor;
        leftRaised = transinv * leftRaised;
        leftRaised = vec4(rotateAxis(leftRaised.xyz, zaxis, PI/4.f), 1.f);
        leftRaised = trans * leftRaised;
        leftRaisedNor = transinv * leftRaisedNor;
        leftRaisedNor = vec4(rotateAxis(leftRaisedNor.xyz, zaxis, PI/4.f), 0.f);
        leftRaisedNor = trans * leftRaisedNor;
        leftRaisedNor = normalize(leftRaisedNor);
        vec4 rightRaised = raisedArm;
        vec4 rightRaisedNor = raisedArmNor;
        rightRaised = transinv * rightRaised;
        rightRaised = vec4(rotateAxis(rightRaised.xyz, zaxis, -PI/4.f), 1.f);
        rightRaised = trans * rightRaised;
        rightRaisedNor = transinv * rightRaisedNor;
        rightRaisedNor = vec4(rotateAxis(rightRaisedNor.xyz, zaxis, -PI/4.f), 0.f);
        rightRaisedNor = trans * rightRaisedNor;
        rightRaisedNor = normalize(rightRaisedNor);
        if (u_CurrFrame <= 6){
            if (vs_Part == 2.f){
                float t = u_CurrFrame/6.f;
                newPos = vec4(mix(vs_Pos.xyz, raisedArm.xyz, t), 1.f);
                newNor = vec4(mix(vs_Nor.xyz, raisedArmNor.xyz, t), 0.f);
            }
        }
        else if (u_CurrFrame <= 12)
        {
            if (vs_Part == 2.f){
                float t = (u_CurrFrame - 6) /6.f;
                newPos = vec4(mix(raisedArm.xyz, leftRaised.xyz, t), 1.f);
                newNor = vec4(mix(raisedArmNor.xyz, leftRaisedNor.xyz, t), 0.f);
            }

        }
        else if (u_CurrFrame <= 24)
        {
            if (vs_Part == 2.f){
                float t = (u_CurrFrame - 12) /12.f;
                newPos = vec4(mix(leftRaised.xyz, rightRaised.xyz, t), 1.f);
                newNor = vec4(mix(leftRaisedNor.xyz, rightRaisedNor.xyz, t), 0.f);
            }
        }
        else if (u_CurrFrame <= 36)
        {
            if (vs_Part == 2.f){
                float t = (u_CurrFrame - 24) /12.f;
                newPos = vec4(mix(rightRaised.xyz, leftRaised.xyz, t), 1.f);
                newNor = vec4(mix(rightRaisedNor.xyz, leftRaisedNor.xyz, t), 0.f);
            }
        }
        else if (u_CurrFrame <= 42)
        {
            if (vs_Part == 2.f){
                float t = (u_CurrFrame - 36) /6.f;
                newPos = vec4(mix(leftRaised.xyz, raisedArm.xyz, t), 1.f);
                newNor = vec4(mix(leftRaisedNor.xyz, raisedArmNor.xyz, t), 0.f);
            }
        }
        else if (u_CurrFrame <= 48)
        {
            if (vs_Part == 2.f){
                float t = (u_CurrFrame - 42) /6.f;
                newPos = vec4(mix(raisedArm.xyz, vs_Pos.xyz, t), 1.f);
                newNor = vec4(mix(raisedArmNor.xyz, newNor.xyz, t), 0.f);
            }
       }

       if (vs_Part == 5.f){
           mat4 trans = mat4(1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             neck.x, neck.y, neck.z, 1);
           mat4 transinv = mat4(1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 1, 0,
                               -neck.x, -neck.y, -neck.z, 1);
           newPos = transinv * newPos;
           newPos = vec4(rotateAxis(newPos.xyz, zaxis, PI/6.f), 1.f);
           newPos = trans * newPos;
       }
    }
    else if (u_Animation == 3)
    {
        vec4 facedTo = newPos;
        vec4 facedToNor = newNor;
        mat4 trans = mat4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          center.x, center.y, center.z, 1);
        mat4 transinv = mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            -center.x, -center.y, -center.z, 1);
        facedTo = transinv * facedTo;
        facedTo = vec4(rotateAxis(facedTo.xyz, yaxis, PI/2), 1.f);
        facedTo = trans * facedTo;
        facedToNor = transinv * facedToNor;
        facedToNor = vec4(rotateAxis(facedToNor.xyz, yaxis, PI/2), 0.f);
        facedToNor = trans * facedToNor;
        vec4 facedToMid = facedTo;
        vec4 facedToMidNor = facedToNor;
        facedToMid = transinv * facedToMid;
        facedToMid = vec4(rotateAxis(facedToMid.xyz, yaxis, PI/2), 1.f);
        facedToMid = trans * facedToMid;
        facedToMidNor = transinv * facedToMidNor;
        facedToMidNor = vec4(rotateAxis(facedToMidNor.xyz, yaxis, PI/2), 0.f);
        facedToMidNor = trans * facedToMidNor;

        if (u_CurrFrame <= 6)
        {
            float t = u_CurrFrame/6.f;
            newPos = vec4(mix(vs_Pos.xyz, facedTo.xyz, t), 1.f);
            newNor = vec4(mix(vsNor.xyz, facedToNor.xyz, t), 0.f);
        }else if (u_CurrFrame <= 48)
        {
            float t = (u_CurrFrame - 6)/6.f;
            if (t > 1) t = 1;
            newPos = vec4(mix(facedTo.xyz, facedToMid.xyz, t), 1.f);
            newNor = vec4(mix(facedToNor.xyz, facedToMidNor.xyz, t), 0.f);
            if (u_CurrFrame >= 12){
                vec4 raisedArm = newPos;
                vec4 raisedArmNor = newNor;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderR.x, shoulderR.y, shoulderR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
                raisedArm = transinv * raisedArm;
                raisedArm = vec4(rotateAxis(raisedArm.xyz, zaxis, 3.f/4.f * PI), 1.f);
                raisedArm = trans * raisedArm;
                raisedArmNor = transinv * raisedArmNor;
                raisedArmNor = vec4(rotateAxis(raisedArmNor.xyz, zaxis, 3.f/4.f * PI), 0.f);
                raisedArmNor = trans * raisedArmNor;

                vec4 halfRaised = newPos;
                vec4 halfRaisedNor = newNor;
                trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderL.x, shoulderL.y, shoulderL.z, 1);
                transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderL.x, -shoulderL.y, -shoulderL.z, 1);
                halfRaised = transinv * halfRaised;
                halfRaised = vec4(rotateAxis(halfRaised.xyz, xaxis, -2.f/3.f * PI), 1.f);
                halfRaised = trans * halfRaised;
                halfRaised = transinv * halfRaised;
                halfRaised = vec4(rotateAxis(halfRaised.xyz, yaxis, 1.f/4.f * PI), 1.f);
                halfRaised = trans * halfRaised;
                halfRaisedNor = transinv * halfRaisedNor;
                halfRaisedNor = vec4(rotateAxis(halfRaisedNor.xyz, xaxis, -2.f/3.f * PI), 0.f);
                halfRaisedNor = trans * halfRaisedNor;
                halfRaisedNor = transinv * halfRaisedNor;
                halfRaisedNor = vec4(rotateAxis(halfRaisedNor.xyz, yaxis, 1.f/4.f * PI), 0.f);
                halfRaisedNor = trans * halfRaisedNor;

                vec4 headNod = newPos;
                vec4 headNodNor = newNor;
                trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  neck.x, neck.y, neck.z, 1);
                transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -neck.x, -neck.y, -neck.z, 1);
                headNod = transinv * headNod;
                headNod = vec4(rotateAxis(headNod.xyz, xaxis, 1.f/12.f * PI), 1.f);
                headNod = trans * headNod;
                headNodNor = transinv * headNodNor;
                headNodNor = vec4(rotateAxis(headNodNor.xyz, xaxis, 1.f/12.f * PI), 0.f);
                headNodNor = trans * headNodNor;
                if (u_CurrFrame <= 42){
                    float t = (u_CurrFrame - 12)/6.f;
                    if (t > 1) t = 1;
                    if (vs_Part == 1.f){
                        newPos = vec4(mix(newPos.xyz, raisedArm.xyz, t), 1.f);
                        newNor = vec4(mix(newNor.xyz, raisedArmNor.xyz, t), 0.f);
                    }else if (vs_Part == 2.f){
                        newPos = vec4(mix(newPos.xyz, halfRaised.xyz, t), 1.f);
                        newNor = vec4(mix(newNor.xyz, halfRaisedNor.xyz, t), 0.f);
                    }else if (vs_Part == 5.f){
                        newPos = vec4(mix(newPos.xyz, headNod.xyz, t), 1.f);
                        newNor = vec4(mix(newNor.xyz, headNodNor.xyz, t), 1.f);
                    }
                }else{
                    float t = (u_CurrFrame - 42)/6.f;
                    if (vs_Part == 1.f){
                        newPos = vec4(mix(raisedArm.xyz, newPos.xyz, t), 1.f);
                        newNor = vec4(mix(raisedArmNor.xyz, newNor.xyz, t), 0.f);
                    }else if (vs_Part == 2.f){
                        newPos = vec4(mix(halfRaised.xyz, newPos.xyz, t), 1.f);
                        newNor = vec4(mix(halfRaisedNor.xyz, newNor.xyz, t), 0.f);
                    }else if (vs_Part == 5.f){
                        newPos = vec4(mix(headNod.xyz, newPos.xyz, t), 1.f);
                        newNor = vec4(mix(headNodNor.xyz, newNor.xyz, t), 0.f);
                    }

                }
            }
        }else if (u_CurrFrame <= 54)
        {
            float t = (u_CurrFrame - 48)/6.f;
            newPos = vec4(mix(facedToMid.xyz, facedTo.xyz, t), 1.f);
            newNor = vec4(mix(facedToMidNor.xyz, facedToNor.xyz, t), 0.f);
        }else if (u_CurrFrame <= 60)
        {
            float t = (u_CurrFrame - 54)/6.f;
            newPos = vec4(mix(facedTo.xyz, vs_Pos.xyz, t), 1.f);
            newNor = vec4(mix(facedToNor.xyz, vs_Nor.xyz, t), 0.f);
        }
    }else if (u_Animation == 4)
    {
        vec4 leftRaised = newPos;
        vec4 leftRaisedNor = newNor;
        mat4 trans = mat4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          shoulderL.x, shoulderL.y, shoulderL.z, 1);
        mat4 transinv = mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            -shoulderL.x, -shoulderL.y, -shoulderL.z, 1);
        leftRaised = transinv * leftRaised;
        leftRaised = vec4(rotateAxis(leftRaised.xyz, xaxis, -1.f/4.f * PI), 1.f);
        leftRaised = trans * leftRaised;
        leftRaisedNor = transinv * leftRaisedNor;
        leftRaisedNor = vec4(rotateAxis(leftRaisedNor.xyz, xaxis, -1.f/4.f * PI), 0.f);
        leftRaisedNor = trans * leftRaisedNor;
        vec4 rightRaised = newPos;
        vec4 rightRaisedNor = newNor;
        trans = mat4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          shoulderR.x, shoulderR.y, shoulderR.z, 1);
        transinv = mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
        rightRaised = transinv * rightRaised;
        rightRaised = vec4(rotateAxis(rightRaised.xyz, xaxis, -1.f/4.f * PI), 1.f);
        rightRaised = trans * rightRaised;
        rightRaisedNor = transinv * rightRaisedNor;
        rightRaisedNor = vec4(rotateAxis(rightRaisedNor.xyz, xaxis, -1.f/4.f * PI), 0.f);
        rightRaisedNor = trans * rightRaisedNor;
        trans = mat4(1, 0, 0, 0,
                     0, 1, 0, 0,
                     0, 0, 1, 0,
                     hip.x, hip.y, hip.z, 1);
        transinv = mat4(1, 0, 0, 0,
                     0, 1, 0, 0,
                     0, 0, 1, 0,
                     -hip.x, -hip.y, -hip.z, 1);
        rightRaised = transinv * rightRaised;
        rightRaised = vec4(rotateAxis(rightRaised.xyz, xaxis, -1.f/4.f * PI), 1.f);
        rightRaised = trans * rightRaised;
        leftRaised = transinv * leftRaised;
        leftRaised = vec4(rotateAxis(leftRaised.xyz, xaxis, -1.f/4.f * PI), 1.f);
        leftRaised = trans * leftRaised;
        rightRaisedNor = transinv * rightRaisedNor;
        rightRaisedNor = vec4(rotateAxis(rightRaisedNor.xyz, xaxis, -1.f/4.f * PI), 0.f);
        rightRaisedNor = trans * rightRaisedNor;
        leftRaisedNor = transinv * leftRaisedNor;
        leftRaisedNor = vec4(rotateAxis(leftRaisedNor.xyz, xaxis, -1.f/4.f * PI), 0.f);
        leftRaisedNor = trans * leftRaisedNor;

        if (vs_Part == 1.f)
        {
            newPos = leftRaised;
            newNor = leftRaisedNor;
        }else if (vs_Part == 2.f)
        {
            newPos = rightRaised;
            newNor = rightRaisedNor;
        }else if (vs_Part == 0.f){
            newPos = transinv * newPos;
            newPos = vec4(rotateAxis(newPos.xyz, xaxis, -1.f/4.f * PI), 1.f);
            newPos = trans * newPos;
            newNor = transinv * newNor;
            newNor = vec4(rotateAxis(newNor.xyz, xaxis, -1.f/4.f * PI), 0.f);
            newNor = trans * newNor;
        }else if (vs_Part == 5.f){
            newPos = transinv * newPos;
            newPos = vec4(rotateAxis(newPos.xyz, xaxis, -1.f/4.f * PI), 1.f);
            newPos = trans * newPos;
            newNor = transinv * newNor;
            newNor = vec4(rotateAxis(newNor.xyz, xaxis, -1.f/4.f * PI), 0.f);
            newNor = trans * newNor;
        }
        if(u_CurrFrame <= 3){
            if (vs_Part == 3.f){
                float t = u_CurrFrame/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = u_CurrFrame/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 1.f);
            }
        }else if (u_CurrFrame <= 6)
        {
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 3.f)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 3)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
        }else if (u_CurrFrame <= 9)
        {
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 6.f)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 9)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
        }else if (u_CurrFrame <= 12)
        {
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 9)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 9)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1-t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1-t), 0.f);
            }
        }else if(u_CurrFrame <= 15){
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 12)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 12)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 1.f);
            }
        }else if (u_CurrFrame <= 18)
        {
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 15.f)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 15)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
        }else if (u_CurrFrame <= 21)
        {
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 18.f)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 18)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
        }else if (u_CurrFrame <= 24)
        {
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 21)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 21)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1-t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1-t), 0.f);
            }
        } else if(u_CurrFrame <= 27){
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 24)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 24)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 1.f);
            }
        }else if (u_CurrFrame <= 30)
        {
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 27.f)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 27)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
        }else if (u_CurrFrame <= 33)
        {
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 30.f)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 30)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
        }else if (u_CurrFrame <= 36)
        {
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 33)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 33)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1-t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1-t), 0.f);
            }
        }else if(u_CurrFrame <= 39){
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 36)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 36)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 1.f);
            }
        }else if (u_CurrFrame <= 42)
        {
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 39.f)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 39)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
        }else if (u_CurrFrame <= 45)
        {
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 42.f)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 42)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, t), 0.f);
            }
        }else if (u_CurrFrame <= 48)
        {
            if (vs_Part == 3.f){
                float t = (u_CurrFrame - 45)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipL.x, hipL.y, hipL.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipL.x, -hipL.y, -hipL.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1 - t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1 - t), 0.f);
            }
            if (vs_Part == 4.f){
                float t = (u_CurrFrame - 45)/3.f;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hipR.x, hipR.y, hipR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hipR.x, -hipR.y, -hipR.z, 1);
                newPos = transinv * newPos;
                newPos = vec4(rotateAxis(newPos.xyz, xaxis, -PI/3.f), 1.f);
                newPos = trans * newPos;
                newNor = transinv * newNor;
                newNor = vec4(rotateAxis(newNor.xyz, xaxis, -PI/3.f), 0.f);
                newNor = trans * newNor;
                newPos = vec4(mix(vs_Pos.xyz, newPos.xyz, 1-t), 1.f);
                newNor = vec4(mix(vsNor.xyz, newNor.xyz, 1-t), 0.f);
            }
        }
    }else if (u_Animation == 5)
    {
        vec4 leftRaised = newPos;
        vec4 leftRaisedNor = newNor;
        mat4 trans = mat4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          shoulderL.x, shoulderL.y, shoulderL.z, 1);
        mat4 transinv = mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            -shoulderL.x, -shoulderL.y, -shoulderL.z, 1);
        leftRaised = transinv * leftRaised;
        leftRaised = vec4(rotateAxis(leftRaised.xyz, xaxis, 0.4f * PI), 1.f);
        leftRaised = trans * leftRaised;
        leftRaisedNor = transinv * leftRaisedNor;
        leftRaisedNor = vec4(rotateAxis(leftRaisedNor.xyz, xaxis, 0.4f * PI), 0.f);
        leftRaisedNor = trans * leftRaisedNor;
        vec4 rightRaised = newPos;
        vec4 rightRaisedNor = newNor;
        trans = mat4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          shoulderR.x, shoulderR.y, shoulderR.z, 1);
        transinv = mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
        rightRaised = transinv * rightRaised;
        rightRaised = vec4(rotateAxis(rightRaised.xyz, xaxis, 0.4f * PI), 1.f);
        rightRaised = trans * rightRaised;
        rightRaisedNor = transinv * rightRaisedNor;
        rightRaisedNor = vec4(rotateAxis(rightRaisedNor.xyz, xaxis, 0.4f * PI), 0.f);
        rightRaisedNor = trans * rightRaisedNor;

        vec4 leftLegRaised = newPos;
        vec4 leftLegRaisedNor = newNor;
        trans = mat4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          hipL.x, hipL.y, hipL.z, 1);
        transinv = mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            -hipL.x, -hipL.y, -hipL.z, 1);
        leftLegRaised = transinv * leftLegRaised;
        leftLegRaised = vec4(rotateAxis(leftLegRaised.xyz, xaxis, PI/2.f), 1.f);
        leftLegRaised = trans * leftLegRaised;
        leftLegRaised = transinv * leftLegRaised;
        leftLegRaised = vec4(rotateAxis(leftLegRaised.xyz, yaxis, -PI/4.f), 1.f);
        leftLegRaised = trans * leftLegRaised;

        leftLegRaisedNor = transinv * leftLegRaisedNor;
        leftLegRaisedNor = vec4(rotateAxis(leftLegRaisedNor.xyz, xaxis, PI/2.f), 0.f);
        leftLegRaisedNor = trans * leftLegRaisedNor;
        leftLegRaisedNor = transinv * leftLegRaisedNor;
        leftLegRaisedNor = vec4(rotateAxis(leftLegRaisedNor.xyz, yaxis, -PI/4.f), 0.f);
        leftLegRaisedNor = trans * leftLegRaisedNor;

        vec4 rightLegRaised = newPos;
        vec4 rightLegRaisedNor = newNor;
        trans = mat4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          hipR.x, hipR.y, hipR.z, 1);
        transinv = mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            -hipR.x, -hipR.y, -hipR.z, 1);
        rightLegRaised = transinv * rightLegRaised;
        rightLegRaised = vec4(rotateAxis(rightLegRaised.xyz, xaxis, PI/2.f), 1.f);
        rightLegRaised = trans * rightLegRaised;
        rightLegRaised = transinv * rightLegRaised;
        rightLegRaised = vec4(rotateAxis(rightLegRaised.xyz, yaxis, PI/4.f), 1.f);
        rightLegRaised = trans * rightLegRaised;

        rightLegRaisedNor = transinv * rightLegRaisedNor;
        rightLegRaisedNor = vec4(rotateAxis(rightLegRaisedNor.xyz, xaxis, PI/2.f), 0.f);
        rightLegRaisedNor = trans * rightLegRaisedNor;
        rightLegRaisedNor = transinv * rightLegRaisedNor;
        rightLegRaisedNor = vec4(rotateAxis(rightLegRaisedNor.xyz, yaxis, PI/4.f), 0.f);
        rightLegRaisedNor = trans * rightLegRaisedNor;

        if (u_CurrFrame <= 114){
            float t = u_CurrFrame/6.f;
            if(t > 1) t = 1;
            if (vs_Part == 1.f){
                newPos = vec4(mix(newPos.xyz, leftRaised.xyz, t), 1.f);
            }else if (vs_Part == 2.f){
                newPos = vec4(mix(newPos.xyz, rightRaised.xyz, t), 1.f);
            }else if (vs_Part == 3.f){
                newPos = vec4(mix(newPos.xyz, leftLegRaised.xyz, t), 1.f);
            }else if (vs_Part == 4.f){
                newPos = vec4(mix(newPos.xyz, rightLegRaised.xyz, t), 1.f);
            }
        }else if (u_CurrFrame <= 120){
            float t = (u_CurrFrame - 114)/6.f;
            if (vs_Part == 1.f){
                newPos = vec4(mix(newPos.xyz, leftRaised.xyz, 1 - t), 1.f);
            }else if (vs_Part == 2.f){
                newPos = vec4(mix(newPos.xyz, rightRaised.xyz, 1- t), 1.f);
            }else if (vs_Part == 3.f){
                newPos = vec4(mix(newPos.xyz, leftLegRaised.xyz, 1- t), 1.f);
            }else if (vs_Part == 4.f){
                newPos = vec4(mix(newPos.xyz, rightLegRaised.xyz, 1-t), 1.f);
            }
        }

        vec4 raised = vec4(newPos.x, newPos.y + 0.25f, newPos.z, 1.f);
        float t = mod(u_CurrFrame, 30.f);
        if (t <= 15.f){
            t = t/12.f;
            newPos = vec4(mix(newPos.xyz, raised.xyz, t), 1.f);
        }else{
            t = (t - 15)/15.f;
            newPos = vec4(mix(newPos.xyz, raised.xyz, 1 - t), 1.f);
        }
    } else if (u_Animation == 6)
    {
        vec4 facedTo = newPos;
        vec4 facedToNor = newNor;
        mat4 trans = mat4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          center.x, center.y, center.z, 1);
        mat4 transinv = mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            -center.x, -center.y, -center.z, 1);
        facedTo = transinv * facedTo;
        facedTo = vec4(rotateAxis(facedTo.xyz, yaxis, PI/2), 1.f);
        facedTo = trans * facedTo;
        facedToNor = transinv * facedToNor;
        facedToNor = vec4(rotateAxis(facedToNor.xyz, yaxis, PI/2), 0.f);
        facedToNor = trans * facedToNor;
        vec4 facedToMid = facedTo;
        vec4 facedToMidNor = facedToNor;
        facedToMid = transinv * facedToMid;
        facedToMid = vec4(rotateAxis(facedToMid.xyz, yaxis, PI/2), 1.f);
        facedToMid = trans * facedToMid;
        facedToMidNor = transinv * facedToMidNor;
        facedToMidNor = vec4(rotateAxis(facedToMidNor.xyz, yaxis, PI/2), 0.f);
        facedToMidNor = trans * facedToMidNor;

        if (u_CurrFrame <= 6)
        {
            float t = u_CurrFrame/6.f;
            newPos = vec4(mix(vs_Pos.xyz, facedTo.xyz, t), 1.f);
            newNor = vec4(mix(vsNor.xyz, facedToNor.xyz, t), 0.f);
        }else if (u_CurrFrame <= 48)
        {
            float t = (u_CurrFrame - 6)/6.f;
            if (t > 1) t = 1;
            newPos = vec4(mix(facedTo.xyz, facedToMid.xyz, t), 1.f);
            newNor = vec4(mix(facedToNor.xyz, facedToMidNor.xyz, t), 0.f);
            if (u_CurrFrame >= 12){
                vec4 raisedArm = newPos;
                vec4 raisedArmNor = newNor;
                mat4 trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderR.x, shoulderR.y, shoulderR.z, 1);
                mat4 transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderR.x, -shoulderR.y, -shoulderR.z, 1);
                raisedArm = transinv * raisedArm;
                raisedArm = vec4(rotateAxis(raisedArm.xyz, xaxis, -1.f/4.f * PI), 1.f);
                raisedArm = trans * raisedArm;
                raisedArm = transinv * raisedArm;
                raisedArm = vec4(rotateAxis(raisedArm.xyz, zaxis, -1.f/4.f * PI), 1.f);
                raisedArm = trans * raisedArm;
                raisedArmNor = transinv * raisedArmNor;
                raisedArmNor = vec4(rotateAxis(raisedArmNor.xyz, xaxis, -1.f/4.f * PI), 0.f);
                raisedArmNor = trans * raisedArmNor;
                raisedArmNor = transinv * raisedArmNor;
                raisedArmNor = vec4(rotateAxis(raisedArmNor.xyz, zaxis, -1.f/4.f * PI), 0.f);
                raisedArmNor = trans * raisedArmNor;

                vec4 halfRaised = newPos;
                vec4 halfRaisedNor = newNor;
                trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  shoulderL.x, shoulderL.y, shoulderL.z, 1);
                transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -shoulderL.x, -shoulderL.y, -shoulderL.z, 1);
                halfRaised = transinv * halfRaised;
                halfRaised = vec4(rotateAxis(halfRaised.xyz, xaxis, 2.f/3.f * PI), 1.f);
                halfRaised = trans * halfRaised;
                halfRaised = transinv * halfRaised;
                halfRaised = vec4(rotateAxis(halfRaised.xyz, yaxis, 1.f/4.f * PI), 1.f);
                halfRaised = trans * halfRaised;
                halfRaisedNor = transinv * halfRaisedNor;
                halfRaisedNor = vec4(rotateAxis(halfRaisedNor.xyz, xaxis, 2.f/3.f * PI), 0.f);
                halfRaisedNor = trans * halfRaisedNor;
                halfRaisedNor = transinv * halfRaisedNor;
                halfRaisedNor = vec4(rotateAxis(halfRaisedNor.xyz, yaxis, 1.f/4.f * PI), 0.f);
                halfRaisedNor = trans * halfRaisedNor;

                trans = mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  hip.x, hip.y, hip.z, 1);
                transinv = mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -hip.x, -hip.y, -hip.z, 1);

                raisedArm = transinv * raisedArm;
                raisedArm = vec4(rotateAxis(raisedArm.xyz, xaxis, 1.f/4.f * PI), 1.f);
                raisedArm = trans * raisedArm;
                halfRaised = transinv * halfRaised;
                halfRaised = vec4(rotateAxis(halfRaised.xyz, xaxis, 1.f/4.f * PI), 1.f);
                halfRaised = trans * halfRaised;
                raisedArmNor = transinv * raisedArmNor;
                raisedArmNor = vec4(rotateAxis(raisedArmNor.xyz, xaxis, 1.f/4.f * PI), 0.f);
                raisedArmNor = trans * raisedArmNor;
                halfRaisedNor = transinv * halfRaisedNor;
                halfRaisedNor = vec4(rotateAxis(halfRaisedNor.xyz, xaxis, 1.f/4.f * PI), 0.f);
                halfRaisedNor = trans * halfRaisedNor;

                vec4 bend = newPos;
                vec4 bendNor = newNor;
                bend = transinv * bend;
                bend = vec4(rotateAxis(bend.xyz, xaxis, 1.f/4.f * PI), 1.f);
                bend = trans * bend;
                bendNor = transinv * bendNor;
                bendNor = vec4(rotateAxis(bendNor.xyz, xaxis, 1.f/4.f * PI), 0.f);
                bendNor = trans * bendNor;

                if (u_CurrFrame <= 42){
                    float t = (u_CurrFrame - 12)/6.f;
                    if (t > 1) t = 1;
                    if (vs_Part == 1.f){
                        newPos = vec4(mix(newPos.xyz, raisedArm.xyz, t), 1.f);
                        newNor = vec4(mix(newNor.xyz, raisedArmNor.xyz, t), 0.f);
                    }else if (vs_Part == 2.f){
                        newPos = vec4(mix(newPos.xyz, halfRaised.xyz, t), 1.f);
                        newNor = vec4(mix(newNor.xyz, halfRaisedNor.xyz, t), 0.f);
                    }else if (vs_Part == 0.f){
                        newPos = vec4(mix(newPos.xyz, bend.xyz, t), 1.f);
                        newNor = vec4(mix(newNor.xyz, bendNor.xyz, t), 0.f);
                    }else if (vs_Part == 5.f){
                        newPos = vec4(mix(newPos.xyz, bend.xyz, t), 1.f);
                        newNor = vec4(mix(newNor.xyz, bendNor.xyz, t), 0.f);
                    }
                }else{
                    float t = (u_CurrFrame - 42)/6.f;
                    if (vs_Part == 1.f){
                        newPos = vec4(mix(raisedArm.xyz, newPos.xyz, t), 1.f);
                        newNor = vec4(mix(raisedArmNor.xyz, newNor.xyz, t), 0.f);
                    }else if (vs_Part == 2.f){
                        newPos = vec4(mix(halfRaised.xyz, newPos.xyz, t), 1.f);
                        newNor = vec4(mix(halfRaisedNor.xyz, newNor.xyz, t), 0.f);
                    }else if (vs_Part == 0.f){
                        newPos = vec4(mix(bend.xyz, newPos.xyz, t), 1.f);
                        newNor = vec4(mix(bendNor.xyz, newNor.xyz, t), 0.f);
                    }else if (vs_Part == 5.f){
                        newPos = vec4(mix(bend.xyz, newPos.xyz, t), 1.f);
                        newNor = vec4(mix(bendNor.xyz, newNor.xyz, t), 0.f);
                    }

                }
            }
        }else if (u_CurrFrame <= 54)
        {
            float t = (u_CurrFrame - 48)/6.f;
            newPos = vec4(mix(facedToMid.xyz, facedTo.xyz, t), 1.f);
            newNor = vec4(mix(facedToMidNor.xyz, facedToNor.xyz, t), 0.f);
        }else if (u_CurrFrame <= 60)
        {
            float t = (u_CurrFrame - 54)/6.f;
            newPos = vec4(mix(facedTo.xyz, vs_Pos.xyz, t), 1.f);
            newNor = vec4(mix(facedToNor.xyz, vs_Nor.xyz, t), 0.f);
        }
    }

    vec4 modelposition = u_Model * newPos;

    //built-in things to pass down the pipeline
    gl_Position = u_ViewProj * modelposition;
    fs_Pos = u_ViewProj * modelposition;
    fs_Nor = vec4(normalize(vec3(newNor)), 0);
    fs_LightVec = getLightDir();
}
