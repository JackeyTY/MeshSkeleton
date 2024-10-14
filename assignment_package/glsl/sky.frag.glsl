#version 150

uniform mat4 u_ViewProj;    // We're actually passing the inverse of the viewproj
                            // from our CPU, but it's named u_ViewProj so we don't
                            // have to bother rewriting our ShaderProgram class

uniform ivec2 u_Dimensions; // Screen dimensions

uniform vec3 u_Eye; // Camera pos

uniform int u_Time;
uniform float u_TimeElapsed;

out vec4 outColor;

const float PI = 3.14159265359;
const float TWO_PI = 6.28318530718;
const vec3 starColor = vec3(255, 210, 125) / 255.0;
const vec3 moonColor = vec3(255, 250, 134) / 255.0;

//  ===============
//  ==== NOISE ====
//  ===============
vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

vec3 random3( vec3 p ) {
    return fract(sin(vec3(dot(p,vec3(127.1, 311.7, 191.999)),
                          dot(p,vec3(269.5, 183.3, 765.54)),
                          dot(p, vec3(420.69, 631.2,109.21))))
                 *43758.5453);
}

float WorleyNoise3D(vec3 p)
{
    // Tile the space
    vec3 pointInt = floor(p);
    vec3 pointFract = fract(p);

    float minDist = 1.0; // Minimum distance initialized to max.

    // Search all neighboring cells and this cell for their point
    for(int z = -1; z <= 1; z++)
    {
        for(int y = -1; y <= 1; y++)
        {
            for(int x = -1; x <= 1; x++)
            {
                vec3 neighbor = vec3(float(x), float(y), float(z));

                // Random point inside current neighboring cell
                vec3 point = random3(pointInt + neighbor);

                // Animate the point
                point = 0.5 + 0.5 * sin(u_Time * 0.01 + 6.2831 * point); // 0 to 1 range

                // Compute the distance b/t the point and the fragment
                // Store the min dist thus far
                vec3 diff = neighbor + point - pointFract;
                float dist = length(diff);
                minDist = min(minDist, dist);
            }
        }
    }
    return minDist;
}

float WorleyNoise3D1(vec3 p)
{
    // Tile the space
    vec3 pointInt = floor(p);
    vec3 pointFract = fract(p);

    float minDist = 1.0; // Minimum distance initialized to max.

    // Search all neighboring cells and this cell for their point
    for(int z = -1; z <= 1; z++)
    {
        for(int y = -1; y <= 1; y++)
        {
            for(int x = -1; x <= 1; x++)
            {
                vec3 neighbor = vec3(float(x), float(y), float(z));

                // Random point inside current neighboring cell
                vec3 point = random3(pointInt + neighbor);

                // Animate the point
                point = 0.5 + 0.5 * sin(u_Time * 0.0005 + 6.2831 * point); // 0 to 1 range

                // Compute the distance b/t the point and the fragment
                // Store the min dist thus far
                vec3 diff = neighbor + point - pointFract;
                float dist = length(diff);
                minDist = min(minDist, dist);
            }
        }
    }
    return minDist;
}

float WorleyNoise(vec2 uv)
{
    // Tile the space
    vec2 uvInt = floor(uv);
    vec2 uvFract = fract(uv);

    float minDist = 1.0; // Minimum distance initialized to max.

    // Search all neighboring cells and this cell for their point
    for(int y = -1; y <= 1; y++)
    {
        for(int x = -1; x <= 1; x++)
        {
            vec2 neighbor = vec2(float(x), float(y));

            // Random point inside current neighboring cell
            vec2 point = random2(uvInt + neighbor);

            // Animate the point
            point = 0.5 + 0.5 * sin(u_Time * 0.01 + 6.2831 * point); // 0 to 1 range

            // Compute the distance b/t the point and the fragment
            // Store the min dist thus far
            vec2 diff = neighbor + point - uvFract;
            float dist = length(diff);
            minDist = min(minDist, dist);
        }
    }
    return minDist;
}

float worleyFBM(vec3 uv) {
    float sum = 0;
    float freq = 4;
    float amp = 0.5;
    for(int i = 0; i < 8; i++) {
        sum += WorleyNoise3D(uv * freq) * amp;
        freq *= 2;
        amp *= 0.5;
    }
    return sum;
}

// Sun palette
const vec3 sun[11] = vec3[](vec3(253, 198, 153) / 255.0,
                            vec3(255, 147, 147) / 255.0,
                            vec3(255, 241, 0) / 255.0,
                            vec3(255, 235, 175) / 255.0,
                            vec3(255, 255, 204) / 255.0,
                            vec3(255, 255, 255) / 255.0,
                            vec3(255, 255, 190) / 255.0,
                            vec3(254, 192, 81) / 255.0,
                            vec3(255, 137, 103) / 255.0,
                            vec3(253, 96, 81) / 255.0,
                            vec3(57, 32, 51) / 255.0);

// Dusk palette
const vec3 dusk[11] = vec3[](vec3(255, 0, 0) / 255.0,
                             vec3(199, 0, 57) / 255.0,
                             vec3(255, 77, 0) / 255.0,
                             vec3(255, 162, 99) / 255.0,
                             vec3(240, 208, 132) / 255,
                             vec3(255, 255, 255) / 255.0,
                             vec3(255, 159, 0) / 255.0,
                             vec3(255, 69, 0) / 255.0,
                             vec3(255, 105, 98) / 255.0,
                             vec3(144, 96, 144) / 255.0,
                             vec3(72, 48, 120) / 255.0);

// Sky palette
const vec3 sky[9] = vec3[](vec3(65, 105, 225) / 255.0, // early morning dark blue
                          vec3(179, 206, 229) / 255.0, // early morning light blue
                          vec3(221, 160, 221) / 255, // sunrise pulm
                          vec3(173, 216, 230) / 255.0, // morning blue
                          vec3(135, 206, 250) / 255.0, // noon blue
                          vec3(135, 206, 235) / 255.0, // afternoon blue
                          vec3(204, 204, 255) / 255.0, // sunset purple
                          vec3(27, 3, 163) / 255.0, // evening blue
                          vec3(25, 25, 112) / 255.0); // midnight blue

// Cloud palette
const vec3 cloud[5] = vec3[](vec3(255, 250, 250) / 255.0,
                             vec3(250, 249, 246) / 255.0,
                             vec3(248, 248, 255) / 255.0,
                             vec3(245, 245, 245) / 255.0,
                             vec3(236, 240, 241) / 255.0);

// hardcode sun color at time
vec3 mapSunColor(float time) {
    if (time <= 0.13) {
        return sun[5];
    } else if (time <= 0.17) {
        return mix(sun[5], sun[6], (time - 0.13) / 0.04);
    } else if (time <= 0.21) {
        return mix(sun[6], sun[7], (time - 0.17) / 0.04);
    } else if (time <= 0.25) {
        return mix(sun[7], sun[8], (time - 0.21) / 0.04);
    } else if (time <= 0.29) {
        return mix(sun[8], sun[9], (time - 0.25) / 0.04);
    } else if (time <= 0.325) {
        return mix(sun[9], sun[10], (time - 0.29) / 0.035);
    } else if (time <= 0.345) {
        return mix(sun[10], vec3(52, 52, 52) / 255.0, (time - 0.325) / 0.02);
    } else if (time <= 0.7) {
        return vec3(52, 52, 52) / 255.0;
    } else if (time <= 0.73) {
        return mix(vec3(52, 52, 52) / 255.0, sun[0], (time - 0.7) / 0.03);
    } else if (time <= 0.775) {
        return mix(sun[0], sun[1], (time - 0.73) / 0.045);
    } else if (time <= 0.8025) {
        return mix(sun[1], sun[2], (time - 0.775) / 0.0275);
    } else if (time <= 0.83) {
        return mix(sun[2], sun[3], (time - 0.8025) / 0.0275);
    } else if (time <= 0.86) {
        return mix(sun[3], sun[4], (time - 0.83) / 0.03);
    } else if (time <= 0.89) {
        return mix(sun[4], sun[5], (time - 0.86) / 0.03);
    } else {
        return sun[5];
    }
}

// hardcode dusk color at time
vec3 mapDuskColor(float time) {
    if (time <= 0.13) {
        return dusk[5];
    } else if (time <= 0.17) {
        return mix(dusk[5], dusk[6], (time - 0.13) / 0.04);
    } else if (time <= 0.21) {
        return mix(dusk[6], dusk[7], (time - 0.17) / 0.04);
    } else if (time <= 0.25) {
        return mix(dusk[7], dusk[8], (time - 0.21) / 0.04);
    } else if (time <= 0.29) {
        return mix(dusk[8], dusk[9], (time - 0.25) / 0.04);
    } else if (time <= 0.325) {
        return mix(dusk[9], dusk[10], (time - 0.29) / 0.035);
    } else if (time <= 0.345) {
        return mix(dusk[10], vec3(52, 52, 52) / 255.0, (time - 0.325) / 0.02);
    } else if (time <= 0.7) {
        return vec3(52, 52, 52) / 255.0;
    } else if (time <= 0.74) {
        return mix(vec3(52, 52, 52) / 255.0, dusk[0], (time - 0.7) / 0.04);
    } else if (time <= 0.775) {
        return mix(dusk[0], dusk[1], (time - 0.74) / 0.035);
    } else if (time <= 0.8025) {
        return mix(dusk[1], dusk[2], (time - 0.775) / 0.0275);
    } else if (time <= 0.83) {
        return mix(dusk[2], dusk[3], (time - 0.8025) / 0.0275);
    } else if (time <= 0.86) {
        return mix(dusk[3], dusk[4], (time - 0.83) / 0.03);
    } else if (time <= 0.89) {
        return mix(dusk[4], dusk[5], (time - 0.86) / 0.03);
    } else {
        return dusk[5];
    }
}

// hardcode sky color at time
vec3 mapSkyColor(float time) {
    if (time <= 0.15) {
        return sky[4];
    } else if (time <= 0.185) {
        return mix(sky[4], sky[5], (time - 0.15) / 0.035);
    } else if (time <= 0.24) {
        return mix(sky[5], sky[6], (time - 0.185) / 0.055);
    } else if (time <= 0.29) {
        return mix(sky[6], sky[7], (time - 0.24) / 0.05);
    } else if (time <= 0.34) {
        return mix(sky[7], sky[8], (time - 0.29) / 0.05);
    } else if (time <= 0.67) {
        return sky[8];
    } else if (time <= 0.705) {
        return mix(sky[8], sky[0], (time - 0.67) / 0.035);
    } else if (time <= 0.74) {
        return mix(sky[0], sky[1], (time - 0.705) / 0.035);
    } else if (time <= 0.775) {
        return mix(sky[1], sky[2], (time - 0.74) / 0.035);
    } else if (time <= 0.815) {
        return mix(sky[2], sky[3], (time - 0.775) / 0.04);
    } else if (time <= 0.855) {
        return mix(sky[3], sky[4], (time - 0.815) / 0.04);
    } else {
        return sky[4];
    }
}

vec3 mapCloudColor(float t) {
    if (t < 0.4) {
        return cloud[4];
    } else if (t < 0.55) {
        return mix(cloud[4], cloud[0], (t - 0.4) / 0.15);
    } else if (t < 0.7) {
        return mix(cloud[4], cloud[1], (t - 0.55) / 0.15);
    } else if (t < 0.85) {
        return mix(cloud[4], cloud[2], (t - 0.7) / 0.15);
    } else {
        return mix(cloud[4], cloud[3], (t - 0.85) / 0.15);
    }
}

// get rotating sun location
vec3 sunLocation(float time) {
    vec3 p = vec3(0.0, 5000.0, 0.0);
    float rot = TWO_PI * time;
    vec3 curLocation = vec3(cos(rot) * p.x - sin(rot) * p.y,
                            sin(rot) * p.x + cos(rot) * p.y,
                            p.z);
    return curLocation;
}

// get rotating moon location
vec3 moonLocation(float time) {
    vec3 p = vec3(0.0, -5000.0, 0.0);
    float rot = -TWO_PI * time;
    vec3 curLocation = vec3(p.x, cos(rot) * p.y - sin(rot) * p.z,
                                 sin(rot) * p.y + cos(rot) * p.z);
    return curLocation;
}

// get changing sun corona size
float getCoronaSize(float time) {
    if (time <= 0.1) {
        return 10.0;
    } else if (time <= 0.35) {
        return 10.0 + smoothstep(0.1, 0.35, time) * 70.0;
    } else if (time <= 0.65) {
        return 80.0;
    } else if (time <= 0.9) {
        return 80.0 - smoothstep(0.65, 0.9, time) * 70.0;
    } else {
        return 10.0;
    }
}

// blend moonColor with skyColor so that moon only appears after sun goes down
vec3 getMoonGradual(float time, vec3 skyColor) {
    if (time <= 0.27) {
        return skyColor;
    } else if (time <= 0.3) {
        return mix(skyColor, moonColor, smoothstep(0.27, 0.3, time));
    } else if (time <= 0.7) {
        return moonColor;
    } else if (time <= 0.73) {
        return mix(moonColor, skyColor, smoothstep(0.7, 0.73, time));
    } else {
        return skyColor;
    }
}

///test

// void pR(inout vec2 p,float a)
// {
//     p=cos(a)*p+sin(a)*vec2(p.y,-p.x);
// }

// float sdSphere( vec3 p, float s )
// {
//     return length(p)-s;
// }


// 3D voronoi https://www.shadertoy.com/view/ldl3Dl
//vec3 hash( vec3 x )
//{
//	return textureLod( iChannel0, (x.xy+vec2(3.0,1.0)*x.z+0.5)/64.0, 0. ).xyz;
//}

// vec3 hash( vec3 x )
// {
//         x = vec3( dot(x,vec3(127.1,311.7, 74.7)),
//                           dot(x,vec3(269.5,183.3,246.1)),
//                           dot(x,vec3(113.5,271.9,124.6)));

//         return fract(sin(x)*43758.5453123);
// }

// vec3 voronoi( in vec3 x )
// {
//     vec3 p = floor( x );
//     vec3 f = fract( x );

//         //float id = 0.0;
//     vec2 res = vec2( 100.0 );
//     for( int k=-1; k<=1; k++ )
//     for( int j=-1; j<=1; j++ )
//     for( int i=-1; i<=1; i++ )
//     {
//         vec3 b = vec3( float(i), float(j), float(k) );
//         vec3 r = vec3( b ) - f + hash( p + b );
//         float d = dot( r, r );

//         if( d < res.x )
//         {
//                         //id = dot( p+b, vec3(1.0,57.0,113.0 ) );
//             res = vec2( d, res.x );
//         }
//         else if( d < res.y )
//         {
//             res.y = d;
//         }
//     }

//     return vec3(  res , 0.0 );//abs(id)
// }


// float sky(vec3 point)
// {
//     //vec3 p2;
//     //const float cldspacing = 80.;
//     //vec2 clsftuv = vec2(ceil(point.x/cldspacing)/64.,ceil(point.y/cldspacing)/64.);
//     //vec2 clsft = vec2(texture(iChannel0,clsftuv).r,texture(iChannel0,clsftuv.yx).r);
//     //p2.xz = mod(point.xz, cldspacing);
//     //p2.y = point.y - 20.;
//         //float d = sdSphere(p2 - vec3(cldspacing/2.,0.,cldspacing/2.),15.0);


//     //vec3 vrn = voronoi(point * 0.25);
//     vec3 vrn2 = voronoi(point * 0.02 + vec3(u_Time * 0.05, u_Time * 0.1,0.));
//     //float v3 =  voronoi(point * 0.6).x;
//     float v2 = 0.6 - vrn2.x * (1.5 - vrn2.y );
//     float v =  voronoi(point * 0.25).x;// vrn.x;// * (1.4 - vrn.y);
//     //v2 = texture(iChannel0, point.xz * 0.001 + vec2(iTime * 0.001)).r;
//     float cld = max(8. - v2 * 9.8,0.);
//     float d = sdSphere(vec3(cld,point.y - 26., cld),6.0);
//         v = -0.3 + v* 2.1 ;
//     d += v;// + v3 * 1.4;

//     return d;
// }

void main() {
    // Simulate a day in mini-minecraft in one hour in real life
    // Time represents the current time in minecraft
    float time = mod(u_TimeElapsed, 216000.0) / 216000;

    // Calculate eye ray direction
    vec2 ndc = (gl_FragCoord.xy / vec2(u_Dimensions)) * 2.0 - 1.0; // -1 to 1 NDC
    vec4 p = vec4(ndc.xy, 1, 1); // Pixel at the far clip plane
    p *= 10000.0; // Times far clip plane value
    p = u_ViewProj * p; // Convert from unhomogenized screen to world
    vec3 rayDir = normalize(p.xyz - u_Eye); // Eye ray direction to pixel

    // Simulate a moving sun with changing color
    vec3 sunDir = normalize(sunLocation(time) - u_Eye); // Eye ray direction to sun
    float sunCoreSize = 1.5;
    float sunSize = 2;
    float coronaSize = getCoronaSize(time);

    //Simulate a moving moon
    vec3 moonDir = normalize(moonLocation(time) - u_Eye);  // Eye ray direction to moon
    float moonSize = 2.5;
    float moonEdge = 5;

    // Get the current sun, dusk, sky, and moon color
    vec3 sunColor = mapSunColor(time);
    vec3 duskColor = mapDuskColor(time);
    vec3 skyColor = mapSkyColor(time);
    vec3 moonGradual = getMoonGradual(time, skyColor);

    // Calculate viewing angle between sun and current pixel
    float sunAngle = acos(dot(rayDir, sunDir)) * 360.0 / PI;
    // Calculate viewing angle between moon and current pixel
    float moonAngle = acos(dot(rayDir, moonDir)) * 360.0 / PI;

    // If the angle between our ray dir and vector to center of sun
    // is less than the threshold, then we're looking at the sun
    if (sunAngle <= sunSize) {
        // Full center of sun
        if(sunAngle <= sunCoreSize) {
            outColor = vec4(sunColor, 1.0);
        }
        // Edge of the sun, mix sunColor with duskColor
        else {
            outColor = vec4(mix(sunColor, duskColor, smoothstep(sunCoreSize, sunSize, sunAngle)), 1.0);
        }
    }
    // If the angle between our ray dir and vector to center of moon
    // is less than the threshold, then we're looking at the moon
    else if (moonAngle <= moonSize) {
        // Full center of moon
        outColor = vec4(moonGradual, 1.0);
    }
    // Otherwise our ray is looking into the sky
    else {
        // If the angle is less than threshold, then we are looking at
        // the corona of sun with changing corona size, mix duskColor and skyColor
        if (sunAngle <= coronaSize) {
            skyColor = mix(duskColor, skyColor, smoothstep(sunSize, coronaSize, sunAngle));
        }
        // If the angle is less than threshold, then we are looking at
        // the edge of the moon, mix moonColor and skyColor
        if (moonAngle <= moonEdge) {
            skyColor = mix(moonGradual, skyColor, smoothstep(moonSize, moonEdge, moonAngle));
        }
        // If the time is within threshold, generate stars
        if (time >= 0.25 && time <= 0.70) {
            // Use worley noise to generate random distance to centers in [0, 1]
            float t = WorleyNoise3D(rayDir * 35);
            // If random distance is bigger then threshold, then just skyColor
            if (t >= 0.1) {
                outColor = vec4(skyColor, 1.0);
            // When stars just start to show, blend skyColor into starColor
            } else if (time <= 0.26) {
                outColor = vec4(mix(skyColor, starColor, (time - 0.25) / 0.01), 1.0);
            // otherwise just starColor
            } else if (time <= 0.69) {
                outColor = vec4(starColor, 1.0);
            // when stars about to disappear, blend starColor into skyColor
            } else {
                outColor = vec4(mix(starColor, skyColor, (time - 0.69) / 0.01), 1.0);
            }
        // If not within threshold, then just skyColor
        } else {
            outColor = vec4(skyColor, 1.0);
        }

        // Generate cloud
        // float t = WorleyNoise3D1(rayDir * 3);
        // if (t < 0.3) {

            // vec2 offset = vec2(worleyFBM(rayDir));
            // offset = offset * 2.0 - vec2(1.0);
            // vec3 cloudColor = mapCloudColor(t / 0.3 + offset.y * 0.2);
            // if (t > 0.15) {
            //     outColor = vec4(mix(cloudColor, skyColor, t / 0.3), 1.0);
            // } else {
            //     outColor = vec4(cloudColor, 1.0);
            // }
        // }
    }


// #define SUNSET_THRESHOLD 0.75
// #define DUSK_THRESHOLD -0.1
//         // if(raySunDot > SUNSET_THRESHOLD) {
//         //     // Do nothing, sky is already correct color
//         // }
//         // // Any dot product between 0.75 and -0.1 is a LERP b/t sunset and dusk color
//         // else if(raySunDot > DUSK_THRESHOLD) {
//         //     float t = (raySunDot - SUNSET_THRESHOLD) / (DUSK_THRESHOLD - SUNSET_THRESHOLD);
//         //     outColor = vec4(mix(outColor.xyz, duskColor, t), 1.f);
//         // }
//         // // Any dot product <= -0.1 are pure dusk color
//         // else {
//         //     outColor = vec4(duskColor, 1.f);
//         // }
}
