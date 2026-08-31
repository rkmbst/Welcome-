#include "entity_id.h"

#include <array>
#include <charconv>
#include <cctype>
#include <cstdio>
#include <random>
#include <string>

namespace beast {

/* ============================================================
 * Generate UUID v4
 * ============================================================ */

EntityId EntityId::generate() noexcept {
    std::random_device randomDevice;

    std::uint64_t high =
        (static_cast<std::uint64_t>(randomDevice()) << 32) |
        static_cast<std::uint64_t>(randomDevice());

    std::uint64_t low =
        (static_cast<std::uint64_t>(randomDevice()) << 32) |
        static_cast<std::uint64_t>(randomDevice());

    /*
     * RFC 4122:
     *
     * Version 4:
     * bits 12-15 of time_hi_and_version = 0100
     *
     * Variant 1:
     * bits 6-7 of clock_seq_hi_and_reserved = 10
     */

    high &= 0xffffffffffff0fffULL;
    high |= 0x0000000000004000ULL;

    low &= 0x3fffffffffffffffULL;
    low |= 0x8000000000000000ULL;

    /*
     * Null ID is reserved.
     */
    if (high == 0 && low == 0) {
        low = 1;
    }

    return EntityId(high, low);
}

/* ============================================================
 * Bytes
 * ============================================================ */

std::array<std::uint8_t, 16>
EntityId::bytes() const noexcept {
    return {
        static_cast<std::uint8_t>((high_ >> 56) & 0xff),
        static_cast<std::uint8_t>((high_ >> 48) & 0xff),
        static_cast<std::uint8_t>((high_ >> 40) & 0xff),
        static_cast<std::uint8_t>((high_ >> 32) & 0xff),
        static_cast<std::uint8_t>((high_ >> 24) & 0xff),
        static_cast<std::uint8_t>((high_ >> 16) & 0xff),
        static_cast<std::uint8_t>((high_ >> 8) & 0xff),
        static_cast<std::uint8_t>(high_ & 0xff),

        static_cast<std::uint8_t>((low_ >> 56) & 0xff),
        static_cast<std::uint8_t>((low_ >> 48) & 0xff),
        static_cast<std::uint8_t>((low_ >> 40) & 0xff),
        static_cast<std::uint8_t>((low_ >> 32) & 0xff),
        static_cast<std::uint8_t>((low_ >> 24) & 0xff),
        static_cast<std::uint8_t>((low_ >> 16) & 0xff),
        static_cast<std::uint8_t>((low_ >> 8) & 0xff),
        static_cast<std::uint8_t>(low_ & 0xff)
    };
}

/* ============================================================
 * To String
 * ============================================================ */

std::string EntityId::toString() const {
    const auto data = bytes();

    char buffer[37]{};

    std::snprintf(
        buffer,
        sizeof(buffer),
        "%02x%02x%02x%02x-"
        "%02x%02x-"
        "%02x%02x-"
        "%02x%02x-"
        "%02x%02x%02x%02x%02x%02x",
        data[0],
        data[1],
        data[2],
        data[3],
        data[4],
        data[5],
        data[6],
        data[7],
        data[8],
        data[9],
        data[10],
        data[11],
        data[12],
        data[13],
        data[14],
        data[15]
    );

    return std::string(buffer);
}

/* ============================================================
 * Parse
 * ============================================================ */

bool EntityId::parse(
    const std::string& value,
    EntityId& out
) noexcept {
    if (value.size() != 36) {
        return false;
    }

    /*
     * Hyphen positions for canonical UUID.
     */
    if (value[8] != '-' ||
        value[13] != '-' ||
        value[18] != '-' ||
        value[23] != '-') {
        return false;
    }

    std::array<std::uint8_t, 16> bytes{};

    std::size_t byteIndex = 0;

    auto hexValue = [](char c) -> int {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }

        if (c >= 'a' && c <= 'f') {
            return c - 'a' + 10;
        }

        if (c >= 'A' && c <= 'F') {
            return c - 'A' + 10;
        }

        return -1;
    };

    for (std::size_t i = 0; i < value.size();) {
        if (value[i] == '-') {
            ++i;
            continue;
        }

        if (i + 1 >= value.size()) {
            return false;
        }

        const int highNibble =
            hexValue(value[i]);

        const int lowNibble =
            hexValue(value[i + 1]);

        if (highNibble < 0 ||
            lowNibble < 0) {
            return false;
        }

        if (byteIndex >= bytes.size()) {
            return false;
        }

        bytes[byteIndex++] =
            static_cast<std::uint8_t>(
                (highNibble << 4) |
                lowNibble
            );

        i += 2;
    }

    if (byteIndex != 16) {
        return false;
    }

    std::uint64_t high = 0;
    std::uint64_t low = 0;

    for (std::size_t i = 0; i < 8; ++i) {
        high =
            (high << 8) |
            bytes[i];

        low =
            (low << 8) |
            bytes[i + 8];
    }

    out = EntityId(high, low);

    return true;
}

} // namespace beast
