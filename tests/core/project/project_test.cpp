#include <cassert>
#include <memory>

#include "../../../native/beast_engine/core/project/asset.h"
#include "../../../native/beast_engine/core/project/project.h"

int main() {
    using namespace beast;

    auto project =
        std::make_unique<Project>(
            1,
            "Test Project"
        );

    assert(project->id() == 1);
    assert(project->name() == "Test Project");
    assert(project->assetCount() == 0);

    auto asset =
        std::make_unique<Asset>(
            100,
            AssetType::Video,
            "video.mp4"
        );

    assert(project->addAsset(
        std::move(asset)
    ));

    assert(project->assetCount() == 1);

    Asset* found =
        project->findAsset(100);

    assert(found != nullptr);
    assert(found->type() == AssetType::Video);

    /*
     * Duplicate IDs must be rejected.
     */
    auto duplicate =
        std::make_unique<Asset>(
            100,
            AssetType::Video,
            "other.mp4"
        );

    assert(
        !project->addAsset(
            std::move(duplicate)
        )
    );

    assert(project->assetCount() == 1);

    return 0;
}
