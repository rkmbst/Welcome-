#include "asset.h"

#include <memory>
#include <utility>

namespace beast {

Asset::Asset(
    AssetId id,
    AssetType type,
    std::string uri
)
    : id_(id),
      type_(type),
      uri_(std::move(uri)) {
}

void Asset::setUri(std::string uri) {
    uri_ = std::move(uri);
}

std::unique_ptr<Asset> Asset::clone() const {
    auto copy = std::make_unique<Asset>(
        id_,
        type_,
        uri_
    );

    copy->metadata_ = metadata_;
    copy->capabilities_ = capabilities_;

    return copy;
}

} // namespace beast
