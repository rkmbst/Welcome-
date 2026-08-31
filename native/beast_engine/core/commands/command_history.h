#ifndef BEAST_COMMAND_HISTORY_H
#define BEAST_COMMAND_HISTORY_H

#include <cstddef>
#include <memory>
#include <vector>

#include "command.h"

namespace beast {

class EditorContext;

class CommandHistory final {
public:
    CommandHistory() = default;
    ~CommandHistory() = default;

    CommandHistory(const CommandHistory&) = delete;
    CommandHistory& operator=(const CommandHistory&) = delete;

    CommandResult execute(
        std::unique_ptr<Command> command,
        EditorContext& context
    );

    CommandResult undo(
        EditorContext& context
    );

    CommandResult redo(
        EditorContext& context
    );

    bool canUndo() const noexcept {
        return !undoStack_.empty();
    }

    bool canRedo() const noexcept {
        return !redoStack_.empty();
    }

    std::size_t undoCount() const noexcept {
        return undoStack_.size();
    }

    std::size_t redoCount() const noexcept {
        return redoStack_.size();
    }

    void clear();

private:
    std::vector<std::unique_ptr<Command>> undoStack_;
    std::vector<std::unique_ptr<Command>> redoStack_;
};

} // namespace beast

#endif // BEAST_COMMAND_HISTORY_H
