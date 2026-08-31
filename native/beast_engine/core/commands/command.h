#ifndef BEAST_COMMAND_H
#define BEAST_COMMAND_H

#include <cstdint>

namespace beast {

class EditorContext;

enum class CommandResult : std::uint8_t {
    Success = 0,
    Failed,
    InvalidState,
    NotFound,
    AlreadyApplied,
    NotReversible
};

class Command {
public:
    Command() = default;
    virtual ~Command() = default;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    Command(Command&&) noexcept = default;
    Command& operator=(Command&&) noexcept = default;

    virtual CommandResult execute(
        EditorContext& context
    ) = 0;

    virtual CommandResult undo(
        EditorContext& context
    ) = 0;

    virtual const char* typeName() const noexcept = 0;

    bool isExecuted() const noexcept {
        return executed_;
    }

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
