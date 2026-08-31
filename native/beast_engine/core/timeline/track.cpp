#include "track.h"

#include <algorithm>
#include <utility>

namespace beast {

/* ============================================================
 * Construction
 * ============================================================ */

Track::Track(
    TrackId id,
    TrackType type,
    std::string name
)
    : id_(id),
      type_(type),
      name_(std::move(name)) {
}

/* ============================================================
 * Name
 * ============================================================ */

void Track::setName(
    std::string name
) {
    name_ = std::move(name);
}

/* ============================================================
 * Add Clip
 * ============================================================ */

bool Track::addClip(
    std::unique_ptr<Clip> clip
) {
    if (!clip) {
        return false;
    }

    /*
     * A Track must not contain two Clips with the same ID.
     */
    if (findClip(clip->id()) != nullptr) {
        return false;
    }

    clips_.push_back(
        std::move(clip)
    );

    /*
     * Keep the Timeline ordered after insertion.
     */
    sortClips();

    return true;
}

/* ============================================================
 * Remove Clip
 * ============================================================ */

bool Track::removeClip(
    ClipId clipId
) {
    const auto it = std::find_if(
        clips_.begin(),
        clips_.end(),
        [clipId](const std::unique_ptr<Clip>& clip) {
            return clip &&
                   clip->id() == clipId;
        }
    );

    if (it == clips_.end()) {
        return false;
    }

    /*
     * unique_ptr destruction removes the Clip permanently.
     */
    clips_.erase(it);

    return true;
}

/* ============================================================
 * Take Clip
 * ============================================================ */

std::unique_ptr<Clip> Track::takeClip(
    ClipId clipId
) {
    const auto it = std::find_if(
        clips_.begin(),
        clips_.end(),
        [clipId](const std::unique_ptr<Clip>& clip) {
            return clip &&
                   clip->id() == clipId;
        }
    );

    if (it == clips_.end()) {
        return nullptr;
    }

    /*
     * Transfer ownership to the caller.
     */
    std::unique_ptr<Clip> result =
        std::move(*it);

    clips_.erase(it);

    return result;
}

/* ============================================================
 * Find Clip
 * ============================================================ */

Clip* Track::findClip(
    ClipId clipId
) noexcept {
    for (const auto& clip : clips_) {
        if (clip &&
            clip->id() == clipId) {
            return clip.get();
        }
    }

    return nullptr;
}

const Clip* Track::findClip(
    ClipId clipId
) const noexcept {
    for (const auto& clip : clips_) {
        if (clip &&
            clip->id() == clipId) {
            return clip.get();
        }
    }

    return nullptr;
}

/* ============================================================
 * Active Clips
 * ============================================================ */

std::vector<Clip*> Track::activeClips(
    Time time
) {
    std::vector<Clip*> result;

    /*
     * Core v0.1 uses a linear scan.
     *
     * This is intentionally simple.
     * Later, after profiling large projects, we can introduce
     * a temporal interval index without changing the public
     * API.
     */
    for (const auto& clip : clips_) {
        if (!clip) {
            continue;
        }

        if (!clip->enabled()) {
            continue;
        }

        const Time start =
            clip->timelineStart();

        const Time end =
            start +
            clip->timelineDuration();

        /*
         * Half-open interval:
         *
         * [start, end)
         *
         * This means:
         *
         * 5s <= time < 15s
         *
         * A Clip ending exactly at 15s is not active at 15s.
         */
        if (time >= start &&
            time < end) {
            result.push_back(
                clip.get()
            );
        }
    }

    return result;
}

std::vector<const Clip*> Track::activeClips(
    Time time
) const {
    std::vector<const Clip*> result;

    for (const auto& clip : clips_) {
        if (!clip) {
            continue;
        }

        if (!clip->enabled()) {
            continue;
        }

        const Time start =
            clip->timelineStart();

        const Time end =
            start +
            clip->timelineDuration();

        if (time >= start &&
            time < end) {
            result.push_back(
                clip.get()
            );
        }
    }

    return result;
}

/* ============================================================
 * Duration
 * ============================================================ */

Time Track::duration() const noexcept {
    Time maximum =
        Time::zero();

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

/* ============================================================
 * Ordering
 * ============================================================ */

void Track::sortClips() {
    std::stable_sort(
        clips_.begin(),
        clips_.end(),
        [](const std::unique_ptr<Clip>& lhs,
           const std::unique_ptr<Clip>& rhs) {

            /*
             * Null entries are not expected in normal operation,
             * but keeping the comparator defensive prevents
             * undefined behavior if an invalid state appears.
             */
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

/* ============================================================
 * Clone
 * ============================================================ */

std::unique_ptr<Track> Track::clone() const {
    auto copy =
        std::make_unique<Track>(
            id_,
            type_,
            name_
        );

    copy->enabled_ = enabled_;
    copy->locked_ = locked_;

    /*
     * Deep-copy the Clips owned by this Track.
     *
     * Each Clip keeps the same AssetId because Assets belong
     * to the Project and are cloned separately at Project level.
     */
    for (const auto& clip : clips_) {
        if (!clip) {
            continue;
        }

        auto clipCopy =
            clip->clone();

        if (clipCopy) {
            copy->clips_.push_back(
                std::move(clipCopy)
            );
        }
    }

    /*
     * The original Track is already ordered, and we preserve
     * that order in the clone.
     */
    return copy;
}

} // namespace beast
