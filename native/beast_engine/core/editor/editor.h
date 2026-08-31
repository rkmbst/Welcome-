#ifndef BEAST_EDITOR_H
#define BEAST_EDITOR_H

#include <memory>

#include "../commands/command_history.h"
#include "../state/editor_state.h"
#include "editor_context.h"

namespace beast {

class Editor final {
public:
    explicit Editor(
        std::unique_ptr<Project> project
    );

    ~Editor() = default;

    Editor(const Editor&) = delete;
    Editor& operator=(const Editor&) = delete;

    Editor(Editor&&) noexcept = delete;
    Editor& operator=(Editor&&) noexcept = delete;

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
     * Snapshot
     * -------------------------------------------------------- */

    std::unique_ptr<Snapshot> createSnapshot() const;

    /* --------------------------------------------------------
     * Revision
     * -------------------------------------------------------- */

    Revision revision() const noexcept {
        return state_.revision();
    }

private:
    EditorState state_;

    EditorContext context_;

    CommandHistory history_;
};

} // namespace beast

#endif // BEAST_EDITOR_H
