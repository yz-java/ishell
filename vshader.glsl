#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif
attribute vec4 position;
attribute vec2 textureIndex;
varying vec2 v_texcoord;
void main(void)
{
    gl_Position = position;
    v_texcoord = textureIndex;
}
