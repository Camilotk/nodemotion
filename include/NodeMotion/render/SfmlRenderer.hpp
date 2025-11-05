#pragma once
#include <SFML/Graphics.hpp>
#include "NodeMotion/Renderer.hpp"

namespace nodemotion {

class SfmlRenderer : public IRenderer2D {
  public:
    SfmlRenderer(unsigned width, unsigned height, IVideoExporter* exporter = nullptr);

    void renderFrame(const FrameState& state) override;

    sf::RenderTexture& getTexture() {
        return m_texture;
    }
    void clear(uint32_t color);
    void display();

  private:
    unsigned m_width;
    unsigned m_height;
    sf::RenderTexture m_texture;
    IVideoExporter* m_exporter{nullptr};
};

} // namespace nodemotion
