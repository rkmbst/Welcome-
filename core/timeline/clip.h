#ifndef BEAST_CLIP_H
#define BEAST_CLIP_H

#include <cstdint>
#include <memory>

#include "../project/asset.h"
#include "../time/time.h"

namespace beast {

using ClipId = std::uint64_t;

/* ============================================================
 * Clip
 * ============================================================ */

class Clip final {
public:
    Clip(
        ClipId id,
        AssetId assetId
    );

    ~Clip() = default;

    Clip(const Clip&) = delete;
    Clip& operator=(const Clip&) = delete;

    Clip(Clip&&) noexcept = default;
    Clip& operator=(Clip&&) noexcept = default;

    /* --------------------------------------------------------
     * Identity
     * -------------------------------------------------------- */

    ClipId id() const noexcept {
        return id_;
    }

    AssetId assetId() const noexcept {
        return assetId_;
    }

    /* --------------------------------------------------------
     * Timeline Position
     * -------------------------------------------------------- */

    Time timelineStart() const noexcept {
        return timelineStart_;
    }

    Time timelineDuration() const noexcept {
        return timelineDuration_;
    }

    void setTimelineStart(Time start) noexcept {
        timelineStart_ = start;
    }

    void setTimelineDuration(Time duration) noexcept {
        timelineDuration_ = duration;
    }

    /* --------------------------------------------------------
     * Source Range
     *
     * This is the portion of the original Asset used by
     * this Clip.
     * -------------------------------------------------------- */

    Time sourceStart() const noexcept {
        return sourceStart_;
    }

    Time sourceDuration() const noexcept {
        return sourceDuration_;
    }

    void setSourceStart(Time start) noexcept {
        sourceStart_ = start;
    }

    void setSourceDuration(Time duration) noexcept {
        sourceDuration_ = duration;
    }

    /* --------------------------------------------------------
     * Playback
     * -------------------------------------------------------- */

    double speed() const noexcept {
        return speed_;
    }

    void setSpeed(double speed) noexcept;

    /* --------------------------------------------------------
     * Basic State
     * -------------------------------------------------------- */

    bool enabled() const noexcept {
        return enabled_;
    }

    void setEnabled(bool enabled) noexcept {
        enabled_ = enabled;
    }

    /* --------------------------------------------------------
     * Clone
     * -------------------------------------------------------- */

    std::unique_ptr<Clip> clone() const;

private:
    ClipId id_{0};

    AssetId assetId_{0};

    /* Timeline coordinates */
    Time timelineStart_{Time::zero()};
    Time timelineDuration_{Time::zero()};

    /* Source coordinates */
    Time sourceStart_{Time::zero()};
    Time sourceDuration_{Time::zero()};

    double speed_{1.0};

    bool enabled_{true};
};

} // namespace beast

#endif // BEAST_CLIP_H
