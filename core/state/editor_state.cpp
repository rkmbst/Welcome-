#include "editor_state.h"

namespace beast {

EditorState::EditorState(
    std::unique_ptr<Project> project
)
    : project_(std::move(project)) {
}

std::unique_ptr<Snapshot>
EditorState::createSnapshot() const {
    /*
     * Temporary deep-copy strategy for Core v0.1.
     *
     * This requires Project deep-cloning support.
     *
     * We deliberately leave the actual cloning implementation
     * to the Project domain model instead of performing ad-hoc
     * copies here.
     *
     * Until clone() exists, snapshot creation cannot succeed.
     */

    if (!project_) {
        return nullptr;
    }

    auto clonedProject = project_->clone();

    if (!clonedProject) {
        return nullptr;
    }

    return std::make_unique<Snapshot>(
        revision_,
        std::move(clonedProject)
    );
}

} // namespace beast
