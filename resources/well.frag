uniform vec4 center;
uniform vec2 size;
uniform vec4 color;

void main() {
    float diff = 1100.0 / size.y;
    float distance = distance(gl_FragCoord.xy, center.xy);

    if (distance > diff*center.w)
    	gl_FragColor = mix(gl_Color, color, 1.0 - (distance / (diff * center.z)));
    else
    	gl_FragColor = gl_Color;
}