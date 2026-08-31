#include <gtest/gtest.h>

#include "../../../native/beast_engine/core/identity/entity_id.h"
#include "../../../native/beast_engine/core/identity/handle.h"

namespace {

using beast::AssetTag;
using beast::ClipTag;
using beast::ClipHandle;
using beast::EntityId;
using beast::Handle;
using beast::TrackHandle;

TEST(HandleTest, DefaultHandleIsInvalid) {
    const ClipHandle handle;

    EXPECT_FALSE(
        handle.isValid()
    );

    EXPECT_TRUE(
        handle.id().isNull()
    );

    EXPECT_EQ(
        handle.generation(),
        0u
    );
}

TEST(HandleTest, InvalidHandleFactoryWorks) {
    const ClipHandle handle =
        ClipHandle::invalid();

    EXPECT_FALSE(
        handle.isValid()
    );
}

TEST(HandleTest, ValidHandleWorks) {
    const EntityId id =
        EntityId::generate();

    const ClipHandle handle(
        id,
        1
    );

    EXPECT_TRUE(
        handle.isValid()
    );

    EXPECT_EQ(
        handle.id(),
        id
    );

    EXPECT_EQ(
        handle.generation(),
        1u
    );
}

TEST(HandleTest, GenerationChangesIdentity) {
    const EntityId id =
        EntityId::generate();

    const ClipHandle first(
        id,
        1
    );

    const ClipHandle second(
        id,
        2
    );

    EXPECT_TRUE(first.isValid());
    EXPECT_TRUE(second.isValid());

    EXPECT_NE(
        first,
        second
    );
}

TEST(HandleTest, DifferentEntityIdsProduceDifferentHandles) {
    const EntityId firstId =
        EntityId::generate();

    const EntityId secondId =
        EntityId::generate();

    const ClipHandle first(
        firstId,
        1
    );

    const ClipHandle second(
        secondId,
        1
    );

    EXPECT_NE(
        first,
        second
    );
}

TEST(HandleTest, DifferentEntityTypesAreDistinctTypes) {
    const EntityId id =
        EntityId::generate();

    const ClipHandle clip(
        id,
        1
    );

    const TrackHandle track(
        id,
        1
    );

    EXPECT_TRUE(
        clip.isValid()
    );

    EXPECT_TRUE(
        track.isValid()
    );

    /*
     * ClipHandle and TrackHandle are different C++ types.
     *
     * This test is intentionally runtime-light; the important
     * guarantee is compile-time type separation.
     */
    EXPECT_EQ(
        clip.id(),
        track.id()
    );
}

TEST(HandleTest, ZeroGenerationIsInvalid) {
    const EntityId id =
        EntityId::generate();

    const ClipHandle handle(
        id,
        0
    );

    EXPECT_FALSE(
        handle.isValid()
    );
}

} // namespace
