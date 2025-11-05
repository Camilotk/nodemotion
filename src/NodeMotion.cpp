#include "NodeMotion/NodeMotion.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

namespace nodemotion {

NodeMotion::NodeMotion(unsigned width, unsigned height, unsigned fps)
    : m_width(width), m_height(height), m_fps(fps) {
    m_exporter = std::make_unique<FfmpegExporter>();
    m_renderer = std::make_unique<SfmlRenderer>(width, height, m_exporter.get());
}

NodeMotion::~NodeMotion() = default; // unique_ptr + vector<unique_ptr> clean up

void NodeMotion::setBackground(std::uint32_t rgba) {
    m_background = rgba;
}

void NodeMotion::setVerticalOffset(float offset) {
    m_verticalOffset = offset;
    layoutNodes();
}

void NodeMotion::setSpeed(float factor) {
    if (factor > 0.f) m_speedFactor = factor;
}

void NodeMotion::addCode(const std::vector<std::string>& lines) {
    m_codeLines = lines;
}

Node* NodeMotion::createNode(const std::string& value) {
    auto node = std::make_unique<Node>(value);
    node->setVisible(false);
    Node* raw = node.get();
    m_nodes.push_back(std::move(node));
    layoutNodes();
    return raw;
}

void NodeMotion::linkNodes(Node* from, Node* to) {
    if (!from) return;
    m_links[from] = to;
}

void NodeMotion::markHead(Node* node) {
    m_head = node;
    for (auto& up : m_nodes)
        up->setHead(up.get() == m_head);
}

void NodeMotion::drawCode(sf::RenderTexture& tex, int highlightLine) {
    static sf::Font font;
    static bool loaded = false;
    if (!loaded) {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf")) {
            font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
        }
        loaded = true;
    }

    const float    startX = 40.f;
    const float    startY = 40.f;
    const float    lineH  = 28.f;
    const unsigned size   = 22;

    for (std::size_t i = 0; i < m_codeLines.size(); ++i) {
        sf::Text t(m_codeLines[i], font, size);
        if (static_cast<int>(i) == highlightLine) {
            t.setFillColor(sf::Color::White);
        } else {
            t.setFillColor(sf::Color(160, 160, 160));
        }
        t.setPosition(startX, startY + lineH * static_cast<float>(i));
        tex.draw(t);
    }
}

void NodeMotion::layoutNodes() {
    if (m_nodes.empty()) return;

    const float nodeW            = 160.f;
    const float nodeH            = 80.f;
    const float horizontalMargin = 80.f;
    const float spacing          = 60.f;
    const float rowGap           = nodeH + 40.f;

    const float usableWidth = static_cast<float>(m_width) - 2 * horizontalMargin;
    const int   perRow      = std::max(
        1, static_cast<int>(std::floor((usableWidth + spacing) / (nodeW + spacing))));

    const float baseY = static_cast<float>(m_height) * 0.6f + m_verticalOffset;

    for (std::size_t i = 0; i < m_nodes.size(); ++i) {
        int row = static_cast<int>(i / perRow);
        int col = static_cast<int>(i % perRow);

        // Center each row horizontally based on how many nodes fit that row
        int nodesInThisRow =
            std::min(perRow, static_cast<int>(m_nodes.size()) - row * perRow);
        float rowWidth = nodesInThisRow * (nodeW + spacing) - spacing;
        float startX   = (m_width - rowWidth) / 2.f; // center horizontally

        float x = startX + col * (nodeW + spacing) + nodeW * 0.5f;
        float y = baseY + row * rowGap;

        Node* n = m_nodes[i].get();
        n->setPosition(x, y);
        n->setSize(nodeW, nodeH);
    }
}

void NodeMotion::render(const std::string& filename) {
    if (!m_exporter || !m_renderer)
        return;

    m_exporter->begin(filename, m_width, m_height, m_fps);

    const int totalSteps    = static_cast<int>(m_nodes.size()) + 1; // +1 for list declaration
    const int framesPerStep = std::max(
        1, static_cast<int>(std::round(static_cast<float>(m_fps) * m_speedFactor)));

    for (int step = 0; step < totalSteps; ++step) {
        for (std::size_t i = 0; i < m_nodes.size(); ++i) {
            bool visible = (static_cast<int>(i) < step - 1);
            m_nodes[i]->setVisible(visible);
        }

        markHead(m_head);

        for (Node* n : m_nodes) {
            Node* logicalNext = nullptr;
            auto it = m_links.find(n);
            if (it != m_links.end())
                logicalNext = it->second;

            Node* visibleNext = (logicalNext && logicalNext->isVisible()) ? logicalNext : nullptr;
            n->setNext(visibleNext);
        }

        for (int f = 0; f < framesPerStep; ++f) {
            m_renderer->clear(m_background);
            drawCode(m_renderer->getTexture(), step);

            for (Node* n : m_nodes)
                n->draw(m_renderer->getTexture());

            m_renderer->display();
        }
    }

    for (Node* n : m_nodes)
        n->setVisible(true);

    for (Node* n : m_nodes) {
        Node* logicalNext = nullptr;
        auto it = m_links.find(n);
        if (it != m_links.end())
            logicalNext = it->second;
        n->setNext(logicalNext);
    }

    for (int f = 0; f < static_cast<int>(m_fps); ++f) {
        m_renderer->clear(m_background);
        drawCode(m_renderer->getTexture(), -1);
        for (Node* n : m_nodes)
            n->draw(m_renderer->getTexture());
        m_renderer->display();
    }

    m_exporter->end();
}

}
