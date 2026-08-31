#include "add_clip_command.h"

#include "../editor/editor_context.h"
#include "../state/editor_state.h"
#include "../project/project.h"
#include "../project/sequence.h"
#include "../timeline/track.h"

namespace beast {

AddClipCommand::AddClipCommand(
    TrackId trackId,
    std::unique_ptr<Clip> clip
)
    : trackId_(trackId),
      clip_(std::move(clip)) {
}

CommandResult AddClipCommand::execute(
    EditorContext& context
) {
    if (isExecuted()) {
        return CommandResult::AlreadyApplied;
    }

    if (!clip_) {
        return CommandResult::InvalidState;
    }

    EditorState& state = context.state();

    if (!state.valid()) {
        return CommandResult::InvalidState;
    }

    Project& project = state.document().project();

    /*
     * Use the currently active sequence.
     */
    Sequence* sequence =
        project.activeSequence();

    if (!sequence) {
        return CommandResult::NotFound;
    }

    Track* track =
        sequence->findTrack(trackId_);

    if (!track) {
        return CommandResult::NotFound;
    }

    /*
     * Locked tracks cannot be modified.
     */
    if (track->locked()) {
        return CommandResult::InvalidState;
    }

    /*
     * Track takes ownership of the Clip.
     */
    if (!track->addClip(
            std::move(clip_)
        )) {
        return CommandResult::Failed;
    }

    markExecuted();

    return CommandResult::Success;
}

CommandResult AddClipCommand::undo(
    EditorContext& context
) {
    if (!isExecuted()) {
        return CommandResult::AlreadyApplied;
    }

    EditorState& state = context.state();

    if (!state.valid()) {
        return CommandResult::InvalidState;
    }

    Project& project = state.document().project();

    Sequence* sequence =
        project.activeSequence();

    if (!sequence) {
        return CommandResult::NotFound;
    }

    Track* track =
        sequence->findTrack(trackId_);

    if (!track) {
        return CommandResult::NotFound;
    }

    /*
     * Find the Clip before removing it.
     */
    Clip* clip =
        track->findClip(
            clip_->id()
        );

    /*
     * clip_ is currently null because Track owns it.
     *
     * Therefore we cannot use clip_ here.
     *
     * The command needs the ClipId independently.
     */
    return CommandResult::Failed;
}

} // namespace beast
