#include "command_history.h"

namespace beast {

CommandResult CommandHistory::execute(
    std::unique_ptr<Command> command
) {
    if (!command) {
        return CommandResult::Failed;
    }

    const CommandResult result =
        command->execute();

    if (result != CommandResult::Success) {
        return result;
    }

    /*
     * Once a new command is executed after an Undo,
     * the old Redo branch is no longer valid.
     */
    redoStack_.clear();

    undoStack_.push_back(
        std::move(command)
    );

    return CommandResult::Success;
}

CommandResult CommandHistory::undo() {
    if (undoStack_.empty()) {
        return CommandResult::Failed;
    }

    std::unique_ptr<Command>& command =
        undoStack_.back();

    if (!command) {
        return CommandResult::Failed;
    }

    const CommandResult result =
        command->undo();

    if (result != CommandResult::Success) {
        return result;
    }

    redoStack_.push_back(
        std::move(command)
    );

    undoStack_.pop_back();

    return CommandResult::Success;
}

CommandResult CommandHistory::redo() {
    if (redoStack_.empty()) {
        return CommandResult::Failed;
    }

    std::unique_ptr<Command>& command =
        redoStack_.back();

    if (!command) {
        return CommandResult::Failed;
    }

    const CommandResult result =
        command->execute();

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
