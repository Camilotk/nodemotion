#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace nodemotion {

class Node {
  public:
    explicit Node(const std::string& value);

    void setSize(float width, float height);
    void setPosition(float x, float y); // center position

    void setNext(Node* next);
    Node* next() const;

    void setVisible(bool v);
    bool isVisible() const;

    void setHead(bool isHead);
    bool isHead() const;

    // World–space center of the pointer circle (anchor for arrows)
    sf::Vector2f pointerCenter() const;

    void draw(sf::RenderTarget& target) const;

  private:
    std::string m_value;
    float m_width{160.f};
    float m_height{80.f};
    float m_x{0.f}; // center
    float m_y{0.f}; // center

    Node* m_next{nullptr};
    bool m_visible{false};
    bool m_isHead{false};

    static sf::Font& font();
};

}
