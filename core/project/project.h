#ifndef BEAST_PROJECT_H
#define BEAST_PROJECT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "asset.h"
#include "sequence.h"

namespace beast {

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
 *
 * Root domain object of the editor.
 *
 * Ownership:
 *
 * Project
 *   ├── owns Assets
 *   └── owns Sequences
 *          └── owns Tracks
 *                 └── owns Clips
 *                        └── references AssetId
 *
 * The Project itself knows nothing about:
 * - Flutter
 * - FFmpeg
 * - GPU
 * - Audio hardware
 * - Database
 *
 * It is pure editor-domain state.
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

    void setSettings(const ProjectSettings& settings);

    /* --------------------------------------------------------
     * Asset Ownership
     * -------------------------------------------------------- */

    bool addAsset(std::unique_ptr<Asset> asset);

    bool removeAsset(AssetId assetId);

    Asset* findAsset(AssetId assetId) noexcept;

    const Asset* findAsset(AssetId assetId) const noexcept;

    const std::vector<std::unique_ptr<Asset>>& assets() const noexcept {
        return assets_;
    }

    std::size_t assetCount() const noexcept {
        return assets_.size();
    }

    /* --------------------------------------------------------
     * Sequence Ownership
     * -------------------------------------------------------- */

    bool addSequence(std::unique_ptr<Sequence> sequence);

    bool removeSequence(SequenceId sequenceId);

    Sequence* findSequence(SequenceId sequenceId) noexcept;

    const Sequence* findSequence(
        SequenceId sequenceId
    ) const noexcept;

    const std::vector<std::unique_ptr<Sequence>>& sequences()
        const noexcept {
        return sequences_;
    }

    std::size_t sequenceCount() const noexcept {
        return sequences_.size();
    }

    /* --------------------------------------------------------
     * Active Sequence
     *
     * The project may eventually contain multiple sequences.
     * Exactly which sequence is active is an editor concern,
     * but the project stores the selected sequence identity.
     * -------------------------------------------------------- */

    SequenceId activeSequenceId() const noexcept {
        return activeSequenceId_;
    }

    bool setActiveSequence(SequenceId sequenceId);

    Sequence* activeSequence() noexcept;

    const Sequence* activeSequence() const noexcept;

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

    std::vector<std::unique_ptr<Asset>> assets_;

    std::vector<std::unique_ptr<Sequence>> sequences_;

    SequenceId activeSequenceId_{0};

    std::uint64_t revision_{0};
};

} // namespace beast

#endif // BEAST_PROJECT_H
