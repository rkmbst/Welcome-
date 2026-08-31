#ifndef BEAST_COMMAND_HISTORY_H
#define BEAST_COMMAND_HISTORY_H

#include <cstddef>
#include <memory>
#include <vector>

#include "command.h"

namespace beast {

/* ============================================================
 * Command History
 *
 * Owns executed commands and manages Undo / Redo.
 *
 * History model:
 *
 *              Execute
 *                 ↓
 *         ┌───────────────┐
 *         │ Undo History  │
 *         └───────┬───────┘
 *                 │
 *                Undo
 *                 ↓
 *         ┌───────────────┐
 *         │ Redo History  │
 *         └───────────────┘
 *
 * Any new command after Undo invalidates the Redo branch.
 * Branching history can be introduced later without changing
 * the Command abstraction.
 * ============================================================ */

class CommandHistory final {
public:
    CommandHistory() = default;

    ~CommandHistory() = default;

    CommandHistory(const CommandHistory&) = delete;
    CommandHistory& operator=(const CommandHistory&) = delete;

    /* --------------------------------------------------------
     * Execute
     * -------------------------------------------------------- */

    CommandResult execute(
        std::unique_ptr<Command> command
    );

    /* --------------------------------------------------------
     * Undo / Redo
     * -------------------------------------------------------- */

    CommandResult undo();

    CommandResult redo();

    /* --------------------------------------------------------
     * State
     * -------------------------------------------------------- */

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

    /* --------------------------------------------------------
     * Clear
     * -------------------------------------------------------- */

    void clear();

private:
    std::vector<std::unique_ptr<Command>> undoStack_;

    std::vector<std::unique_ptr<Command>> redoStack_;
};

} // namespace beast

#endif // BEAST_COMMAND_HISTORY_H
