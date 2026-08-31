#include "editor_state.h"

namespace beast {

EditorState::EditorState(
    std::unique_ptr<Document> document
)
    : document_(std::move(document)) {
}

std::unique_ptr<Snapshot>
EditorState::createSnapshot() const {
    if (!document_) {
        return nullptr;
    }

    auto projectCopy =
        document_->project().clone();

    if (!projectCopy) {
        return nullptr;
    }

    return std::make_unique<Snapshot>(
        document_->revision(),
        std::move(projectCopy)
    );
}

} // namespace beast
