#include <gtest/gtest.h>

#include <string>

#include "../../../native/beast_engine/core/identity/entity_id.h"

namespace {

using beast::EntityId;

TEST(EntityIdTest, DefaultIdIsNull) {
    const EntityId id;

    EXPECT_TRUE(id.isNull());
    EXPECT_EQ(id.high(), 0u);
    EXPECT_EQ(id.low(), 0u);
}

TEST(EntityIdTest, NullIdIsStable) {
    const EntityId first = EntityId::null();
    const EntityId second = EntityId::null();

    EXPECT_TRUE(first.isNull());
    EXPECT_EQ(first, second);
}

TEST(EntityIdTest, GeneratedIdsAreNotNull) {
    const EntityId id = EntityId::generate();

    EXPECT_FALSE(id.isNull());
}

TEST(EntityIdTest, GeneratedIdsAreNormallyUnique) {
    constexpr int count = 1000;

    EntityId previous = EntityId::null();

    for (int i = 0; i < count; ++i) {
        const EntityId current = EntityId::generate();

        EXPECT_FALSE(current.isNull());

        if (!previous.isNull()) {
            EXPECT_NE(current, previous);
        }

        previous = current;
    }
}

TEST(EntityIdTest, StringRoundTrip) {
    const EntityId original =
        EntityId::generate();

    const std::string text =
        original.toString();

    EXPECT_EQ(text.size(), 36u);

    EntityId parsed;

    ASSERT_TRUE(
        EntityId::parse(
            text,
            parsed
        )
    );

    EXPECT_EQ(
        original,
        parsed
    );
}

TEST(EntityIdTest, InvalidStringIsRejected) {
    EntityId parsed;

    EXPECT_FALSE(
        EntityId::parse(
            "",
            parsed
        )
    );

    EXPECT_FALSE(
        EntityId::parse(
            "invalid-id",
            parsed
        )
    );
}

TEST(EntityIdTest, BytesAreSixteenBytes) {
    const EntityId id =
        EntityId::generate();

    const auto bytes =
        id.bytes();

    EXPECT_EQ(
        bytes.size(),
        16u
    );
}

} // namespace
