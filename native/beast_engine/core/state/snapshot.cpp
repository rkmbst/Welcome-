#include "snapshot.h"

namespace beast {

Snapshot::Snapshot(
    Revision revision,
    std::unique_ptr<Project> project
)
    : revision_(revision),
      project_(std::move(project)) {
}

} // namespace beast
