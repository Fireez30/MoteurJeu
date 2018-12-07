#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

uniform vec2 lightpos;
uniform vec3 lightColor;
uniform float screenHeight;
uniform vec3 lightAttenuation;
uniform float aradius;

varying vec2 v_texcoord;
uniform int test;
//! [0]
void main()
{
    vec4 base = texture2D(texture, v_texcoord);
    if(base.a < 0.5){ discard;}
    if (test == 0)
    {
    //vec3 play = mvp_matrix * positionjoueur;
    //vec2 result = gl_Position.xy - play.xy;
    //float result2 = dot(result, result);
    //float dis = inversesqrt(result2);
    //float dis = 0.15;
    //if (dis == 0)
     //   dis = 0.1;
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

