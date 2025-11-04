#pragma once
#include <cstdint>
#include <string>

namespace nodemotion {

struct FrameState {
    float x;
    float y;
    float radius;
};

class IRenderer2D {
  public:
    virtual ~IRenderer2D() = default;
    virtual void renderFrame(const FrameState& state) = 0;
};

class IVideoExporter {
  public:
    virtual ~IVideoExporter() = default;

    virtual void begin(const std::string& filename, std::uint32_t width, std::uint32_t height,
                       std::uint32_t fps) = 0;

    virtual void captureFrame(const std::uint8_t* rgbaData, std::size_t numBytes) = 0;

    virtual void end() = 0;
};

} // namespace nodemotion
