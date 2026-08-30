#ifndef BEAST_TRACK_H
#define BEAST_TRACK_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "clip.h"

namespace beast {

using TrackId = std::uint64_t;

/* ============================================================
 * Track Type
 * ============================================================ */

enum class TrackType : std::uint8_t {
    Unknown = 0,
    Video,
    Audio,
    Subtitle,
    Overlay
};

/* ============================================================
 * Track
 *
 * Owns the Clips placed on this track.
 *
 * Important:
 * - A Track owns its Clips.
 * - A Clip references an Asset by AssetId.
 * - Overlapping Clips are allowed.
 * - Timeline ordering is maintained by timelineStart().
 * ============================================================ */

class Track final {
public:
    Track(
        TrackId id,
        TrackType type,
        std::string name
    );

    ~Track() = default;

    Track(const Track&) = delete;
    Track& operator=(const Track&) = delete;

    Track(Track&&) noexcept = default;
    Track& operator=(Track&&) noexcept = default;

    /* --------------------------------------------------------
     * Identity
     * -------------------------------------------------------- */

    TrackId id() const noexcept {
        return id_;
    }

    TrackType type() const noexcept {
        return type_;
    }

    /* --------------------------------------------------------
     * Name
     * -------------------------------------------------------- */

    const std::string& name() const noexcept {
        return name_;
    }

    void setName(std::string name);

    /* --------------------------------------------------------
     * Track State
     * -------------------------------------------------------- */

    bool enabled() const noexcept {
        return enabled_;
    }

    void setEnabled(bool enabled) noexcept {
        enabled_ = enabled;
    }

    bool locked() const noexcept {
        return locked_;
    }

    void setLocked(bool locked) noexcept {
        locked_ = locked;
    }

    /* --------------------------------------------------------
     * Clip Ownership
     * -------------------------------------------------------- */

    bool addClip(std::unique_ptr<Clip> clip);

    bool removeClip(ClipId clipId);

    Clip* findClip(ClipId clipId) noexcept;

    const Clip* findClip(ClipId clipId) const noexcept;

    /* --------------------------------------------------------
     * Clip Access
     * -------------------------------------------------------- */

    const std::vector<std::unique_ptr<Clip>>& clips() const noexcept {
        return clips_;
    }

    std::size_t clipCount() const noexcept {
        return clips_.size();
    }

    /* --------------------------------------------------------
     * Timeline Queries
     * -------------------------------------------------------- */

    std::vector<Clip*> activeClips(Time time);

    std::vector<const Clip*> activeClips(Time time) const;

    Time duration() const noexcept;

    /* --------------------------------------------------------
     * Ordering
     * -------------------------------------------------------- */

    void sortClips();

private:
    TrackId id_{0};

    TrackType type_{TrackType::Unknown};

    std::string name_;

    bool enabled_{true};
    bool locked_{false};

    /*
     * Track owns its clips.
     *
     * unique_ptr gives us:
     * - explicit ownership
     * - stable Clip addresses
     * - no accidental copying
     * - clean lifetime management
     */
    std::vector<std::unique_ptr<Clip>> clips_;
};

} // namespace beast

#endif // BEAST_TRACK_H
