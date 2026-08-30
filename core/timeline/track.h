#ifndef BEAST_TRACK_H
#define BEAST_TRACK_H

#include <cstdint>
#include <string>
#include <vector>

namespace beast {

using TrackId = std::uint64_t;

enum class TrackType : std::uint8_t {
    Unknown = 0,
    Video,
    Audio,
    Subtitle,
    Overlay
};

class Track final {
public:
    Track(
        TrackId id,
        TrackType type,
        std::string name
    );

    ~Track() = default;

    Track(const Track&) = delete;
    Track& operator=(const Track&) = delete;

    Track(Track&&) noexcept = default;
    Track& operator=(Track&&) noexcept = default;

    TrackId id() const noexcept {
        return id_;
    }

    TrackType type() const noexcept {
        return type_;
    }

    const std::string& name() const noexcept {
        return name_;
    }

    void setName(std::string name);

    bool enabled() const noexcept {
        return enabled_;
    }

    void setEnabled(bool enabled) noexcept {
        enabled_ = enabled;
    }

    bool locked() const noexcept {
        return locked_;
    }

    void setLocked(bool locked) noexcept {
        locked_ = locked;
    }

private:
    TrackId id_{0};
    TrackType type_{TrackType::Unknown};
    std::string name_;

    bool enabled_{true};
    bool locked_{false};
};

} // namespace beast

#endif // BEAST_TRACK_H
