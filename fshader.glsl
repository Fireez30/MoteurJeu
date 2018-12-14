#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif


uniform sampler2D texture;

uniform vec3 playerpos;
uniform float radius;

//attribute vec4 a_position;
varying vec2 v_texcoord;
uniform int test;
//! [0]
void main()
{
    vec4 base = texture2D(texture, v_texcoord);
    if(base.a < 0.5){ discard;}
    if (test < 1)
    {
    //vec3 play = mvp_matrix * positionjoueur;
    //vec2 result = playerpos.xy - gl_FragCoord.xy;
    //base.x = gl_FragCoord.x;
    //base.y = gl_FragCoord.y;
    //float result2 = dot(result, result);
    //float dis = inversesqrt(result2);
    //float dis = 0.15;
    //if (dis < 0.000001){dis = 0.1;}
    // base.x *= dis;
     //base.y *= dis;
     //base.z *= dis;
     base.x *= 0.15;
     base.y *= 0.15;
     base.z *= 0.15;
     //vec2 pixel=base.xy;
     //pixel.y=screenHeight-pixel.y;
     //vec2 aux=lightpos-pixel;
     //float distance=length(aux);
     //float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);
     //vec4 color=vec4(attenuation,attenuation,attenuation,1.0)*vec4(lightColor,1.0);
     gl_FragColor = base;//*texture2D(texture,gl_TexCoord[0].st);
     //gl_FragColor = color;
    }
    else {
        gl_FragColor = base;
    }
    // Set fragment color from texture
    //gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);

    //gl_FragColor = color;
}
//! [0]

