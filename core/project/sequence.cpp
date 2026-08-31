#include "sequence.h"

#include <algorithm>
#include <utility>

namespace beast {

Sequence::Sequence(
    SequenceId id,
    std::string name
)
    : id_(id),
      name_(std::move(name)) {
}

/* ============================================================
 * Name
 * ============================================================ */

void Sequence::setName(std::string name) {
    name_ = std::move(name);
}

/* ============================================================
 * Tracks
 * ============================================================ */

bool Sequence::addTrack(
    std::unique_ptr<Track> track
) {
    if (!track) {
        return false;
    }

    if (findTrack(track->id()) != nullptr) {
        return false;
    }

    tracks_.push_back(std::move(track));

    return true;
}

bool Sequence::removeTrack(
    TrackId trackId
) {
    const auto it = std::find_if(
        tracks_.begin(),
        tracks_.end(),
        [trackId](const std::unique_ptr<Track>& track) {
            return track && track->id() == trackId;
        }
    );

    if (it == tracks_.end()) {
        return false;
    }

    tracks_.erase(it);

    return true;
}

Track* Sequence::findTrack(
    TrackId trackId
) noexcept {
    for (const auto& track : tracks_) {
        if (track && track->id() == trackId) {
            return track.get();
        }
    }

    return nullptr;
}

const Track* Sequence::findTrack(
    TrackId trackId
) const noexcept {
    for (const auto& track : tracks_) {
        if (track && track->id() == trackId) {
            return track.get();
        }
    }

    return nullptr;
}

/* ============================================================
 * Timeline Queries
 * ============================================================ */

Time Sequence::duration() const noexcept {
    Time maximum = Time::zero();

    for (const auto& track : tracks_) {
        if (!track) {
            continue;
        }

        const Time trackDuration =
            track->duration();

        if (trackDuration > maximum) {
            maximum = trackDuration;
        }
    }

    return maximum;
}

std::vector<Clip*> Sequence::activeClips(
    Time time
) {
    std::vector<Clip*> result;

    for (const auto& track : tracks_) {
        if (!track || !track->enabled()) {
            continue;
        }

        auto active =
            track->activeClips(time);

        result.insert(
            result.end(),
            active.begin(),
            active.end()
        );
    }

    return result;
}

std::vector<const Clip*> Sequence::activeClips(
    Time time
) const {
    std::vector<const Clip*> result;

    for (const auto& track : tracks_) {
        if (!track || !track->enabled()) {
            continue;
        }

        auto active =
            track->activeClips(time);

        result.insert(
            result.end(),
            active.begin(),
            active.end()
        );
    }

    return result;
}

/* ============================================================
 * Clone
 *
 * Used by EditorState::createSnapshot().
 * ============================================================ */

std::unique_ptr<Sequence> Sequence::clone() const {
    auto copy = std::make_unique<Sequence>(
        id_,
        name_
    );

    copy->settings_ = settings_;

    for (const auto& track : tracks_) {
        if (!track) {
            continue;
        }

        auto trackCopy =
            track->clone();

        if (trackCopy) {
            copy->tracks_.push_back(
                std::move(trackCopy)
            );
        }
    }

    return copy;
}

} // namespace beast
