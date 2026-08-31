#include <cassert>
#include <iostream>
#include <memory>

#include "../../native/beast_engine/core/project/project.h"
#include "../../native/beast_engine/core/document/document.h"
#include "../../native/beast_engine/core/editor/editor.h"
#include "../../native/beast_engine/core/state/revision.h"

int main() {
    using namespace beast;

    auto project =
        std::make_unique<Project>(
            1,
            "Beast Test Project"
        );

    assert(project->id() == 1);
    assert(project->name() == "Beast Test Project");

    auto editor =
        std::make_unique<Editor>(
            std::move(project)
        );

    assert(editor->revision() == Revision::initial());

    auto snapshot =
        editor->createSnapshot();

    /*
     * Snapshot creation depends on the complete clone chain:
     *
     * Project
     *   -> Sequence
     *      -> Track
     *         -> Clip
     *
     * Once all clone APIs are present, this must succeed.
     */
    assert(snapshot != nullptr);
    assert(snapshot->valid());

    assert(
        snapshot->revision() ==
        editor->revision()
    );

    std::cout
        << "Beast Core smoke test: PASS\n";

    return 0;
}
