#include "NodeMotion/NodeMotion.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>

namespace nodemotion {

// -----------------------------------------------------------------------------
// ctor / dtor
// -----------------------------------------------------------------------------

NodeMotion::NodeMotion(unsigned width, unsigned height, unsigned fps)
    : m_width(width),
      m_height(height),
      m_fps(fps),
      m_exporter(new FfmpegExporter()),
      m_renderer(new SfmlRenderer(width, height, m_exporter)) {}

NodeMotion::~NodeMotion() {
    // renderer/exporter own no external resources past here
    delete m_renderer;
    delete m_exporter;

    for (Node* n : m_nodes) {
        delete n;
    }
}

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------

void NodeMotion::setBackground(std::uint32_t rgba) {
    m_background = rgba;
}

void NodeMotion::setVerticalOffset(float offset) {
    m_verticalOffset = offset;
}

void NodeMotion::setSpeed(float factor) {
    if (factor > 0.f) {
        m_speedFactor = factor;
    }
}

void NodeMotion::addCode(const std::vector<std::string>& lines) {
    m_codeLines = lines;
}

// -----------------------------------------------------------------------------
// List setup & operations
// -----------------------------------------------------------------------------

Node* NodeMotion::createNode(const std::string& value) {
    Node* n = new Node(value);
    n->setVisible(false);
    m_nodes.push_back(n);
    return n;
}

void NodeMotion::initList(const std::vector<Node*>& nodes) {
    m_initialList = nodes;
}

void NodeMotion::setCircular(bool v) {
    m_isCircular = v;
}

void NodeMotion::addLast(Node* node) {
    if (!node) return;
    Operation op;
    op.type = Operation::Type::AddLast;
    op.node = node;
    m_ops.push_back(op);
}

void NodeMotion::addFirst(Node* node) {
    if (!node) return;
    Operation op;
    op.type = Operation::Type::AddFirst;
    op.node = node;
    m_ops.push_back(op);
}

void NodeMotion::removeLast() {
    Operation op;
    op.type = Operation::Type::RemoveLast;
    m_ops.push_back(op);
}

void NodeMotion::removeFirst() {
    Operation op;
    op.type = Operation::Type::RemoveFirst;
    m_ops.push_back(op);
}

void NodeMotion::remove(int index1Based) {
    if (index1Based <= 0) return;
    Operation op;
    op.type  = Operation::Type::RemoveIndex;
    op.index = static_cast<std::size_t>(index1Based - 1); // store 0-based
    m_ops.push_back(op);
}

void NodeMotion::get(int index1Based) {
    if (index1Based <= 0) return;
    Operation op;
    op.type  = Operation::Type::GetIndex;
    op.index = static_cast<std::size_t>(index1Based - 1);
    m_ops.push_back(op);
}

void NodeMotion::markHead(Node* node) {
    m_explicitHead = node;
}

// -----------------------------------------------------------------------------
// Code drawing
// -----------------------------------------------------------------------------

void NodeMotion::drawCode(sf::RenderTexture& tex, int highlightLine) {
    static sf::Font font;
    static bool loaded = false;
    if (!loaded) {
        if (!font.loadFromFile(
                "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf")) {
            font.loadFromFile(
                "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
        }
        loaded = true;
    }

    const float startX = 40.f;
    const float startY = 40.f;
    const float lineH  = 28.f;
    const unsigned size = 22;

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

// -----------------------------------------------------------------------------
// Layout
// -----------------------------------------------------------------------------

void NodeMotion::layoutNodes(const std::vector<Node*>& logical) {
    if (logical.empty()) return;

    const float nodeW = 160.f;
    const float nodeH = 80.f;
    const float horizontalMargin = 40.f;   // now uses space under code too
    const float spacing = 60.f;
    const float rowGap = nodeH + 40.f;

    const float usableWidth =
        static_cast<float>(m_width) - 2.f * horizontalMargin;

    const int perRow = std::max(
        1, static_cast<int>(
               std::floor((usableWidth + spacing) / (nodeW + spacing))));

    const float baseY =
        static_cast<float>(m_height) * 0.6f + m_verticalOffset;

    // Place nodes in logical order; hidden nodes keep whatever stale pos
    for (std::size_t i = 0; i < logical.size(); ++i) {
        Node* n = logical[i];
        if (!n) continue;

        int row = static_cast<int>(i / perRow);
        int col = static_cast<int>(i % perRow);

        int nodesInRow =
            std::min(perRow,
                     static_cast<int>(logical.size()) - row * perRow);
        float rowWidth =
            nodesInRow * (nodeW + spacing) - spacing;

        float startX = (static_cast<float>(m_width) - rowWidth) / 2.f;

        float x = startX + col * (nodeW + spacing) + nodeW * 0.5f;
        float y = baseY + row * rowGap;

        n->setSize(nodeW, nodeH);
        n->setPosition(x, y);
    }
}

// -----------------------------------------------------------------------------
// Render
// -----------------------------------------------------------------------------
void NodeMotion::render(const std::string& filename) {
    if (!m_exporter || !m_renderer) return;

    m_exporter->begin(filename, m_width, m_height, m_fps);

    // Logical list starts from initial configuration
    std::vector<Node*> logical = m_initialList;

    const int totalSteps = static_cast<int>(m_ops.size()) + 1; // +1 for declaration
    const int framesPerStep = std::max(
        1, static_cast<int>(std::round(m_fps * m_speedFactor)));

    for (int step = 0; step < totalSteps; ++step) {
        // --- 1) Apply operation for this step (after initial state) ----
        if (step > 0) {
            const Operation& op = m_ops[step - 1];

            switch (op.type) {
                // --- Insert at front (also update HEAD) ---
                case Operation::Type::AddFirst: {
                    Node* node = op.node;
                    if (node) {
                        auto it = std::find(logical.begin(), logical.end(), node);
                        if (it != logical.end())
                            logical.erase(it);
                        logical.insert(logical.begin(), node);
                        m_explicitHead = node; // new node becomes HEAD
                    }
                    break;
                }

                // --- Insert at end ---
                case Operation::Type::AddLast: {
                    Node* node = op.node;
                    if (node) {
                        auto it = std::find(logical.begin(), logical.end(), node);
                        if (it != logical.end())
                            logical.erase(it);
                        logical.push_back(node);
                    }
                    break;
                }

                // --- Remove first node ---
                case Operation::Type::RemoveFirst: {
                    if (!logical.empty()) {
                        Node* removed = logical.front();
                        logical.erase(logical.begin());
                        if (removed) {
                            removed->setNext(nullptr);
                            removed->setVisible(false);
                        }
                        // If head was removed, reset to new first
                        if (!logical.empty())
                            m_explicitHead = logical.front();
                        else
                            m_explicitHead = nullptr;
                    }
                    break;
                }

                // --- Remove last node ---
                case Operation::Type::RemoveLast: {
                    if (!logical.empty()) {
                        Node* removed = logical.back();
                        logical.pop_back();
                        if (removed) {
                            removed->setNext(nullptr);
                            removed->setVisible(false);
                        }
                        if (logical.empty())
                            m_explicitHead = nullptr;
                    }
                    break;
                }

                // --- Remove by index ---
                case Operation::Type::RemoveIndex: {
                    std::size_t idx = op.index;
                    if (idx < logical.size()) {
                        Node* removed = logical[idx];
                        logical.erase(logical.begin() + idx);
                        if (removed) {
                            removed->setNext(nullptr);
                            removed->setVisible(false);
                        }
                        // Adjust head if we removed the first
                        if (idx == 0) {
                            if (!logical.empty())
                                m_explicitHead = logical.front();
                            else
                                m_explicitHead = nullptr;
                        }
                    }
                    break;
                }

                // --- Access by index (no structural change) ---
                case Operation::Type::GetIndex:
                    break;
            }
        }

        // --- 2) Visibility: only logical nodes are visible ---
        for (Node* n : m_nodes) n->setVisible(false);
        for (Node* n : logical) n->setVisible(true);

        // --- 3) Head marker ---
        Node* head = nullptr;
        if (!logical.empty()) {
            head = logical.front();
            if (m_explicitHead) {
                auto it = std::find(logical.begin(), logical.end(), m_explicitHead);
                if (it != logical.end())
                    head = m_explicitHead;
            }
        }
        for (Node* n : m_nodes)
            n->setHead(n == head);

        // --- 4) Reset all links ---
        for (Node* n : m_nodes)
            n->setNext(nullptr);

        // --- 5) Build valid link chain ---
        if (!logical.empty()) {
            for (std::size_t i = 0; i + 1 < logical.size(); ++i)
                logical[i]->setNext(logical[i + 1]);
            if (m_isCircular && logical.size() > 1)
                logical.back()->setNext(logical.front());
        }

        // --- 6) Re-layout nodes dynamically ---
        layoutNodes(logical);

        // --- 7) Highlight correct line (step-synced) ---
        int highlightLine = (step < static_cast<int>(m_codeLines.size())) ? step : -1;

        // --- 8) Render this step (multiple identical frames) ---
        for (int f = 0; f < framesPerStep; ++f) {
            m_renderer->clear(m_background);
            drawCode(m_renderer->getTexture(), highlightLine);
            for (Node* n : m_nodes)
                n->draw(m_renderer->getTexture());
            m_renderer->display();
        }
    }

    // --- Final still frame (no highlight) ---
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
