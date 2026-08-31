#ifndef BEAST_EDITOR_SESSION_H
#define BEAST_EDITOR_SESSION_H

#include <cstdint>

#include "../project/project.h"
#include "../time/time.h"

namespace beast {

/*
 * Session state is transient.
 *
 * It describes how the user is currently interacting with
 * the document.
 *
 * It must NOT become part of the saved project document.
 */

enum class PlaybackState : std::uint8_t {
    Stopped = 0,
    Playing,
    Paused,
    Seeking
};

class EditorSession final {
public:
    EditorSession() = default;

    /* --------------------------------------------------------
     * Playhead
     * -------------------------------------------------------- */

    Time playhead() const noexcept {
        return playhead_;
    }

    void setPlayhead(Time time) noexcept {
        playhead_ = time;
    }

    /* --------------------------------------------------------
     * Playback
     * -------------------------------------------------------- */

    PlaybackState playbackState() const noexcept {
        return playbackState_;
    }

    void setPlaybackState(
        PlaybackState state
    ) noexcept {
        playbackState_ = state;
    }

    /* --------------------------------------------------------
     * Active Sequence
     * -------------------------------------------------------- */

    SequenceId activeSequenceId() const noexcept {
        return activeSequenceId_;
    }

    void setActiveSequenceId(
        SequenceId sequenceId
    ) noexcept {
        activeSequenceId_ = sequenceId;
    }

private:
    Time playhead_{
        Time::zero()
    };

    PlaybackState playbackState_{
        PlaybackState::Stopped
    };

    SequenceId activeSequenceId_{0};
};

} // namespace beast

#endif // BEAST_EDITOR_SESSION_H
