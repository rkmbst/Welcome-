#ifndef BEAST_SNAPSHOT_H
#define BEAST_SNAPSHOT_H

#include <cstdint>
#include <memory>

#include "../document/document.h"

namespace beast {

/*
 * Immutable rendering snapshot.
 *
 * Contains only information required by rendering/export
 * and a frozen copy of the Document.
 */
class Snapshot final {
public:
    Snapshot(
        Revision revision,
        std::unique_ptr<Project> project
    );

    ~Snapshot() = default;

    Snapshot(const Snapshot&) = delete;
    Snapshot& operator=(const Snapshot&) = delete;

    Snapshot(Snapshot&&) noexcept = default;
    Snapshot& operator=(Snapshot&&) noexcept = default;

    /* --------------------------------------------------------
     * Revision
     * -------------------------------------------------------- */

    Revision revision() const noexcept {
        return revision_;
    }

    /* --------------------------------------------------------
     * Project
     * -------------------------------------------------------- */

    const Project& project() const noexcept {
        return *project_;
    }

    bool valid() const noexcept {
        return project_ != nullptr;
    }

private:
    Revision revision_{};

    std::unique_ptr<Project> project_;
};

} // namespace beast

#endif // BEAST_SNAPSHOT_H
