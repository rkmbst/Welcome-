#include "track.h"

#include <algorithm>
#include <utility>

namespace beast {

Track::Track(
    TrackId id,
    TrackType type,
    std::string name
)
    : id_(id),
      type_(type),
      name_(std::move(name)) {
}

void Track::setName(std::string name) {
    name_ = std::move(name);
}

bool Track::addClip(std::unique_ptr<Clip> clip) {
    if (!clip) {
        return false;
    }

    /*
     * Do not allow duplicate Clip IDs inside the same Track.
     */
    if (findClip(clip->id()) != nullptr) {
        return false;
    }

    clips_.push_back(std::move(clip));

    sortClips();

    return true;
}

bool Track::removeClip(ClipId clipId) {
    const auto it = std::find_if(
        clips_.begin(),
        clips_.end(),
        [clipId](const std::unique_ptr<Clip>& clip) {
            return clip && clip->id() == clipId;
        }
    );

    if (it == clips_.end()) {
        return false;
    }

    clips_.erase(it);

    return true;
}

Clip* Track::findClip(ClipId clipId) noexcept {
    for (const auto& clip : clips_) {
        if (clip && clip->id() == clipId) {
            return clip.get();
        }
    }

    return nullptr;
}

const Clip* Track::findClip(ClipId clipId) const noexcept {
    for (const auto& clip : clips_) {
        if (clip && clip->id() == clipId) {
            return clip.get();
        }
    }

    return nullptr;
}

std::vector<Clip*> Track::activeClips(Time time) {
    std::vector<Clip*> result;

    for (const auto& clip : clips_) {
        if (!clip || !clip->enabled()) {
            continue;
        }

        const Time start = clip->timelineStart();
        const Time end = start + clip->timelineDuration();

        /*
         * Half-open interval:
         *
         * [start, end)
         *
         * This avoids ambiguity when one clip ends exactly
         * where another begins.
         */
        if (time >= start && time < end) {
            result.push_back(clip.get());
        }
    }

    return result;
}

std::vector<const Clip*> Track::activeClips(Time time) const {
    std::vector<const Clip*> result;

    for (const auto& clip : clips_) {
        if (!clip || !clip->enabled()) {
            continue;
        }

        const Time start = clip->timelineStart();
        const Time end = start + clip->timelineDuration();

        if (time >= start && time < end) {
            result.push_back(clip.get());
        }
    }

    return result;
}

Time Track::duration() const noexcept {
    Time maximum = Time::zero();

    for (const auto& clip : clips_) {
        if (!clip) {
            continue;
        }

        const Time end =
            clip->timelineStart() +
            clip->timelineDuration();

        if (end > maximum) {
            maximum = end;
        }
    }

    return maximum;
}

void Track::sortClips() {
    std::stable_sort(
        clips_.begin(),
        clips_.end(),
        [](const std::unique_ptr<Clip>& lhs,
           const std::unique_ptr<Clip>& rhs) {

            if (!lhs) {
                return false;
            }

            if (!rhs) {
                return true;
            }

            return lhs->timelineStart() <
                   rhs->timelineStart();
        }
    );
}

} // namespace beast
