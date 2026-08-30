#ifndef BEAST_SEQUENCE_H
#define BEAST_SEQUENCE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../time/time.h"
#include "../timeline/track.h"

namespace beast {

using SequenceId = std::uint64_t;

/* ============================================================
 * Sequence Settings
 * ============================================================ */

struct SequenceSettings final {
    std::uint32_t width{1920};
    std::uint32_t height{1080};

    FrameRate frameRate{30, 1};

    Time duration{Time::zero()};

    bool hdr{false};
};

/* ============================================================
 * Sequence
 *
 * A Sequence is the timeline container inside a Project.
 *
 * Ownership:
 *
 * Sequence
 *   └── owns Tracks
 *          └── owns Clips
 *                 └── references Assets by AssetId
 * ============================================================ */

class Sequence final {
public:
    Sequence(
        SequenceId id,
        std::string name
    );

    ~Sequence() = default;

    Sequence(const Sequence&) = delete;
    Sequence& operator=(const Sequence&) = delete;

    Sequence(Sequence&&) noexcept = default;
    Sequence& operator=(Sequence&&) noexcept = default;

    /* --------------------------------------------------------
     * Identity
     * -------------------------------------------------------- */

    SequenceId id() const noexcept {
        return id_;
    }

    const std::string& name() const noexcept {
        return name_;
    }

    void setName(std::string name);

    /* --------------------------------------------------------
     * Settings
     * -------------------------------------------------------- */

    const SequenceSettings& settings() const noexcept {
        return settings_;
    }

    SequenceSettings& settings() noexcept {
        return settings_;
    }

    void setSettings(const SequenceSettings& settings) {
        settings_ = settings;
    }

    /* --------------------------------------------------------
     * Track Ownership
     * -------------------------------------------------------- */

    bool addTrack(std::unique_ptr<Track> track);

    bool removeTrack(TrackId trackId);

    Track* findTrack(TrackId trackId) noexcept;

    const Track* findTrack(TrackId trackId) const noexcept;

    /* --------------------------------------------------------
     * Track Access
     * -------------------------------------------------------- */

    const std::vector<std::unique_ptr<Track>>& tracks() const noexcept {
        return tracks_;
    }

    std::size_t trackCount() const noexcept {
        return tracks_.size();
    }

    /* --------------------------------------------------------
     * Timeline Queries
     * -------------------------------------------------------- */

    Time duration() const noexcept;

    std::vector<Clip*> activeClips(Time time);

    std::vector<const Clip*> activeClips(Time time) const;

private:
    SequenceId id_{0};

    std::string name_;

    SequenceSettings settings_{};

    /*
     * Sequence owns its tracks.
     */
    std::vector<std::unique_ptr<Track>> tracks_;
};

} // namespace beast

#endif // BEAST_SEQUENCE_H
