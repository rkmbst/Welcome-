#include "command_history.h"

#include "../editor/editor_context.h"

namespace beast {

CommandResult CommandHistory::execute(
    std::unique_ptr<Command> command,
    EditorContext& context
) {
    if (!command) {
        return CommandResult::Failed;
    }

    const CommandResult result =
        command->execute(context);

    if (result != CommandResult::Success) {
        return result;
    }

    /*
     * A new command after Undo creates a new history branch.
     * Core v0.1 discards the old redo branch.
     */
    redoStack_.clear();

    undoStack_.push_back(
        std::move(command)
    );

    return CommandResult::Success;
}

CommandResult CommandHistory::undo(
    EditorContext& context
) {
    if (undoStack_.empty()) {
        return CommandResult::Failed;
    }

    auto& command = undoStack_.back();

    if (!command) {
        return CommandResult::Failed;
    }

    const CommandResult result =
        command->undo(context);

    if (result != CommandResult::Success) {
        return result;
    }

    redoStack_.push_back(
        std::move(command)
    );

    undoStack_.pop_back();

    return CommandResult::Success;
}

CommandResult CommandHistory::redo(
    EditorContext& context
) {
    if (redoStack_.empty()) {
        return CommandResult::Failed;
    }

    auto& command = redoStack_.back();

    if (!command) {
        return CommandResult::Failed;
    }

    const CommandResult result =
        command->execute(context);

    if (result != CommandResult::Success) {
        return result;
    }

    undoStack_.push_back(
        std::move(command)
    );

    redoStack_.pop_back();

    return CommandResult::Success;
}

void CommandHistory::clear() {
    undoStack_.clear();
    redoStack_.clear();
}

} // namespace beast
