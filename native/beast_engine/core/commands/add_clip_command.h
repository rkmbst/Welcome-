#ifndef BEAST_ADD_CLIP_COMMAND_H
#define BEAST_ADD_CLIP_COMMAND_H

#include <memory>

#include "command.h"

#include "../timeline/clip.h"

namespace beast {

/*
 * AddClipCommand
 *
 * Adds a Clip to a Track.
 *
 * The command owns the Clip while it is not attached to the
 * Track, and releases ownership to the Track after execute().
 *
 * During undo(), ownership is transferred back into the
 * command so redo() can safely execute the same command again.
 */
class AddClipCommand final : public Command {
public:
    AddClipCommand(
        TrackId trackId,
        std::unique_ptr<Clip> clip
    );

    ~AddClipCommand() override = default;

    AddClipCommand(const AddClipCommand&) = delete;
    AddClipCommand& operator=(const AddClipCommand&) = delete;

    AddClipCommand(AddClipCommand&&) noexcept = default;
    AddClipCommand& operator=(AddClipCommand&&) noexcept = default;

    CommandResult execute(
        EditorContext& context
    ) override;

    CommandResult undo(
        EditorContext& context
    ) override;

    const char* typeName() const noexcept override {
        return "AddClipCommand";
    }

private:
    TrackId trackId_{0};

    ClipId clipId_{0};

    std::unique_ptr<Clip> clip_;
};

} // namespace beast

#endif // BEAST_ADD_CLIP_COMMAND_H
