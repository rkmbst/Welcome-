#include "project.h"

#include <utility>

namespace beast {

Project::Project(
    ProjectId id,
    std::string name
)
    : id_(id),
      name_(std::move(name)) {
}

void Project::setName(std::string name) {
    if (name_ == name) {
        return;
    }

    name_ = std::move(name);

    markChanged();
}

} // namespace beast
