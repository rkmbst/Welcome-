#ifndef BEAST_EDITOR_H
#define BEAST_EDITOR_H

#include <cstdint>
#include <memory>

#include "../commands/command_history.h"
#include "../state/editor_state.h"

namespace beast {

/* ============================================================
 * Editor
 *
 * High-level coordinator for the editor core.
 *
 * Responsibilities:
 * - Own the mutable EditorState
 * - Own CommandHistory
 * - Execute editor commands
 * - Provide snapshots for rendering/export
 *
 * It does NOT:
 * - decode video
 * - render GPU frames
 * - know about Flutter
 * - know about FFmpeg
 *
 * Those systems will consume the state/snapshots later.
 * ============================================================ */

class Editor final {
public:
    explicit Editor(
        std::unique_ptr<Project> project
    );

    ~Editor() = default;

    Editor(const Editor&) = delete;
    Editor& operator=(const Editor&) = delete;

    Editor(Editor&&) noexcept = default;
    Editor& operator=(Editor&&) noexcept = default;

    /* --------------------------------------------------------
     * State
     * -------------------------------------------------------- */

    EditorState& state() noexcept {
        return state_;
    }

    const EditorState& state() const noexcept {
        return state_;
    }

    /* --------------------------------------------------------
     * Commands
     * -------------------------------------------------------- */

    CommandResult execute(
        std::unique_ptr<Command> command
    );

    CommandResult undo();

    CommandResult redo();

    /* --------------------------------------------------------
     * History
     * -------------------------------------------------------- */

    const CommandHistory& history() const noexcept {
        return history_;
    }

    /* --------------------------------------------------------
     * Snapshot
     * -------------------------------------------------------- */

    std::unique_ptr<Snapshot> createSnapshot() const;

    /* --------------------------------------------------------
     * Revision
     * -------------------------------------------------------- */

    Revision revision() const noexcept {
        return state_.revision();
    }

    /* --------------------------------------------------------
     * Playhead
     * -------------------------------------------------------- */

    Time playhead() const noexcept {
        return state_.playhead();
    }

    void setPlayhead(Time time) noexcept {
        state_.setPlayhead(time);
    }

private:
    EditorState state_;

    CommandHistory history_;
};

} // namespace beast

#endif // BEAST_EDITOR_H
