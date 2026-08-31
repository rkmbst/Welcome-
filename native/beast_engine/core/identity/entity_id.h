#ifndef BEAST_ENTITY_ID_H
#define BEAST_ENTITY_ID_H

#include <array>
#include <cstdint>
#include <string>

namespace beast {

/*
 * EntityId
 *
 * Stable 128-bit identity for objects that live inside a Beast
 * document/project.
 *
 * Why 128-bit?
 * - Extremely low collision probability for generated IDs.
 * - Stable across save/load.
 * - Suitable for plugins and external integrations.
 * - Does not depend on memory address or container position.
 *
 * The ID is data only. It does not imply object lifetime.
 */
class EntityId final {
public:
    constexpr EntityId() noexcept = default;

    constexpr EntityId(
        std::uint64_t high,
        std::uint64_t low
    ) noexcept
        : high_(high),
          low_(low) {
    }

    /*
     * Creates a random RFC 4122 version 4 UUID.
     */
    static EntityId generate() noexcept;

    /*
     * Null / invalid identity.
     */
    static constexpr EntityId null() noexcept {
        return EntityId{};
    }

    constexpr std::uint64_t high() const noexcept {
        return high_;
    }

    constexpr std::uint64_t low() const noexcept {
        return low_;
    }

    constexpr bool isNull() const noexcept {
        return high_ == 0 && low_ == 0;
    }

    /*
     * Canonical textual form:
     *
     * xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
     *
     * Useful for:
     * - debugging
     * - logs
     * - JSON
     * - external SDKs
     */
    std::string toString() const;

    /*
     * Parses the canonical 36-character UUID form.
     */
    static bool parse(
        const std::string& value,
        EntityId& out
    ) noexcept;

    /*
     * Raw bytes in network-order style.
     *
     * Useful later for:
     * - serialization
     * - hashing
     * - C ABI
     */
    std::array<std::uint8_t, 16> bytes() const noexcept;

    /* --------------------------------------------------------
     * Comparison
     * -------------------------------------------------------- */

    constexpr bool operator==(
        const EntityId& other
    ) const noexcept {
        return high_ == other.high_ &&
               low_ == other.low_;
    }

    constexpr bool operator!=(
        const EntityId& other
    ) const noexcept {
        return !(*this == other);
    }

    constexpr bool operator<(
        const EntityId& other
    ) const noexcept {
        if (high_ != other.high_) {
            return high_ < other.high_;
        }

        return low_ < other.low_;
    }

private:
    std::uint64_t high_{0};
    std::uint64_t low_{0};
};

} // namespace beast

#endif // BEAST_ENTITY_ID_H
