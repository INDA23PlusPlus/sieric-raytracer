#version 460

uniform float ratio;
in vec2 npos;
out vec4 color;

const vec3 cam_pos = vec3(0., 0., -2500.);
const float max_dist = 50000.;
const float min_dist = .5;

struct Sphere {
    vec3 pos;
    float r;
    vec4 col;
};

const Sphere spheres[2] = Sphere[2](
Sphere(vec3(0., 0., 2500.), 100., vec4(1., 0., 1., 1.)),
Sphere(vec3(0., 100., 2550.), 150., vec4(1., 0., 0., 1.))
);

float sphere_dist(Sphere sphere, vec3 pos) {
    return distance(pos, sphere.pos) - sphere.r;
}

int closest;
float dfs(vec3 pos) {
    float m = max_dist;
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
    vec3 p = ratio > 1
        ? 500*vec3(npos.x*ratio, npos.y, 0.)
        : 500*vec3(npos.x, npos.y/ratio, 0.);

    vec3 d = normalize(p - cam_pos);

    float D, min_D = max_dist;
    while((D = dfs(p)) < max_dist && D >= min_dist) {
        min_D = min(D, min_D);
        p += D*d;
    }

    vec4 sphere_col = spheres[closest].col;
    const vec4 sky_col = vec4(.3, .8, .8, 1.);

    color = D < min_dist ? sphere_col : sky_col;
}
