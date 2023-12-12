#version 460

uniform float ratio;
in vec2 npos;
out vec4 color;

uniform mat4 affine;

#define NEAR_CLIP 1000.
#define MIN_DIST .5
#define GLOW_DIST 50.
#define MAX_MARCH 50

#define ENABLE_GLOW
#define ENABLE_LOOP

struct Sphere {
    vec3 pos;
    float r;
    vec4 col;
};

vec3 mod2(vec3 x, float y) {
#ifdef ENABLE_LOOP
    return mod(x, y) - y/2;
#else
    return x;
#endif
}

const Sphere spheres[2] = Sphere[2](
Sphere(vec3(100., 150., 200.), 100., vec4(1., 0., 1., 1.)),
Sphere(vec3(200., 200., 250.), 150., vec4(1., 0., 0., 1.))
);

float sphere_dist(Sphere sphere, vec3 pos) {
    pos -= sphere.pos;
    return length(mod2(pos, 2000.)) - sphere.r;
}

int closest;
float dfs(vec3 pos) {
    float m = 1.0 / 0.0;
    for(int i = 0; i < 2; ++i) {
        float d = sphere_dist(spheres[i], pos);
        if(d < m) {
            m = d;
            closest = i;
        }
    }

    return m;
}

void main() {
    vec4 p = ratio > 1
        ? vec4(npos.x*ratio, npos.y, NEAR_CLIP, 1.)
        : vec4(npos.x, npos.y/ratio, NEAR_CLIP, 1.);
    p.xy *= 500;

    p = affine*p;
    p /= p.w;
    vec4 cam = affine[3];
    cam /= cam.w;

    vec4 d = vec4(normalize(p.xyz - cam.xyz), 1.);

    float D, min_D = 1.0 / 0.0;
    for(int i = 0; i < MAX_MARCH; ++i) {
        D = dfs(cam.xyz);
        min_D = min(D, min_D);
        if(D <= MIN_DIST) break;
        cam.xyz += D*d.xyz;
    }

    vec4 sphere_col = spheres[closest].col;
    const vec4 sky_col = vec4(.3, .8, .8, 1.);

    color = D < MIN_DIST ? sphere_col : sky_col;
#ifdef ENABLE_GLOW
    if(D > MIN_DIST && min_D < GLOW_DIST) {
        float blend = 1 - min_D/GLOW_DIST;
        color = color*(1-blend) + vec4(1., 1., 1., 1.)*blend;
    }
#endif
}
