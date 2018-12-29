#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
layout(origin_upper_left) varying vec4 gl_FragCoord;

//attribute vec4 a_position;
varying vec2 v_texcoord;
uniform vec4 playerpos;
uniform mat4 mvpmatrix;
uniform float radius;
uniform vec4 objectwcoord;
uniform int test;
//! [0]

uniform int numLights;
uniform struct Light {
   vec4 position;
   vec3 color; //a.k.a the color of the light
   float attenuation;
   float ambientCoefficient;
   float coneAngle;
   float maxAngle;
   float dist;
   float maxDist;
   vec3 coneDirection;
} allLights[10];

vec3 ApplyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos) {
    vec3 surfaceToLight = normalize(light.position.xyz - surfacePos);
    float distanceToLight = length(light.position.xyz - surfacePos);
    if(distanceToLight < light.maxDist){
        float facteur = 1.0f;
        if(light.dist < distanceToLight){
            float diff = distanceToLight - light.dist;
            facteur = 1.0f - diff/(light.maxDist - light.dist);
        }
        float attenuation = 1.0 / (1.0 + light.attenuation * distanceToLight * distanceToLight);
        float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.coneDirection))));
        float facteur2 = 1.0f;
        if(lightToSurfaceAngle > light.coneAngle){
            float diff = lightToSurfaceAngle - light.coneAngle;
            facteur2 = 1.0f - diff/(light.maxAngle - light.coneAngle);
        }
        if(lightToSurfaceAngle > light.maxAngle){
            return vec3(0.0,0.0,0.0);
        }
        vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.color;
        return (ambient+ attenuation) * facteur * facteur2;
    }
    return vec3(0.0,0.0,0.0);
}

void main()
{
    vec4 base = texture2D(texture, v_texcoord);
    if(base.a < 0.5){ discard;}
    if (test < 1)
    {
        /*float vectdiff = length(gl_FragCoord-playerpos);
        float facteur=1.0-vectdiff/125.0;
        if(facteur > 1.0) facteur = 1.0;
        else if(facteur < 0.15) facteur = 0.15;*/
        vec3 linearColor = vec3(0);
        for(int i = 0; i < numLights; ++i){
            linearColor += ApplyLight(allLights[i], vec3(0.15f,0.15f,0.15f), vec3(0,0,1), vec3(gl_FragCoord.x,gl_FragCoord.y,gl_FragCoord.z));
        }
        if(linearColor.x < 0.15) linearColor.x =0.15;
        if(linearColor.y < 0.15) linearColor.y =0.15;
        if(linearColor.z < 0.15) linearColor.z =0.15;
        if(linearColor.x > 1.0) linearColor.x =1.0;
        if(linearColor.y > 1.0) linearColor.y =1.0;
        if(linearColor.z > 1.0) linearColor.z =1.0;
        gl_FragColor = vec4(linearColor.x*base.x,linearColor.y*base.y,linearColor.z*base.z,1.0);
    }
    else
        gl_FragColor = base;
}
//! [0]

