#ifndef BEAST_HANDLE_H
#define BEAST_HANDLE_H

#include <cstdint>
#include <type_traits>

#include "entity_id.h"

namespace beast {

/*
 * Handle<T>
 *
 * A lightweight, non-owning reference to an Entity.
 *
 * IMPORTANT:
 * A Handle does not keep the object alive.
 *
 * It contains:
 * - stable EntityId
 * - generation
 *
 * Generation allows a future ObjectRegistry to detect stale
 * handles when an entity is destroyed and its storage is reused.
 *
 * Example:
 *
 * Handle<ClipTag>
 *     EntityId = ABC...
 *     generation = 4
 *
 * After deletion/reuse:
 *
 * generation = 5
 *
 * Old handle becomes stale.
 */
template <typename Tag>
class Handle final {
public:
    using TagType = Tag;

    constexpr Handle() noexcept = default;

    constexpr Handle(
        EntityId id,
        std::uint32_t generation
    ) noexcept
        : id_(id),
          generation_(generation) {
    }

    static constexpr Handle invalid() noexcept {
        return Handle{};
    }

    constexpr EntityId id() const noexcept {
        return id_;
    }

    constexpr std::uint32_t generation() const noexcept {
        return generation_;
    }

    constexpr bool isValid() const noexcept {
        return !id_.isNull() &&
               generation_ != 0;
    }

    constexpr bool operator==(
        const Handle& other
    ) const noexcept {
        return id_ == other.id_ &&
               generation_ == other.generation_;
    }

    constexpr bool operator!=(
        const Handle& other
    ) const noexcept {
        return !(*this == other);
    }

    constexpr bool operator<(
        const Handle& other
    ) const noexcept {
        if (id_ != other.id_) {
            return id_ < other.id_;
        }

        return generation_ < other.generation_;
    }

private:
    EntityId id_{};

    /*
     * Generation 0 is reserved for invalid handles.
     */
    std::uint32_t generation_{0};
};

/* ============================================================
 * Entity Tags
 *
 * These are empty types used purely for compile-time safety.
 * ============================================================ */

struct ProjectTag {};
struct SequenceTag {};
struct TrackTag {};
struct ClipTag {};
struct AssetTag {};
struct MarkerTag {};
struct KeyframeTag {};
struct EffectTag {};
struct TransitionTag {};
struct MediaSourceTag {};

/* ============================================================
 * Strongly typed handles
 * ============================================================ */

using ProjectHandle =
    Handle<ProjectTag>;

using SequenceHandle =
    Handle<SequenceTag>;

using TrackHandle =
    Handle<TrackTag>;

using ClipHandle =
    Handle<ClipTag>;

using AssetHandle =
    Handle<AssetTag>;

using MarkerHandle =
    Handle<MarkerTag>;

using KeyframeHandle =
    Handle<KeyframeTag>;

using EffectHandle =
    Handle<EffectTag>;

using TransitionHandle =
    Handle<TransitionTag>;

using MediaSourceHandle =
    Handle<MediaSourceTag>;

} // namespace beast

#endif // BEAST_HANDLE_H
