#ifndef BEAST_ENTITY_REGISTRY_H
#define BEAST_ENTITY_REGISTRY_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <unordered_map>

#include "entity_id.h"
#include "handle.h"

namespace beast {

/*
 * EntityRegistry
 *
 * Central registry for runtime entity ownership and lookup.
 *
 * Important:
 * - EntityId is stable identity.
 * - Handle<T> is a temporary typed reference.
 * - Registry owns registered runtime objects only when explicitly
 *   registered through this API.
 *
 * Core v0.1 keeps the registry intentionally small.
 * Specialized registries/indexes can be introduced later
 * after profiling.
 */

/* ============================================================
 * Generation
 * ============================================================ */

using Generation = std::uint32_t;

/* ============================================================
 * Registry Entry
 * ============================================================ */

struct RegistryEntry final {
    EntityId id{EntityId::null()};
    Generation generation{0};
    void* object{nullptr};
};

/* ============================================================
 * Entity Registry
 * ============================================================ */

class EntityRegistry final {
public:
    EntityRegistry() = default;
    ~EntityRegistry() = default;

    EntityRegistry(const EntityRegistry&) = delete;
    EntityRegistry& operator=(const EntityRegistry&) = delete;

    EntityRegistry(EntityRegistry&&) noexcept = default;
    EntityRegistry& operator=(EntityRegistry&&) noexcept = default;

    /* --------------------------------------------------------
     * Identity
     * -------------------------------------------------------- */

    EntityId createId() const noexcept;

    /* --------------------------------------------------------
     * Registration
     * -------------------------------------------------------- */

    bool registerObject(
        EntityId id,
        void* object,
        Generation generation
    );

    bool unregisterObject(
        EntityId id
    );

    /* --------------------------------------------------------
     * Lookup
     * -------------------------------------------------------- */

    void* find(
        EntityId id
    ) noexcept;

    const void* find(
        EntityId id
    ) const noexcept;

    bool contains(
        EntityId id
    ) const noexcept;

    /* --------------------------------------------------------
     * Generation
     * -------------------------------------------------------- */

    Generation generation(
        EntityId id
    ) const noexcept;

    bool isAlive(
        EntityId id,
        Generation generation
    ) const noexcept;

    /* --------------------------------------------------------
     * Statistics
     * -------------------------------------------------------- */

    std::size_t size() const noexcept {
        return entries_.size();
    }

    bool empty() const noexcept {
        return entries_.empty();
    }

    /* --------------------------------------------------------
     * Clear
     * -------------------------------------------------------- */

    void clear() noexcept;

private:
    std::unordered_map<EntityId, RegistryEntry> entries_;
};

} // namespace beast

#endif // BEAST_ENTITY_REGISTRY_H
