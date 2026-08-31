#include "project.h"

#include <algorithm>
#include <utility>

namespace beast {

Project::Project(
    ProjectId id,
    std::string name
)
    : id_(id),
      name_(std::move(name)) {
}

/* ============================================================
 * Name
 * ============================================================ */

void Project::setName(std::string name) {
    if (name_ == name) {
        return;
    }

    name_ = std::move(name);

    markChanged();
}

/* ============================================================
 * Settings
 * ============================================================ */

void Project::setSettings(
    const ProjectSettings& settings
) {
    settings_ = settings;

    markChanged();
}

/* ============================================================
 * Assets
 * ============================================================ */

bool Project::addAsset(
    std::unique_ptr<Asset> asset
) {
    if (!asset) {
        return false;
    }

    if (findAsset(asset->id()) != nullptr) {
        return false;
    }

    assets_.push_back(std::move(asset));

    markChanged();

    return true;
}

bool Project::removeAsset(
    AssetId assetId
) {
    const auto it = std::find_if(
        assets_.begin(),
        assets_.end(),
        [assetId](const std::unique_ptr<Asset>& asset) {
            return asset && asset->id() == assetId;
        }
    );

    if (it == assets_.end()) {
        return false;
    }

    /*
     * Important future rule:
     *
     * We will later validate whether another Clip still
     * references this Asset before allowing deletion.
     *
     * For Core v0.1 we keep that validation outside this
     * primitive and implement it in the higher editor command.
     */

    assets_.erase(it);

    markChanged();

    return true;
}

Asset* Project::findAsset(
    AssetId assetId
) noexcept {
    for (const auto& asset : assets_) {
        if (asset && asset->id() == assetId) {
            return asset.get();
        }
    }

    return nullptr;
}

const Asset* Project::findAsset(
    AssetId assetId
) const noexcept {
    for (const auto& asset : assets_) {
        if (asset && asset->id() == assetId) {
            return asset.get();
        }
    }

    return nullptr;
}

/* ============================================================
 * Sequences
 * ============================================================ */

bool Project::addSequence(
    std::unique_ptr<Sequence> sequence
) {
    if (!sequence) {
        return false;
    }

    if (findSequence(sequence->id()) != nullptr) {
        return false;
    }

    const bool firstSequence = sequences_.empty();

    sequences_.push_back(std::move(sequence));

    if (firstSequence) {
        activeSequenceId_ = sequences_.back()->id();
    }

    markChanged();

    return true;
}

bool Project::removeSequence(
    SequenceId sequenceId
) {
    const auto it = std::find_if(
        sequences_.begin(),
        sequences_.end(),
        [sequenceId](const std::unique_ptr<Sequence>& sequence) {
            return sequence &&
                   sequence->id() == sequenceId;
        }
    );

    if (it == sequences_.end()) {
        return false;
    }

    sequences_.erase(it);

    /*
     * If the active sequence was removed, choose another
     * existing sequence as the new active sequence.
     */
    if (activeSequenceId_ == sequenceId) {
        activeSequenceId_ =
            sequences_.empty()
                ? 0
                : sequences_.front()->id();
    }

    markChanged();

    return true;
}

Sequence* Project::findSequence(
    SequenceId sequenceId
) noexcept {
    for (const auto& sequence : sequences_) {
        if (sequence && sequence->id() == sequenceId) {
            return sequence.get();
        }
    }

    return nullptr;
}

const Sequence* Project::findSequence(
    SequenceId sequenceId
) const noexcept {
    for (const auto& sequence : sequences_) {
        if (sequence && sequence->id() == sequenceId) {
            return sequence.get();
        }
    }

    return nullptr;
}

/* ============================================================
 * Active Sequence
 * ============================================================ */

bool Project::setActiveSequence(
    SequenceId sequenceId
) {
    if (findSequence(sequenceId) == nullptr) {
        return false;
    }

    if (activeSequenceId_ == sequenceId) {
        return true;
    }

    activeSequenceId_ = sequenceId;

    markChanged();

    return true;
}

Sequence* Project::activeSequence() noexcept {
    if (activeSequenceId_ == 0) {
        return nullptr;
    }

    return findSequence(activeSequenceId_);
}

const Sequence* Project::activeSequence() const noexcept {
    if (activeSequenceId_ == 0) {
        return nullptr;
    }

    return findSequence(activeSequenceId_);
}

} // namespace beast
