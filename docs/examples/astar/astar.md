# A* search algorithm

[astar](_media/astar.html ':include width=512px height=512px scrolling=no')

<div id="el">
    <div class="tabs">
        <input name="tabs" type="radio" id="tab-1" checked="checked" class="input"/>
        <label for="tab-1" class="label">app.cpp</label>
        <div class="panel">
            <pre data-lang="cpp"><code class="lang-cpp" v-html="highlight(appcpp)"></code></pre>
        </div>
        <input name="tabs" type="radio" id="tab-2" class="input"/>
        <label for="tab-2" class="label">app.hpp</label>
        <div class="panel">
            <pre data-lang="cpp"><code class="lang-cpp" v-html="highlight(apphpp)"></code></pre>
        </div>
        <input name="tabs" type="radio" id="tab-3" class="input"/>
        <label for="tab-3" class="label">astar.cpp</label>
        <div class="panel">
            <pre data-lang="cpp"><code class="lang-cpp" v-html="highlight(astarcpp)"></code></pre>
        </div>
        <input name="tabs" type="radio" id="tab-4" class="input"/>
        <label for="tab-4" class="label">astar.hpp</label>
        <div class="panel">
            <pre data-lang="cpp"><code class="lang-cpp" v-html="highlight(astarhpp)"></code></pre>
        </div>
        <input name="tabs" type="radio" id="tab-5" class="input"/>
        <label for="tab-5" class="label">main.cpp</label>
        <div class="panel">
            <pre data-lang="cpp"><code class="lang-cpp" v-html="highlight(maincpp)"></code></pre>
        </div>
    </div>
</div>

<script>
new Vue({
    el: '#el',
    data: {
        appcpp: `#include "app.hpp"
#include "astar.hpp"
#include <glm/glm.hpp>
#include <piksel/rng.hpp>

const int rows = 100, cols = 100;
float w, h, wh, hh;
std::vector<bool> collisionGrid(cols * rows);
int gx = cols - 1, gy = rows - 1;
AStar* astar;
std::vector<std::pair<int, int>> path;

int coordsToIndex(int x, int y) {
    return y * cols + x;
}

void initCollisionGrid() {
    piksel::Rng& rng = piksel::Rng::getInstance();
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            float r = rng.random(0.0f, 1.0f);
            collisionGrid[coordsToIndex(x, y)] = r < 0.25f;
        }
    }
    // ensure there is no collision at the start point
    collisionGrid[coordsToIndex(cols / 2, rows / 2)] = false;
}

void initAStar() {
    delete astar;
    astar = new AStar(collisionGrid, cols, rows, true);
    path = astar->findPath(cols / 2, rows / 2, gx, gy);
}

void App::setup() {
    w = (float) width / cols;
    h = (float) height / rows;
    wh = w / 2.0f;
    hh = h / 2.0f;
    initCollisionGrid();
    initAStar();
}

void App::draw(piksel::Graphics& g) {
    g.background(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (collisionGrid[coordsToIndex(x, y)]) {
                g.stroke(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                g.strokeWeight(w);
                g.point(x * w + w / 2.0f, y * h + h / 2.0f);
            }
        }
    }
    if (path.size() > 1) {
        for (int i = 1; i < path.size(); i++) {
            g.stroke(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
            g.strokeWeight(wh);
            g.line(
                path[i - 1].first * w + wh,
                path[i - 1].second * h + hh,
                path[i].first * w + wh,
                path[i].second * h + hh
            );
        }
    }
}

void App::mouseMoved(int x, int y) {
    int prevGx = gx, prevGy = gy;
    gx = glm::clamp<int>(x / w, 0, cols - 1);
    gy = glm::clamp<int>(y / h, 0, rows - 1);
    if (gx != prevGx || gy != prevGy) {
        initAStar();
    }
}`,
        apphpp: `#ifndef APP_HPP
#define APP_HPP

#include <piksel/baseapp.hpp>

class App : public piksel::BaseApp {
public:
    App() : BaseApp(512, 512) {}
    void setup();
    void draw(piksel::Graphics& g);
    void mouseMoved(int x, int y);
};

#endif /* APP_HPP */`,
        astarcpp: `#include "astar.hpp"
#include <queue>
#include <cmath>
#include <algorithm>

struct Node {
    int x, y;
    float gScore, hScore;
    Node* cameFrom;

    Node(int x, int y, float gScore, float hScore, Node* cameFrom)
        : x(x)
        , y(y)
        , gScore(gScore)
        , hScore(hScore)
        , cameFrom(cameFrom) {
    }

    bool hasCoords(int x, int y) {
        return this->x == x && this->y == y;
    }
};

struct NodeCompare {
    bool operator()(const Node* a, const Node* b) {
        return a->gScore + a->hScore > b->gScore + b->hScore;
    }
};

/**
 * MyPriorityQueue is an extension of
 * std::priority_queue that allows iteration
 */
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> struct MyPriorityQueue : std::priority_queue<T, Container, Compare> {
    typename Container::iterator begin() {
        return std::priority_queue<T, Container, Compare>::c.begin();
    }
    typename Container::iterator end() {
        return std::priority_queue<T, Container, Compare>::c.end();
    }
};

const float SQUARE_ROOT_OF_TWO = sqrtf(2.0f);

float AStar::heuristic_cost_estimate(int x1, int y1, int x2, int y2) {
    if (moveDiagonally) {
         // euclidian distance
        return sqrtf(powf(x1 - x2, 2.0f) + powf(y1 - y2, 2.0f));
    } else {
        // manhattan distance
        return abs(x1 - x2) + abs(y1 - y2);
    }
}

bool AStar::isCollisionAt(int x, int y) {
    return collisionGrid[y * cols + x];
}

/**
 * Implementation of A* search algorithm
 * based on https://en.wikipedia.org/wiki/A*_search_algorithm
 */
std::vector<std::pair<int, int>> AStar::findPath(
    int x1, int y1, int x2, int y2
) {
    std::vector<std::pair<int, int>> path;
    if (isCollisionAt(x1, y1) || isCollisionAt(x2, y2)) {
        return path;
    }
    std::vector<Node*> closedSet;
    MyPriorityQueue<Node*, std::vector<Node*>, NodeCompare> openSet;
    Node* start = new Node(
        x1, y1, 0.0f, heuristic_cost_estimate(x1, y1, x2, y2), nullptr
    );
    openSet.push(start);

    Node* current = nullptr;
    while (!openSet.empty()) {
        current = openSet.top();
        if (current->hasCoords(x2, y2)) {
            break;
        }

        openSet.pop();
        closedSet.push_back(current);

        for (int i = 0; i < (moveDiagonally ? 8 : 4); i++) {
            int x = current->x + directions[i].first;
            int y = current->y + directions[i].second;
            if (x < 0 || x >= cols 
                || y < 0 || y >= rows
                || isCollisionAt(x, y)) {
                continue;
            }
            bool cont = false;
            for (Node* node : closedSet) {
                if (node->hasCoords(x, y)) {
                    cont = true;
                    break;
                }
            }
            if (cont) {
                continue;
            }

            float dist_between = (i < 4 ? 1.0f : SQUARE_ROOT_OF_TWO);
            int tentative_gScore = current->gScore + dist_between;

            Node* neighbor = nullptr;
            for (Node* node : openSet) {
                if (node->hasCoords(x, y)) {
                    neighbor = node;
                    break;
                }
            }
            if (neighbor == nullptr) { // neighbor not in openSet
                neighbor = new Node(
                    x,
                    y,
                    tentative_gScore,
                    heuristic_cost_estimate(x, y, x2, y2),
                    current
                );
                openSet.push(neighbor);
            } else if (tentative_gScore < neighbor->gScore) {
                neighbor->cameFrom = current;
                neighbor->gScore = tentative_gScore;
            }
        }
    }

    if (current != nullptr && current->hasCoords(x2, y2)) {
        while(current != nullptr) {
            path.push_back({current->x, current->y});
            current = current->cameFrom;
        }
        std::reverse(path.begin(), path.end());
    }

    return path;
}`,
        astarhpp: `#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <vector>
#include <utility>

class AStar {
public:
    AStar(const std::vector<bool>& collisionGrid,
        int cols, int rows, bool moveDiagonally)
        : collisionGrid(collisionGrid)
        , cols(cols)
        , rows(rows)
        , moveDiagonally(moveDiagonally) {
    }
    std::vector<std::pair<int, int>> findPath(
        int x1, int y1, int x2, int y2
    );

private:
    const std::vector<bool>& collisionGrid;
    int cols, rows;
    bool moveDiagonally;
    const std::vector<std::pair<int, int>> directions = {
        { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 },
        { -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }
    };
    
    float heuristic_cost_estimate(int x1, int y1, int x2, int y2);
    bool isCollisionAt(int x, int y);
};

#endif /* ASTAR_HPP */`,
        maincpp: `#include "app.hpp"

int main() {
    App app;
    app.start();
}`
    },
    methods: {
        highlight: function(code) {
            return Prism.highlight(code, Prism.languages.cpp, 'cpp');
        }
    }
});
</script>
