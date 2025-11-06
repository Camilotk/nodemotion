#include <cstdio>
#include <string>
#include <cstdint>
#include <sys/types.h>
#include "NodeMotion/Renderer.hpp"

namespace nodemotion {

class FfmpegExporter : public IVideoExporter {
  public:
    FfmpegExporter() = default;
    ~FfmpegExporter() override;

    void begin(const std::string& filename, std::uint32_t width, std::uint32_t height,
               std::uint32_t fps) override;

    void captureFrame(const std::uint8_t* rgbaData, std::size_t numBytes) override;

    void end() override;

  private:
    std::FILE* m_pipe{nullptr};
    pid_t m_pid{0};
    bool m_running{false};
    std::uint32_t m_width{0};
    std::uint32_t m_height{0};

    void openPipe(const std::string& cmd);
    void closePipe();
};

}
