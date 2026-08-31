#ifndef BEAST_REVISION_H
#define BEAST_REVISION_H

#include <cstdint>

namespace beast {

/*
 * Immutable logical version identifier for editor state.
 *
 * A revision changes whenever the logical project state changes.
 *
 * It is NOT a timestamp.
 * It is NOT a render frame number.
 */
class Revision final {
public:
    constexpr Revision() noexcept = default;

    explicit constexpr Revision(
        std::uint64_t value
    ) noexcept
        : value_(value) {}

    static constexpr Revision initial() noexcept {
        return Revision(0);
    }

    constexpr std::uint64_t value() const noexcept {
        return value_;
    }

    constexpr Revision next() const noexcept {
        return Revision(value_ + 1);
    }

    constexpr bool operator==(Revision other) const noexcept {
        return value_ == other.value_;
    }

    constexpr bool operator!=(Revision other) const noexcept {
        return value_ != other.value_;
    }

    constexpr bool operator<(Revision other) const noexcept {
        return value_ < other.value_;
    }

    constexpr bool operator>(Revision other) const noexcept {
        return value_ > other.value_;
    }

private:
    std::uint64_t value_{0};
};

} // namespace beast

#endif // BEAST_REVISION_H
