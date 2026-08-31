#include <gtest/gtest.h>

#include "../../../native/beast_engine/core/identity/entity_id.h"
#include "../../../native/beast_engine/core/identity/handle.h"

TEST(EntityIdTest, GeneratedIdsAreValid) {
    const auto first =
        beast::EntityId::generate();

    const auto second =
        beast::EntityId::generate();

    EXPECT_FALSE(first.isNull());
    EXPECT_FALSE(second.isNull());
    EXPECT_NE(first, second);
}

TEST(EntityIdTest, StringRoundTripWorks) {
    const auto original =
        beast::EntityId::generate();

    const std::string text =
        original.toString();

    beast::EntityId parsed;

    ASSERT_TRUE(
        beast::EntityId::parse(
            text,
            parsed
        )
    );

    EXPECT_EQ(
        original,
        parsed
    );
}

TEST(HandleTest, InvalidHandleIsInvalid) {
    using ClipHandle =
        beast::Handle<beast::ClipTag>;

    const ClipHandle handle =
        ClipHandle::invalid();

    EXPECT_FALSE(
        handle.isValid()
    );
}

TEST(HandleTest, ValidHandleWorks) {
    const auto id =
        beast::EntityId::generate();

    using ClipHandle =
        beast::Handle<beast::ClipTag>;

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
        1
    );
}
