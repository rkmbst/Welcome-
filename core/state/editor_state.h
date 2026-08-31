#ifndef BEAST_EDITOR_STATE_H
#define BEAST_EDITOR_STATE_H

#include <cstdint>
#include <memory>

#include "../project/project.h"
#include "../time/time.h"
#include "revision.h"
#include "snapshot.h"

namespace beast {

/* ============================================================
 * Playback State
 * ============================================================ */

enum class PlaybackState : std::uint8_t {
    Stopped = 0,
    Playing,
    Paused,
    Seeking
};

/* ============================================================
 * Editor State
 *
 * Mutable state used by the editor controller.
 *
 * The Project contains persistent logical content.
 * EditorState additionally contains transient editor state:
 *
 * - playhead
 * - playback
 * - active sequence
 * - revision
 *
 * Renderer must not directly read this mutable object.
 * ============================================================ */

class EditorState final {
public:
    explicit EditorState(
        std::unique_ptr<Project> project
    );

    ~EditorState() = default;

    EditorState(const EditorState&) = delete;
    EditorState& operator=(const EditorState&) = delete;

    EditorState(EditorState&&) noexcept = default;
    EditorState& operator=(EditorState&&) noexcept = default;

    /* --------------------------------------------------------
     * Project
     * -------------------------------------------------------- */

    Project& project() noexcept {
        return *project_;
    }

    const Project& project() const noexcept {
        return *project_;
    }

    bool valid() const noexcept {
        return project_ != nullptr;
    }

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
     * Revision
     * -------------------------------------------------------- */

    Revision revision() const noexcept {
        return revision_;
    }

    Revision bumpRevision() noexcept {
        revision_ = revision_.next();
        return revision_;
    }

    /* --------------------------------------------------------
     * Snapshot
     * -------------------------------------------------------- */

    std::unique_ptr<Snapshot> createSnapshot() const;

private:
    std::unique_ptr<Project> project_;

    Time playhead_{Time::zero()};

    PlaybackState playbackState_{
        PlaybackState::Stopped
    };

    Revision revision_{
        Revision::initial()
    };
};

} // namespace beast

#endif // BEAST_EDITOR_STATE_H
