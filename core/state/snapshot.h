#ifndef BEAST_SNAPSHOT_H
#define BEAST_SNAPSHOT_H

#include <cstdint>
#include <memory>

#include "../project/project.h"
#include "revision.h"

namespace beast {

/*
 * Immutable view of editor state.
 *
 * Renderer / Preview / Export must consume snapshots,
 * not the live mutable Project.
 *
 * Core v0.1:
 * Snapshot owns an independent Project copy through deep
 * cloning. We will later optimize this using structural
 * sharing / copy-on-write where profiling proves useful.
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
     *
     * Read-only access only.
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
