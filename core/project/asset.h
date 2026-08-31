#ifndef BEAST_ASSET_H
#define BEAST_ASSET_H

#include <cstdint>
#include <memory>
#include <string>

#include "../time/time.h"

namespace beast {

/* ============================================================
 * Asset Identity
 * ============================================================ */

using AssetId = std::uint64_t;

/* ============================================================
 * Asset Type
 * ============================================================ */

enum class AssetType : std::uint8_t {
    Unknown = 0,
    Video,
    Audio,
    Image,
    Gif,
    Subtitle,
    Font,
    Sticker
};

/* ============================================================
 * Media Capabilities
 * ============================================================ */

struct AssetCapabilities final {
    bool canDecode{false};
    bool canPreview{false};
    bool canEdit{false};
    bool canExport{false};

    bool hasVideo{false};
    bool hasAudio{false};
    bool hasAlpha{false};

    bool hardwareDecodeSupported{false};
};

/* ============================================================
 * Media Metadata
 * ============================================================ */

struct AssetMetadata final {
    std::uint32_t width{0};
    std::uint32_t height{0};

    FrameRate frameRate{30, 1};

    Time duration{Time::zero()};

    std::string codec;
    std::string container;

    std::string colorSpace;
};

/* ============================================================
 * Asset
 * ============================================================ */

class Asset final {
public:
    Asset(
        AssetId id,
        AssetType type,
        std::string uri
    );

    ~Asset() = default;

    Asset(const Asset&) = delete;
    Asset& operator=(const Asset&) = delete;

    Asset(Asset&&) noexcept = default;
    Asset& operator=(Asset&&) noexcept = default;

    /* --------------------------------------------------------
     * Identity
     * -------------------------------------------------------- */

    AssetId id() const noexcept {
        return id_;
    }

    AssetType type() const noexcept {
        return type_;
    }

    /* --------------------------------------------------------
     * Source
     * -------------------------------------------------------- */

    const std::string& uri() const noexcept {
        return uri_;
    }

    void setUri(std::string uri);

    /* --------------------------------------------------------
     * Metadata
     * -------------------------------------------------------- */

    const AssetMetadata& metadata() const noexcept {
        return metadata_;
    }

    void setMetadata(const AssetMetadata& metadata) {
        metadata_ = metadata;
    }

    /* --------------------------------------------------------
     * Capabilities
     * -------------------------------------------------------- */

    const AssetCapabilities& capabilities() const noexcept {
        return capabilities_;
    }

    void setCapabilities(
        const AssetCapabilities& capabilities
    ) {
        capabilities_ = capabilities;
    }

    /* --------------------------------------------------------
     * Clone
     * -------------------------------------------------------- */

    std::unique_ptr<Asset> clone() const;

private:
    AssetId id_{0};

    AssetType type_{AssetType::Unknown};

    std::string uri_;

    AssetMetadata metadata_{};

    AssetCapabilities capabilities_{};
};

} // namespace beast

#endif // BEAST_ASSET_H
