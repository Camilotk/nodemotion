#include "NodeMotion/render/SfmlRenderer.hpp"
#include <stdexcept>

namespace nodemotion {

SfmlRenderer::SfmlRenderer(unsigned width, unsigned height, IVideoExporter* exporter)
    : m_width(width), m_height(height), m_exporter(exporter) {
    if (!m_texture.create(width, height))
        throw std::runtime_error("Failed to create SFML RenderTexture");
}

void SfmlRenderer::renderFrame(const FrameState& state) {
    m_texture.clear(sf::Color::Black);

    sf::CircleShape ball(state.radius);
    ball.setFillColor(sf::Color::Cyan);
    ball.setOrigin(state.radius, state.radius);
    ball.setPosition(state.x, state.y);

    m_texture.draw(ball);
    m_texture.display();

    if (m_exporter) {
        sf::Image frame = m_texture.getTexture().copyToImage();
        const std::uint8_t* pixels = frame.getPixelsPtr();
        const std::size_t bytes = m_width * m_height * 4;
        m_exporter->captureFrame(pixels, bytes);
    }
}

}
