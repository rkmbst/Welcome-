std::unique_ptr<Project> Project::clone() const {
    auto copy = std::make_unique<Project>(
        id_,
        name_
    );

    copy->settings_ = settings_;
    copy->activeSequenceId_ = activeSequenceId_;
    copy->revision_ = revision_;

    for (const auto& asset : assets_) {
        if (!asset) {
            continue;
        }

        /*
         * Asset clone will be implemented in the Asset model.
         */
        auto assetCopy = asset->clone();

        if (assetCopy) {
            copy->assets_.push_back(
                std::move(assetCopy)
            );
        }
    }

    for (const auto& sequence : sequences_) {
        if (!sequence) {
            continue;
        }

        auto sequenceCopy = sequence->clone();

        if (sequenceCopy) {
            copy->sequences_.push_back(
                std::move(sequenceCopy)
            );
        }
    }

    return copy;
}
