uniform vec4 center;
uniform float sizeDiff;
uniform vec4 color;

void main() {
    float distance = distance(gl_FragCoord.xy, center.xy);

    if (distance > sizeDiff * center.w)
    	gl_FragColor = mix(gl_Color, color, 1.0 - (distance / (sizeDiff * center.z)));
    else
    	gl_FragColor = gl_Color;
}