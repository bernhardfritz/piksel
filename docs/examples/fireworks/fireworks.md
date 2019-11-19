# Fireworks

[fireworks](_media/fireworks.html ':include width=512px height=512px scrolling=no')

<div id="el">
    <div class="tabs">
        <template v-for="file in files">
            <input name="tabs" type="radio" v-bind:id="'tab-' + (files.indexOf(file) + 1)" checked="checked" class="input"/>
            <label v-bind:for="'tab-' + (files.indexOf(file) + 1)" class="label">{{ file.name }}</label>
            <div class="panel">
                <pre data-lang="cpp"><code class="lang-cpp" v-html="highlight(file.content)"></code></pre>
            </div>
        </template>
    </div>
</div>

<script>
new Vue({
    el: '#el',
    data: {
        files: [{
            name: 'app.cpp',
            content: `#include "app.hpp"
#include "firework.hpp"
#include <piksel/ext/rng.hpp>

std::vector<Firework> fireworks;
glm::vec2 gravity;

void App::setup() {
    gravity = glm::vec2(0.0f, 0.2f);
}

void App::draw(piksel::Graphics& g) {
    g.background(glm::vec4(0.0f, 0.0f, 0.0f, 0.1f));
    piksel::Rng& rng = piksel::Rng::getInstance();
    if (rng.random(0, 1) < 0.1f) {
        Firework firework(glm::vec2(rng.random(0, width), height));
        fireworks.push_back(firework);
    }
    for (Firework& firework : fireworks) {
        firework.applyForce(gravity);
        firework.update();
        firework.draw(g);
    }
    std::vector<Firework>::iterator it = fireworks.begin();
    while (it != fireworks.end()) {
        if ((*it).isDone()) {
            it = fireworks.erase(it);
        } else {
            it++;
        }
    }
}`
        }, {
            name: 'app.hpp',
            content: `#ifndef APP_HPP
#define APP_HPP

#include <piksel/baseapp.hpp>

class App : public piksel::BaseApp {
public:
    App() : BaseApp(512, 512) {}
    void setup();
    void draw(piksel::Graphics& g);
};

#endif /* APP_HPP */`
        }, {
            name: 'firework.cpp',
            content: `#include "firework.hpp"
#include <piksel/ext/rng.hpp>

Firework::Firework(glm::vec2 position) {
    rocket.position = position;
    rocket.velocity = glm::vec2(0.0f,
            piksel::Rng::getInstance().random(-10.0f, -12.0f));
    rocket.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    exploded = false;
}

void Firework::applyForce(glm::vec2 force) {
    if (!exploded) {
        rocket.applyForce(force);
    } else {
        for (Particle& particle : particles) {
            particle.applyForce(force);
        }
    }
}

void Firework::update() {
    if (!exploded) {
        rocket.update();
        if (rocket.velocity.y >= 0.0f) {
            explode();
        }
    } else {
        for (Particle& particle : particles) {
            particle.velocity *= 0.95;
            particle.update();
        }
        std::vector<Particle>::iterator it = particles.begin();
        while(it != particles.end()) {
            if ((*it).isDead()) {
                it = particles.erase(it);
            } else {
                it++;
            }
        }
    }
}

void Firework::explode() {
    piksel::Rng& rng = piksel::Rng::getInstance();
    glm::vec4 color = glm::vec4(rng.random(0.0f, 1.0f),
            rng.random(0.0f, 1.0f), rng.random(0.0f, 1.0f), 1.0f);
    for (int i = 0; i < 200; i++) {
        Particle particle;
        particle.position = rocket.position;
        particle.mass = rng.random(0.7, 1.0);
        float angle = rng.random(0.0, piksel::TWO_PI);
        float magnitude = rng.random(2.0f, 10.0f);
        particle.velocity = glm::vec2(magnitude * cos(angle),
                magnitude * sin(angle));
        particle.lifespan = rng.random(90.0f, 110.0f);
        particle.color = color;
        particles.push_back(particle);
    }
    exploded = true;
}

void Firework::draw(piksel::Graphics& g) {
    g.noStroke();
    if (!exploded) {
        rocket.draw(g);
    } else {
        for (Particle& particle : particles) {
            particle.draw(g);
        }
    }
}

bool Firework::isDone() {
    return exploded && particles.empty();
}`
        }, {
            name: 'firework.hpp',
            content: `#ifndef FIREWORK_HPP
#define FIREWORK_HPP

#include "particle.hpp"
#include <piksel/graphics.hpp>
#include <glm/glm.hpp>
#include <vector>

class Firework {
public:
    Particle rocket;
    bool exploded;
    std::vector<Particle> particles;

    Firework(glm::vec2 position);

    void applyForce(glm::vec2 force);
    void update();
    void explode();
    void draw(piksel::Graphics& g);
    bool isDone();
};

#endif /* FIREWORK_HPP */`
        }, {
            name: 'main.cpp',
            content: `#include "app.hpp"

int main() {
    App app;
    app.start();
}`
        }, {
            name: 'particle.cpp',
            content: `#include "particle.hpp"

void Particle::applyForce(glm::vec2 force) {
    acceleration += force * mass;
}

void Particle::update() {
    velocity += acceleration;
    position += velocity;
    acceleration = glm::vec2(0.0f);
    if (age < lifespan) {
        age++;
    }
}

void Particle::draw(piksel::Graphics& g) {
    if (lifespan == -1) {
        g.fill(color);
    } else {
        float pct = 1.0f - age / (float) lifespan;
        g.fill(glm::vec4(color.r, color.g, color.b, pct));
    }
    g.ellipse(position.x, position.y, mass * 8.0f, mass * 8.0f);
}

bool Particle::isDead() {
    return age == lifespan;
}`
        }, {
            name: 'particle.hpp',
            content: `#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <piksel/graphics.hpp>
#include <glm/glm.hpp>

class Particle {
public:
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;
    int age;
    int lifespan;
    float mass;
    glm::vec4 color;

    Particle()
        : position(glm::vec2(0.0f))
        , velocity(glm::vec2(0.0f))
        , acceleration(glm::vec2(0.0f))
        , age(0)
        , lifespan(-1)
        , mass(1.0f)
        , color(glm::vec4(1.0f)) {
    }

    void applyForce(glm::vec2 force);
    void update();
    void draw(piksel::Graphics& g);
    bool isDead();
};

#endif /* PARTICLE_HPP */`
        }]
    },
    methods: {
        highlight: function(code) {
            return Prism.highlight(code, Prism.languages.cpp, 'cpp');
        }
    }
});
</script>
