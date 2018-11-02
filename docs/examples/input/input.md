# Input

[input](_media/input.html ':include width=512px height=512px scrolling=no')

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

void App::setup() {
}

void App::draw(piksel::Graphics& g) {
    g.background(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    g.stroke(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    g.line(lastX, 0, lastX, height);
    g.line(0, lastY, width, lastY);
    g.noStroke();
    g.text(std::to_string(lastX) + ", " + std::to_string(lastY),
        lastX + 5, lastY - 5);
    g.textSize(64);
    g.text(std::string(1, lastKey) + " : " + std::to_string(lastKey),
        width / 4, height / 2 + 16);
}

void App::keyPressed(int key) {
    lastKey = key;
}

void App::mouseMoved(int x, int y) {
    lastX = x;
    lastY = y;
}`
        }, {
            name: 'app.hpp',
            content: `#ifndef APP_HPP
#define APP_HPP

#include <piksel/baseapp.hpp>

class App : public piksel::BaseApp {
public:
    App() : BaseApp(512, 512), lastKey('A'), lastX(0), lastY(0) {}
    void setup();
    void draw(piksel::Graphics& g);
    void keyPressed(int key);
    void mouseMoved(int x, int y);
private:
    int lastKey;
    int lastX, lastY;
};

#endif /* APP_HPP */`
        }, {
            name: 'main.cpp',
            content: `#include "app.hpp"

int main() {
    App app;
    app.start();
}`
        }]
    },
    methods: {
        highlight: function(code) {
            return Prism.highlight(code, Prism.languages.cpp, 'cpp');
        }
    }
});
</script>
