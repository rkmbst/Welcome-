#include "editor.h"

namespace beast {

Editor::Editor(
    std::unique_ptr<Project> project
)
    : state_(std::move(project)) {
}

CommandResult Editor::execute(
    std::unique_ptr<Command> command
) {
    const CommandResult result =
        history_.execute(std::move(command));

    if (result == CommandResult::Success) {
        /*
         * A successful logical edit creates a new revision.
         *
         * Playback-only state changes such as playhead movement
         * do not necessarily need to create a project revision.
         */
        state_.bumpRevision();
    }

    return result;
}

CommandResult Editor::undo() {
    const CommandResult result =
        history_.undo();

    if (result == CommandResult::Success) {
        state_.bumpRevision();
    }

    return result;
}

CommandResult Editor::redo() {
    const CommandResult result =
        history_.redo();

    if (result == CommandResult::Success) {
        state_.bumpRevision();
    }

    return result;
}

std::unique_ptr<Snapshot>
Editor::createSnapshot() const {
    return state_.createSnapshot();
}

} // namespace beast
