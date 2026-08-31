#ifndef BEAST_TRACK_H
#define BEAST_TRACK_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "clip.h"

namespace beast {

/* ============================================================
 * Track Identity
 * ============================================================ */

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
 * Owns the clips placed on the timeline.
 *
 * Ownership model:
 *
 * Sequence
 *   └── owns Tracks
 *          └── owns Clips
 *                 └── references Asset by AssetId
 *
 * Important:
 * - Track owns Clip lifetime.
 * - Clip references an Asset; it does not own the Asset.
 * - Multiple Clips may reference the same Asset.
 * - Clips may overlap.
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

    /* --------------------------------------------------------
     * Ownership / Copying
     * -------------------------------------------------------- */

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
     * State
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

    bool addClip(
        std::unique_ptr<Clip> clip
    );

    bool removeClip(
        ClipId clipId
    );

    /*
     * Transfers ownership of a Clip from the Track
     * to the caller.
     *
     * Unlike removeClip(), this does NOT destroy the Clip.
     */
    std::unique_ptr<Clip> takeClip(
        ClipId clipId
    );

    /* --------------------------------------------------------
     * Clip Lookup
     * -------------------------------------------------------- */

    Clip* findClip(
        ClipId clipId
    ) noexcept;

    const Clip* findClip(
        ClipId clipId
    ) const noexcept;

    /* --------------------------------------------------------
     * Clip Access
     * -------------------------------------------------------- */

    const std::vector<std::unique_ptr<Clip>>& clips()
        const noexcept {
        return clips_;
    }

    std::size_t clipCount() const noexcept {
        return clips_.size();
    }

    /* --------------------------------------------------------
     * Timeline Queries
     * -------------------------------------------------------- */

    /*
     * Returns all enabled Clips active at the specified time.
     *
     * Interval semantics:
     *
     *     [start, end)
     *
     * The start is inclusive.
     * The end is exclusive.
     */
    std::vector<Clip*> activeClips(
        Time time
    );

    std::vector<const Clip*> activeClips(
        Time time
    ) const;

    /*
     * Returns the end of the furthest Clip on this Track.
     */
    Time duration() const noexcept;

    /* --------------------------------------------------------
     * Ordering
     * -------------------------------------------------------- */

    /*
     * Sort clips by timeline start.
     *
     * std::stable_sort preserves relative ordering for clips
     * beginning at the same time.
     */
    void sortClips();

    /* --------------------------------------------------------
     * Snapshot / Clone
     * -------------------------------------------------------- */

    /*
     * Creates an independent copy of this Track and all
     * Clips owned by it.
     *
     * Used by Snapshot / revision infrastructure.
     */
    std::unique_ptr<Track> clone() const;

private:
    TrackId id_{0};

    TrackType type_{TrackType::Unknown};

    std::string name_;

    bool enabled_{true};
    bool locked_{false};

    /*
     * Explicit ownership.
     *
     * unique_ptr gives us:
     * - deterministic lifetime
     * - no accidental copies
     * - stable object identity while stored
     * - clear ownership boundaries
     */
    std::vector<std::unique_ptr<Clip>> clips_;
};

} // namespace beast

#endif // BEAST_TRACK_H
