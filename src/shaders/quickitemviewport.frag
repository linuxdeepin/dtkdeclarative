varying highp vec2 qt_TexCoord;

uniform lowp float opacity;
uniform sampler2D qt_Texture;
uniform sampler2D mask;
uniform vec2 maskScale;
uniform vec2 maskOffset;
uniform vec2 sourceScale;

void main()
{
    vec2 scale = maskScale * sourceScale;
    // mask材质右下角的偏移量
    vec2 maskOffset2 = maskOffset + (1.0 / sourceScale) - 1.0;
    // 左上角的mask材质坐标
    vec2 tex_top_left = (qt_TexCoord - maskOffset) * scale;
    // 左下角的mask材质坐标，由于mask本身为左上角，因此还需要将材质按垂直方向翻转
    vec2 tex_bottom_left = (qt_TexCoord - vec2(maskOffset.s, maskOffset2.t)) * scale - vec2(0, scale.t - 1.0);
    tex_bottom_left.t = 1.0 - tex_bottom_left.t; // 将材质按垂直方向翻转
    // 右上角的mask材质坐标，由于mask本身为左上角，因此还需要将材质按水平方向翻转
    vec2 tex_top_right = (qt_TexCoord - vec2(maskOffset2.s, maskOffset.t)) * scale - vec2(scale.s - 1.0, 0);
    tex_top_right.s = 1.0 - tex_top_right.s; // 将材质按水平方向翻转
    // 右下角的mask材质坐标, 由于mask本身为左上角，因此还需要将材质翻转
    vec2 tex_bottom_right = 1.0 - ((qt_TexCoord - maskOffset2 - 1.0) * scale + 1.0);

    // 从mask材质中取出对应坐标的颜色
    vec4 mask_top_left = texture2D(mask, tex_top_left);
    vec4 mask_bottom_left = texture2D(mask, tex_bottom_left);
    vec4 mask_top_right = texture2D(mask, tex_top_right);
    vec4 mask_bottom_right = texture2D(mask, tex_bottom_right);

    vec4 tex = texture2D(qt_Texture, qt_TexCoord);

    // 统一计算此像素点被模板遮盖后的颜色，此处不需要区分点是否在某个区域，不在此区域时取出的mask颜色的alpha值必为1
    tex *= mask_top_left * mask_bottom_left * mask_top_right * mask_bottom_right;
    gl_FragColor = tex * opacity;
}