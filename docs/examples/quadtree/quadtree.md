# Quadtree

[quadtree](_media/quadtree.html ':include width=512px height=512px scrolling=no')

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
            name: 'aabb.cpp',
            content: `#include "aabb.hpp"
#include <piksel/drawmode.hpp>

bool AABB::containsPoint(XY& point) {
    return abs(center.x - point.x) <= halfDimension
        && abs(center.y - point.y) <= halfDimension
    ;
}

bool AABB::intersectsAABB(AABB& other) {
    float sum = halfDimension + other.halfDimension;
    return abs(center.x - other.center.x) <= sum
        || abs(center.y - other.center.y) <= sum
    ;
}

void AABB::draw(piksel::Graphics& g, glm::vec4 color) {
    g.push();
    g.stroke(color);
    g.strokeWeight(1.0f);
    g.noFill();
    g.rectMode(piksel::DrawMode::RADIUS);
    g.rect(center.x, center.y, halfDimension, halfDimension);
    g.pop();
}`
        }, {
            name: 'aabb.hpp',
            content: `#ifndef AABB_HPP
#define AABB_HPP

#include "xy.hpp"
#include <piksel/graphics.hpp>

class AABB {
public:
    XY center;
    float halfDimension;

    AABB(XY center, float halfDimension)
        : center(center)
        , halfDimension(halfDimension) {
    };

    bool containsPoint(XY& point);
    bool intersectsAABB(AABB& other);
    void draw(piksel::Graphics& g, glm::vec4 color);
};

#endif /* AABB_HPP */`
        }, {
            name: 'app.cpp',
            content: `#include "app.hpp"
#include "quadtree.hpp"
#include <piksel/rng.hpp>

const glm::vec4 BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 GREEN(0.0f, 1.0f, 0.0f, 1.0f);

QuadTree* quadTree;
AABB aabb(XY(0.0f, 0.0f), 50.0f);

void App::setup() {
    quadTree = new QuadTree(
        AABB(XY(width / 2.0f, height / 2.0f), width / 2.0f)
    );
    piksel::Rng& rng = piksel::Rng::getInstance();
    for (int i = 0; i < 1000; i++) {
        float r = rng.random(0.0f, width);
        float phi = rng.random(0.0f, piksel::TWO_PI);
        glm::vec2 v = glm::vec2(width / 2.0f, height / 2.0f) +
            r * glm::vec2(cosf(phi), sinf(phi))
        ;
        quadTree->insert(XY(v.x, v.y));
    }
}

void App::draw(piksel::Graphics& g) {
    g.background(BLACK);
    quadTree->draw(g, WHITE, WHITE);

    aabb.draw(g, GREEN);
    for(XY& p : quadTree->queryRange(aabb)) {
        p.draw(g, GREEN);
    }
}

void App::mouseMoved(int x, int y) {
    aabb.center = XY(x, y);
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
    void mouseMoved(int x, int y);
};

#endif /* APP_HPP */`
        }, {
            name: 'main.cpp',
            content: `#include "app.hpp"

int main() {
    App app;
    app.start();
}`
        }, {
            name: 'quadtree.cpp',
            content: `#include "quadtree.hpp"

QuadTree::~QuadTree() {
    delete nw;
    delete ne;
    delete sw;
    delete se;
}

bool QuadTree::insert(XY p) {
    if (!boundary.containsPoint(p)) {
        return false;
    }
    
    if (points.size() < CAPACITY) {
        points.push_back(p);
        return true;
    }

    if (nw == nullptr) {
        subdivide();
    }

    return nw->insert(p) 
        || ne->insert(p)
        || sw->insert(p)
        || se->insert(p)
    ;
}

template <typename T>
void concat(std::vector<T>& a, const std::vector<T>& b) {
    a.insert(end(a), begin(b), end(b));
}

std::vector<XY> QuadTree::queryRange(AABB& range) {
    std::vector<XY> pointsInRange;

    if (!boundary.intersectsAABB(range)) {
        return pointsInRange;
    }

    for (XY& p : points) {
        if (range.containsPoint(p)) {
            pointsInRange.push_back(p);
        }
    }

    if (nw == nullptr) {
        return pointsInRange;
    }

    concat(pointsInRange, nw->queryRange(range));
    concat(pointsInRange, ne->queryRange(range));
    concat(pointsInRange, sw->queryRange(range));
    concat(pointsInRange, se->queryRange(range));

    return pointsInRange;
}

void QuadTree::subdivide() {
    float halfDimension = boundary.halfDimension / 2.0f;
    nw = new QuadTree(AABB(
        XY(
            boundary.center.x - halfDimension,
            boundary.center.y - halfDimension
        ),
        halfDimension
    ));
    ne = new QuadTree(AABB(
        XY(
            boundary.center.x + halfDimension,
            boundary.center.y - halfDimension
        ),
        halfDimension
    ));
    sw = new QuadTree(AABB(
        XY(
            boundary.center.x - halfDimension,
            boundary.center.y + halfDimension
        ),
        halfDimension
    ));
    se = new QuadTree(AABB(
        XY(
            boundary.center.x + halfDimension,
            boundary.center.y + halfDimension
        ),
        halfDimension
    ));

    for (XY& p : points) {
        if (nw->insert(p)
            || ne->insert(p)
            || sw->insert(p)
            || se->insert(p)
        ) {
            continue;
        }
    }   
}

void QuadTree::draw(
    piksel::Graphics& g, glm::vec4 boundaryColor, glm::vec4 pointColor
) {
    boundary.draw(g, boundaryColor);

    for (XY& p : points) {
        p.draw(g, pointColor);
    }
    
    if (nw == nullptr) {
        return;
    }

    nw->draw(g, boundaryColor, pointColor);
    ne->draw(g, boundaryColor, pointColor);
    sw->draw(g, boundaryColor, pointColor);
    se->draw(g, boundaryColor, pointColor);
}`
        }, {
            name: 'quadtree.hpp',
            content: `#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "aabb.hpp"
#include <piksel/graphics.hpp>
#include <vector>

#define CAPACITY 4

class QuadTree {
private:
    AABB boundary;
    std::vector<XY> points;
    QuadTree *nw, *ne, *sw, *se;

public:
    QuadTree(AABB boundary)
        : boundary(boundary)
        , points({})
        , nw(nullptr)
        , ne(nullptr)
        , sw(nullptr)
        , se(nullptr) {
    }

    ~QuadTree();
    
    bool insert(XY p);
    std::vector<XY> queryRange(AABB& range);

    void draw(
        piksel::Graphics& g, glm::vec4 boundaryColor, glm::vec4 pointColor
    );
    
private:
    void subdivide();
};

#endif /* QUADTREE_HPP */`
        }, {
            name: 'xy.cpp',
            content: `#include "xy.hpp"

void XY::draw(piksel::Graphics& g, glm::vec4 color) {
    g.push();
    g.stroke(color);
    g.strokeWeight(3.0f);
    g.point(x, y);
    g.pop();
}`
        }, {
            name: 'xy.hpp',
            content: `#ifndef XY_HPP
#define XY_HPP

#include <piksel/graphics.hpp>

class XY {
public:
    float x;
    float y;

    XY(float x, float y)
        : x(x)
        , y(y) {
    };

    void draw(piksel::Graphics& g, glm::vec4 color);
};

#endif /* XY_HPP */`
        }]
    },
    methods: {
        highlight: function(code) {
            return Prism.highlight(code, Prism.languages.cpp, 'cpp');
        }
    }
});
</script>
