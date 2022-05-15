precision mediump float;

uniform vec2 resolution;
uniform float time;
uniform float seed;

vec3 generate_gray_modifier(vec2 cell, float x_scale, float y_scale, float mod_factor) {
    float someGray = mod(x_scale * cell.x + y_scale * cell.y, mod_factor) / mod_factor;
    return vec3(someGray);
}

vec3 generate_color_modifier(vec2 cell, float x_scale, float y_scale, float red_mod, float green_mod, float blue_mod, float mod_factor) {
    float someRed = mod(x_scale * cell.x + y_scale * cell.y, red_mod) / mod_factor;
    float someGreen= mod(x_scale * cell.x + y_scale * cell.y, green_mod) / mod_factor;
    float someBlue = mod(x_scale * cell.x + y_scale * cell.y, blue_mod) / mod_factor;
    return vec3(someRed, someGreen, someBlue);
}

void main() {
    vec2 coord = gl_FragCoord.xy / resolution;

    // 0. Draws a crazy rainbox. Every x coord is the r value, every y coord is the g value, and the blue coord changes with time over a poppin' sine function
    // Output RGB color in range from 0.0 to 1.0
    vec3 color = vec3(coord.x, coord.y, abs(sin(time)));

    // 1. Uncomment these lines to draw black and white triangles which move
    vec2 screenCoord = gl_FragCoord.xy / resolution.y ;
    // Here's how increasing this factor works.
    // By increasing this factor, pixels which are closer together will sample colors further apart.
    // Hence, colors will start to wrap sooner
    vec2 scaledScreenCoord = 10.0 * screenCoord; 
    vec2 movingScreenCoord = scaledScreenCoord + vec2(-time, time);
    vec2 squareCoord = movingScreenCoord;
    vec2 loc = fract(squareCoord);
    // By increasing this sharpness value, we make the bounds of the smooth step function closer together and closer to zero.
    // This means that values are more likely to snap to either 0 or 1 (black or white). If the sharpness is lower (allowing for
    // a wider range of values to smoothstep between), then more values won't snap to black or white and will instead be smoothly
    // interpolated between black and white (e.g. gray)
    float sharpness = 100000000.0; 
    color = vec3(smoothstep(-1.0/sharpness, 1.0/sharpness, loc.y - loc.x));

    // 2. Uncomment these lines to invert some of the triangles
    vec2 cell = squareCoord - loc; // the whole number value of any given square on the rendered grid

    float og_mod = 5.0;

    float seed_step = floor(seed * 9.0); // Transform the tenths place into an int value [0,9]
    float seeded_mod = 1.0 + seed_step; // seed will set the seeded mod to a value between [1.0,10.0)
    // Task 2. select og_mod for the modulo provided in the original project 
    //         select seeded_mod for task2 modulo which controls the shape of the pattern by the seed
    //float pattern_mod = og_mod;
    float pattern_mod = seeded_mod;
    if (mod(2.0 * cell.x + cell.y, pattern_mod) == 1.0) { // cells whos x,y sum, doubled and modulo 5 == 1
        color = 1.0 - color;               // N.B. edit: changed == 1.0 to 0.0 because it's arbitrary which mod we hit. it's just 1-in-5
    }

    // 3. Uncomment these lines to produce interesting colors
    // O.g. some gray
    // float someGray = mod(3.0 * cell.x + 2.0 * cell.y, 7.0) / 7.0; // ok I don't totally understand the left side of the mod, but doing
    //                                                                   // x % 7 gives us back a value in range [0,7)
    //                                                                   // dividing by 7  gives us a value in range [0.0,1.0)
    //                                                                   // the left side of the mod just adds some interesting pattern that semis structured yet kinda random
    // vec3 grayModifier = vec3(someGray);

    vec3 grayModifier = generate_gray_modifier(cell, 3.0, 2.0, 7.0);
    vec3 rainbowModifier = generate_color_modifier(cell, 3.0, 2.0, 7.0, 6.0, 5.0, 7.0);

    // Task 1. select one gray for original project's gray color
    //         select rainbow for my modified one
    vec3 darkColorModifier = rainbowModifier;
    // vec3 darkColorModifier = grayModifier;

    vec3 invertedTriangleMask = 1.0 - color; // all black becomes white and vice versa
    vec3 darkColorOnlyOnSomeTriangles = invertedTriangleMask * darkColorModifier;
    // o.g. 1.0 - (1.0 - color) * gray;
    // o.g. 1.0 - gray - color * gray;
    //color = 1.0 - (flipped_color * grayModifier);
    //color = color * grayModifier;
    //color = flipped_grays;
    // color = 1.0 - (1.0 - color) * grayModifier;
    vec3 lightColorOnlyOnSomeTriangles = 1.0 - darkColorOnlyOnSomeTriangles; // triangles without color are white. triangles with color are light colors.
    color = lightColorOnlyOnSomeTriangles;

    // 4. Uncomment to lighten the colors
    color = sqrt(color);// each value of color is 0.0->1.0 (i.e. a fraction) 
                        // the square root of a fraction is a larger fraction (still less than 1)
                        // larger values are lighter values.

    gl_FragColor = vec4(color, 1.0);
}
