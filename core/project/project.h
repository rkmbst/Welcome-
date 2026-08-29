#ifndef BEAST_PROJECT_H
#define BEAST_PROJECT_H

#include <cstdint>
#include <string>
#include <vector>

#include "../time/time.h"

namespace beast {

/* ============================================================
 * Project ID
 * ============================================================ */

using ProjectId = std::uint64_t;

/* ============================================================
 * Project Settings
 * ============================================================ */

struct ProjectSettings final {
    std::uint32_t width{1920};
    std::uint32_t height{1080};

    FrameRate frameRate{30, 1};

    Time duration{Time::zero()};

    bool hdr{false};
};

/* ============================================================
 * Project
 * ============================================================ */

class Project final {
public:
    Project(
        ProjectId id,
        std::string name
    );

    ~Project() = default;

    Project(const Project&) = delete;
    Project& operator=(const Project&) = delete;

    Project(Project&&) noexcept = default;
    Project& operator=(Project&&) noexcept = default;

    /* --------------------------------------------------------
     * Identity
     * -------------------------------------------------------- */

    ProjectId id() const noexcept {
        return id_;
    }

    const std::string& name() const noexcept {
        return name_;
    }

    void setName(std::string name);

    /* --------------------------------------------------------
     * Settings
     * -------------------------------------------------------- */

    const ProjectSettings& settings() const noexcept {
        return settings_;
    }

    ProjectSettings& settings() noexcept {
        return settings_;
    }

    void setSettings(const ProjectSettings& settings) {
        settings_ = settings;
    }

    /* --------------------------------------------------------
     * Revision
     * -------------------------------------------------------- */

    std::uint64_t revision() const noexcept {
        return revision_;
    }

    void markChanged() noexcept {
        ++revision_;
    }

private:
    ProjectId id_{0};

    std::string name_;

    ProjectSettings settings_{};

    /*
     * Incremented whenever the logical project state changes.
     *
     * This becomes important later for:
     * - Snapshot isolation
     * - Render invalidation
     * - Autosave
     * - Undo/Redo
     * - Cache invalidation
     */
    std::uint64_t revision_{0};
};

} // namespace beast

#endif // BEAST_PROJECT_H
