#include "document.h"

namespace beast {

Document::Document(
    std::unique_ptr<Project> project
)
    : project_(std::move(project)) {
}

} // namespace beast
