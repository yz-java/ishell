#ifdef GL_ES
precision mediump float;
#endif
varying vec2 v_texcoord;
uniform sampler2D inputTexture;

void main()
{
    vec4 rgbaColor = texture2D(inputTexture,v_texcoord);
    rgbaColor.a = 1.0;
    gl_FragColor = rgbaColor;
}
