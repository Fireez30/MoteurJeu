#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform vec4 objectwcoord;
uniform sampler2D texture;
layout(origin_upper_left) varying vec4 gl_FragCoord;

//attribute vec4 a_position;
varying vec2 v_texcoord;


void main()
{
    vec4 base = texture2D(texture, v_texcoord);
    if(base.a < 0.5){ discard;}

    gl_FragColor = base;
}
//! [0]

