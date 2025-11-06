#include "NodeMotion/Node.hpp"
#include <cmath>

namespace nodemotion {

static sf::Font g_nodeFont;
static bool g_fontLoaded = false;

sf::Font& Node::font() {
    if (!g_fontLoaded) {
        if (!g_nodeFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf")) {
            g_nodeFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
        }
        g_fontLoaded = true;
    }
    return g_nodeFont;
}

Node::Node(const std::string& value)
    : m_value(value) {}

void Node::setSize(float width, float height) {
    m_width  = width;
    m_height = height;
}

void Node::setPosition(float x, float y) {
    m_x = x;
    m_y = y;
}

void Node::setNext(Node* next) {
    m_next = next;
}

Node* Node::next() const {
    return m_next;
}

void Node::setVisible(bool v) {
    m_visible = v;
}

bool Node::isVisible() const {
    return m_visible;
}

void Node::setHead(bool isHead) {
    m_isHead = isHead;
}

bool Node::isHead() const {
    return m_isHead;
}

void Node::setHighlighted(bool on) {
    m_highlighted = on;
}

bool Node::isHighlighted() const {
    return m_highlighted;
}

sf::Vector2f Node::pointerCenter() const {
    float left     = m_x - m_width / 2.f;
    float centerX  = left + m_width * 0.75f;
    float centerY  = m_y;
    return {centerX, centerY};
}

void Node::draw(sf::RenderTarget& target) const {
    if (!m_visible) return;

    sf::Font& f = font();

    const float left = m_x - m_width / 2.f;
    const float top  = m_y - m_height / 2.f;

    // Outer node rectangle
    sf::RectangleShape outer({m_width, m_height});
    outer.setPosition(left, top);

    // Highlighted nodes glow cyan
    if (m_highlighted)
        outer.setFillColor(sf::Color(20, 80, 80));
    else
        outer.setFillColor(sf::Color(60, 60, 60));

    outer.setOutlineThickness(2.f);
    outer.setOutlineColor(m_highlighted ? sf::Color(0, 255, 255) : sf::Color::White);
    target.draw(outer);

    // Value half
    sf::RectangleShape leftRect({m_width * 0.5f, m_height});
    leftRect.setPosition(left, top);
    leftRect.setFillColor(m_highlighted ? sf::Color(30, 100, 100) : sf::Color(40, 40, 40));
    target.draw(leftRect);

    // Pointer half
    sf::RectangleShape rightRect({m_width * 0.5f, m_height});
    rightRect.setPosition(left + m_width * 0.5f, top);
    rightRect.setFillColor(m_highlighted ? sf::Color(60, 120, 120) : sf::Color(80, 80, 80));
    target.draw(rightRect);

    // Value text
    sf::Text valueText(m_value, f, static_cast<unsigned>(m_height * 0.5f));
    valueText.setFillColor(sf::Color(0, 190, 255));
    valueText.setPosition(left + m_width * 0.12f, top + m_height * 0.15f);
    target.draw(valueText);

    // Pointer circle
    sf::Vector2f pc = pointerCenter();
    sf::CircleShape circle(m_height * 0.12f);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(pc);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(2.f);
    circle.setOutlineColor(sf::Color::White);
    target.draw(circle);

    // HEAD label
    if (m_isHead) {
        sf::Text headText("HEAD", f, static_cast<unsigned>(m_height * 0.4f));
        headText.setFillColor(sf::Color(255, 215, 0));
        headText.setPosition(m_x - m_width * 0.3f, top - m_height * 0.8f);
        target.draw(headText);
    }

    // Draw pointer arrow (same as before)
    if (m_next) {
        sf::Vector2f start = pc;
        float nextLeft     = m_next->m_x - m_next->m_width / 2.f;
        sf::Vector2f end(nextLeft - 10.f, m_next->m_y);

        sf::Vertex line[] = {
            {start, sf::Color::White},
            {end,   sf::Color::White}
        };
        target.draw(line, 2, sf::Lines);

        const float ah = 8.f;
        sf::Vector2f dir = end - start;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0.0001f) {
            dir.x /= len; dir.y /= len;
            sf::Vector2f n(-dir.y, dir.x);
            sf::Vertex head[3];
            head[0] = sf::Vertex(end, sf::Color::White);
            head[1] = sf::Vertex(end - dir * ah + n * ah * 0.5f, sf::Color::White);
            head[2] = sf::Vertex(end - dir * ah - n * ah * 0.5f, sf::Color::White);
            target.draw(head, 3, sf::Triangles);
        }
    } else {
        sf::Vector2f c   = pc;
        float downLen    = m_height * 0.5f;
        sf::Vector2f end = {c.x, c.y + downLen};

        sf::Vertex vline[] = {
            {c,   sf::Color::White},
            {end, sf::Color::White}
        };
        target.draw(vline, 2, sf::Lines);

        const float ah = 8.f;
        sf::Vertex head[3];
        head[0] = sf::Vertex(end, sf::Color::White);
        head[1] = sf::Vertex({end.x - ah * 0.6f, end.y - ah}, sf::Color::White);
        head[2] = sf::Vertex({end.x + ah * 0.6f, end.y - ah}, sf::Color::White);
        target.draw(head, 3, sf::Triangles);

        sf::Text nullText("NULL", f, static_cast<unsigned>(m_height * 0.45f));
        nullText.setFillColor(sf::Color(255, 64, 64));
        nullText.setPosition(end.x - m_width * 0.18f, end.y + 4.f);
        target.draw(nullText);
    }
}

}
