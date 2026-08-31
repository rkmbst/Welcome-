#ifndef BEAST_COMMAND_H
#define BEAST_COMMAND_H

#include <cstdint>
#include <string>

namespace beast {

/* ============================================================
 * Command Result
 * ============================================================ */

enum class CommandResult : std::uint8_t {
    Success = 0,
    Failed,
    InvalidState,
    NotFound,
    AlreadyApplied,
    NotReversible
};

/* ============================================================
 * Command
 *
 * Every logical editor modification should eventually pass
 * through a Command.
 *
 * Examples:
 *
 *   AddClipCommand
 *   RemoveClipCommand
 *   MoveClipCommand
 *   SplitClipCommand
 *   SetPropertyCommand
 *   AddEffectCommand
 *
 * A Command changes editor state and knows how to undo itself.
 * ============================================================ */

class Command {
public:
    Command() = default;
    virtual ~Command() = default;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    Command(Command&&) noexcept = default;
    Command& operator=(Command&&) noexcept = default;

    /* --------------------------------------------------------
     * Execution
     * -------------------------------------------------------- */

    virtual CommandResult execute() = 0;

    virtual CommandResult undo() = 0;

    /* --------------------------------------------------------
     * State
     * -------------------------------------------------------- */

    bool executed() const noexcept {
        return executed_;
    }

    /* --------------------------------------------------------
     * Identification
     * -------------------------------------------------------- */

    virtual const char* typeName() const noexcept = 0;

protected:
    void markExecuted() noexcept {
        executed_ = true;
    }

    void markUndone() noexcept {
        executed_ = false;
    }

private:
    bool executed_{false};
};

} // namespace beast

#endif // BEAST_COMMAND_H
