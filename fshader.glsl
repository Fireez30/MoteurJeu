#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

varying vec2 v_texcoord;
varying vec4 color;

//! [0]
void main()
{
    vec4 base = texture2D(texture, v_texcoord);
    if(base.a < 0.5){ discard;}
    gl_FragColor = base;
    // Set fragment color from texture
    //gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);

    //gl_FragColor = color;
}
//! [0]

