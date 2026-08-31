#include "editor.h"

namespace beast {

Editor::Editor(
    std::unique_ptr<Project> project
)
    : state_(std::move(project)),
      context_(state_) {
}

CommandResult Editor::execute(
    std::unique_ptr<Command> command
) {
    const CommandResult result =
        history_.execute(
            std::move(command),
            context_
        );

    if (result == CommandResult::Success) {
        state_.bumpRevision();
    }

    return result;
}

CommandResult Editor::undo() {
    const CommandResult result =
        history_.undo(context_);

    if (result == CommandResult::Success) {
        state_.bumpRevision();
    }

    return result;
}

CommandResult Editor::redo() {
    const CommandResult result =
        history_.redo(context_);

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
