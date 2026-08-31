#ifndef BEAST_EDITOR_STATE_H
#define BEAST_EDITOR_STATE_H

#include <memory>

#include "../document/document.h"
#include "../editor/editor_session.h"
#include "snapshot.h"

namespace beast {

/*
 * EditorState
 *
 * Runtime container joining:
 *
 *   Document + Session
 *
 * It is NOT itself the persistent document.
 */
class EditorState final {
public:
    explicit EditorState(
        std::unique_ptr<Document> document
    );

    ~EditorState() = default;

    EditorState(const EditorState&) = delete;
    EditorState& operator=(const EditorState&) = delete;

    EditorState(EditorState&&) noexcept = default;
    EditorState& operator=(EditorState&&) noexcept = default;

    /* --------------------------------------------------------
     * Document
     * -------------------------------------------------------- */

    Document& document() noexcept {
        return *document_;
    }

    const Document& document() const noexcept {
        return *document_;
    }

    bool valid() const noexcept {
        return document_ != nullptr;
    }

    /* --------------------------------------------------------
     * Session
     * -------------------------------------------------------- */

    EditorSession& session() noexcept {
        return session_;
    }

    const EditorSession& session() const noexcept {
        return session_;
    }

    /* --------------------------------------------------------
     * Revision
     * -------------------------------------------------------- */

    Revision revision() const noexcept {
        return document_->revision();
    }

    Revision bumpRevision() noexcept {
        return document_->advanceRevision();
    }

    /* --------------------------------------------------------
     * Snapshot
     * -------------------------------------------------------- */

    std::unique_ptr<Snapshot> createSnapshot() const;

private:
    std::unique_ptr<Document> document_;

    EditorSession session_;
};

} // namespace beast

#endif // BEAST_EDITOR_STATE_H
