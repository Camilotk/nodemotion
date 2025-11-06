#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include "NodeMotion/export/FfmpegExporter.hpp"
#include "NodeMotion/render/SfmlRenderer.hpp"
#include "NodeMotion/Node.hpp"

namespace nodemotion {

class NodeMotion {
  public:
    NodeMotion(unsigned width, unsigned height, unsigned fps);
    ~NodeMotion();

    void setBackground(std::uint32_t rgba);
    void setVerticalOffset(float offset);
    void setSpeed(float factor);
    void initList(const std::vector<Node*>& nodes);
    void setCircular(bool circular = false);

    void addCode(const std::vector<std::string>& lines);

    Node* createNode(const std::string& value);
    void markHead(Node* node);

    void addFirst(Node* node);
    void addLast(Node* node);
    void removeFirst();
    void removeLast();
    void remove(int index1Based);
    void get(int index1Based);

    void render(const std::string& filename);

  private:
    void drawCode(sf::RenderTexture& tex, int highlightLine);
    void layoutNodes(const std::vector<Node*>& order);

    unsigned m_width{1280};
    unsigned m_height{720};
    unsigned m_fps{30};
    float m_verticalOffset{0.f};
    float m_speedFactor{1.f};
    std::uint32_t m_background{0x151515FF};

    bool m_isCircular{false};

    FfmpegExporter* m_exporter{nullptr};
    SfmlRenderer* m_renderer{nullptr};

    std::vector<Node*> m_nodes;

    struct Operation {
        enum class Type {
            AddFirst,
            AddLast,
            RemoveFirst,
            RemoveLast,
            RemoveIndex,
            GetIndex
        };
        Type type;
        Node* node;
        std::size_t index;
    };

    std::vector<Operation> m_ops;

    Node* m_explicitHead{nullptr};
    std::vector<std::string> m_codeLines;
    std::vector<Node*> m_initialList;
};

}
