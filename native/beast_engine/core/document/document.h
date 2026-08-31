#ifndef BEAST_DOCUMENT_H
#define BEAST_DOCUMENT_H

#include <cstdint>
#include <memory>

#include "../project/project.h"
#include "../state/revision.h"

namespace beast {

/*
 * Document
 *
 * Owns the logical project content.
 *
 * It does NOT own:
 * - playhead
 * - selection
 * - playback state
 * - UI state
 *
 * Those belong to the Editor Session.
 *
 * Document is the persistent editing model.
 */
class Document final {
public:
    explicit Document(
        std::unique_ptr<Project> project
    );

    ~Document() = default;

    Document(const Document&) = delete;
    Document& operator=(const Document&) = delete;

    Document(Document&&) noexcept = default;
    Document& operator=(Document&&) noexcept = default;

    /* --------------------------------------------------------
     * Project
     * -------------------------------------------------------- */

    Project& project() noexcept {
        return *project_;
    }

    const Project& project() const noexcept {
        return *project_;
    }

    bool valid() const noexcept {
        return project_ != nullptr;
    }

    /* --------------------------------------------------------
     * Revision
     * -------------------------------------------------------- */

    Revision revision() const noexcept {
        return revision_;
    }

    Revision advanceRevision() noexcept {
        revision_ = revision_.next();
        return revision_;
    }

    void setRevision(Revision revision) noexcept {
        revision_ = revision;
    }

private:
    std::unique_ptr<Project> project_;

    Revision revision_{
        Revision::initial()
    };
};

} // namespace beast

#endif // BEAST_DOCUMENT_H
