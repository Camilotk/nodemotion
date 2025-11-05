#include "NodeMotion/export/FfmpegExporter.hpp"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace nodemotion {

FfmpegExporter::~FfmpegExporter() {
    if (m_running)
        end();
}

void FfmpegExporter::begin(const std::string& filename, std::uint32_t width, std::uint32_t height,
                           std::uint32_t fps) {
    if (m_running)
        throw std::runtime_error("Exporter already running.");

    m_width = width;
    m_height = height;

    std::ostringstream cmd;
    cmd << "ffmpeg -y -f rawvideo -pixel_format rgba " << "-video_size " << width << "x" << height
        << " -framerate " << fps << " -i - -c:v libx264 -pix_fmt yuv420p " << "\"" << filename
        << "\"";

    std::cout << "[NodeMotion] Running command: " << cmd.str() << std::endl;

    openPipe(cmd.str());
    m_running = true;
}

void FfmpegExporter::captureFrame(const std::uint8_t* rgbaData, std::size_t numBytes) {
    if (!m_running || !m_pipe)
        return;

    const std::size_t expected =
        static_cast<std::size_t>(m_width) * static_cast<std::size_t>(m_height) * 4u;

    if (numBytes < expected)
        return;

    std::fwrite(rgbaData, 1, expected, m_pipe);
}

void FfmpegExporter::end() {
    if (!m_running)
        return;

    closePipe();
    m_running = false;

    std::cout << "[NodeMotion] Export complete." << std::endl;
}

void FfmpegExporter::openPipe(const std::string& cmd) {
    int pipefd[2];
    if (pipe(pipefd) == -1)
        throw std::runtime_error("Failed to create pipe");

    pid_t pid = fork();
    if (pid == -1)
        throw std::runtime_error("Failed to fork process");

    if (pid == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        execlp("bash", "bash", "-c", cmd.c_str(), (char*)nullptr);
        std::cerr << "[NodeMotion] Failed to exec ffmpeg." << std::endl;
        std::_Exit(1);
    }

    close(pipefd[0]);
    m_pipe = fdopen(pipefd[1], "wb");
    if (!m_pipe)
        throw std::runtime_error("Failed to open ffmpeg write stream");

    m_pid = pid;
}

void FfmpegExporter::closePipe() {
    if (m_pipe) {
        std::fclose(m_pipe);
        m_pipe = nullptr;
        int status = 0;
        waitpid(m_pid, &status, 0);
    }
}

}
