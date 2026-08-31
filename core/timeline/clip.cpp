#include "clip.h"

#include <algorithm>
#include <memory>

namespace beast {

Clip::Clip(
    ClipId id,
    AssetId assetId
)
    : id_(id),
      assetId_(assetId) {
}

void Clip::setSpeed(double speed) noexcept {
    /*
     * Speed must always be positive.
     *
     * Reverse playback will later use a dedicated
     * playback-direction/range model instead of a
     * negative speed value.
     */
    speed_ = std::max(0.01, speed);
}

std::unique_ptr<Clip> Clip::clone() const {
    auto copy = std::make_unique<Clip>(
        id_,
        assetId_
    );

    copy->timelineStart_ = timelineStart_;
    copy->timelineDuration_ = timelineDuration_;

    copy->sourceStart_ = sourceStart_;
    copy->sourceDuration_ = sourceDuration_;

    copy->speed_ = speed_;
    copy->enabled_ = enabled_;

    return copy;
}

} // namespace beast
