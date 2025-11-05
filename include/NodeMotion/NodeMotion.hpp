#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "NodeMotion/Node.hpp"
#include "NodeMotion/export/FfmpegExporter.hpp"
#include "NodeMotion/render/SfmlRenderer.hpp"

namespace nodemotion {

class NodeMotion {
  public:
    NodeMotion(unsigned width, unsigned height, unsigned fps);
    ~NodeMotion(); // RAII

    void setBackground(std::uint32_t rgba);
    void setVerticalOffset(float offset);
    void setSpeed(float factor);
    void addCode(const std::vector<std::string>& lines);

    Node* createNode(const std::string& value);
    void   linkNodes(Node* from, Node* to);
    void   markHead(Node* node);

    void render(const std::string& filename);

  private:
    unsigned      m_width;
    unsigned      m_height;
    unsigned      m_fps;
    std::uint32_t m_background{0x151515FF};
    float         m_verticalOffset{0.f};
    float         m_speedFactor{1.f};

    std::unique_ptr<FfmpegExporter> m_exporter;
    std::unique_ptr<SfmlRenderer>   m_renderer;

    std::vector<std::unique_ptr<Node>> m_nodes;
    std::unordered_map<Node*, Node*>   m_links;
    Node*                              m_head{nullptr};
    std::vector<std::string>           m_codeLines;

    void drawCode(sf::RenderTexture& tex, int highlightLine);
    void layoutNodes();
};

}
