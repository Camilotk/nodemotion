#include "NodeMotion/Renderer.hpp"
#include "NodeMotion/export/FfmpegExporter.hpp"
#include "NodeMotion/render/SfmlRenderer.hpp"

int main() {
    using namespace nodemotion;

    const unsigned width = 800;
    const unsigned height = 600;
    const unsigned fps = 60;
    const float seconds = 3.0f;

    FfmpegExporter exporter;
    exporter.begin("motion.mp4", width, height, fps);

    SfmlRenderer renderer(width, height, &exporter);

    const float radius = 30.f;
    const float y = height / 2.f;
    const float startX = radius;
    const float endX = width - radius;
    const unsigned totalFrames = fps * seconds;

    for (unsigned frame = 0; frame < totalFrames; ++frame) {
        float t = static_cast<float>(frame) / (totalFrames - 1);
        float x = startX + t * (endX - startX);
        renderer.renderFrame({x, y, radius});
    }

    exporter.end();
}
