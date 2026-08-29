#ifndef BEAST_TIME_H
#define BEAST_TIME_H

#include <cstdint>

namespace beast {

/*
 * Beast Media Studio
 * Core Time System
 *
 * Internal time is represented as integer ticks.
 *
 * This avoids floating-point drift and gives us deterministic
 * timeline, playback, keyframe and export calculations.
 */

using Tick = std::int64_t;
using FrameIndex = std::int64_t;

/*
 * Global internal timebase.
 *
 * 1 second = 1,000,000 ticks
 */
constexpr Tick kTicksPerSecond = 1'000'000;

/* ============================================================
 * Time
 * ============================================================ */

class Time final {
public:
    constexpr Time() noexcept = default;

    explicit constexpr Time(Tick ticks) noexcept
        : ticks_(ticks) {}

    static constexpr Time zero() noexcept {
        return Time(0);
    }

    static constexpr Time fromTicks(Tick ticks) noexcept {
        return Time(ticks);
    }

    static constexpr Time fromSeconds(double seconds) noexcept;

    constexpr Tick ticks() const noexcept {
        return ticks_;
    }

    double seconds() const noexcept;

    bool isZero() const noexcept {
        return ticks_ == 0;
    }

    bool isNegative() const noexcept {
        return ticks_ < 0;
    }

    /* --------------------------------------------------------
     * Arithmetic
     * -------------------------------------------------------- */

    constexpr Time operator+(Time other) const noexcept {
        return Time(ticks_ + other.ticks_);
    }

    constexpr Time operator-(Time other) const noexcept {
        return Time(ticks_ - other.ticks_);
    }

    constexpr Time& operator+=(Time other) noexcept {
        ticks_ += other.ticks_;
        return *this;
    }

    constexpr Time& operator-=(Time other) noexcept {
        ticks_ -= other.ticks_;
        return *this;
    }

    constexpr Time operator-() const noexcept {
        return Time(-ticks_);
    }

    /* --------------------------------------------------------
     * Comparison
     * -------------------------------------------------------- */

    constexpr bool operator==(Time other) const noexcept {
        return ticks_ == other.ticks_;
    }

    constexpr bool operator!=(Time other) const noexcept {
        return ticks_ != other.ticks_;
    }

    constexpr bool operator<(Time other) const noexcept {
        return ticks_ < other.ticks_;
    }

    constexpr bool operator<=(Time other) const noexcept {
        return ticks_ <= other.ticks_;
    }

    constexpr bool operator>(Time other) const noexcept {
        return ticks_ > other.ticks_;
    }

    constexpr bool operator>=(Time other) const noexcept {
        return ticks_ >= other.ticks_;
    }

private:
    Tick ticks_{0};
};

/* ============================================================
 * Rational Frame Rate
 * ============================================================ */

struct FrameRate final {
    std::int32_t numerator{30};
    std::int32_t denominator{1};

    constexpr FrameRate() noexcept = default;

    constexpr FrameRate(
        std::int32_t num,
        std::int32_t den
    ) noexcept
        : numerator(num),
          denominator(den) {}

    double fps() const noexcept;
};

/* ============================================================
 * Frame
 * ============================================================ */

class Frame final {
public:
    constexpr Frame() noexcept = default;

    explicit constexpr Frame(FrameIndex index) noexcept
        : index_(index) {}

    static constexpr Frame fromIndex(FrameIndex index) noexcept {
        return Frame(index);
    }

    constexpr FrameIndex index() const noexcept {
        return index_;
    }

    Time timeAt(FrameRate rate) const noexcept;

    /* --------------------------------------------------------
     * Arithmetic
     * -------------------------------------------------------- */

    constexpr Frame operator+(FrameIndex amount) const noexcept {
        return Frame(index_ + amount);
    }

    constexpr Frame operator-(FrameIndex amount) const noexcept {
        return Frame(index_ - amount);
    }

    constexpr FrameIndex operator-(Frame other) const noexcept {
        return index_ - other.index_;
    }

    constexpr Frame& operator+=(FrameIndex amount) noexcept {
        index_ += amount;
        return *this;
    }

    constexpr Frame& operator-=(FrameIndex amount) noexcept {
        index_ -= amount;
        return *this;
    }

    /* --------------------------------------------------------
     * Comparison
     * -------------------------------------------------------- */

    constexpr bool operator==(Frame other) const noexcept {
        return index_ == other.index_;
    }

    constexpr bool operator!=(Frame other) const noexcept {
        return index_ != other.index_;
    }

    constexpr bool operator<(Frame other) const noexcept {
        return index_ < other.index_;
    }

    constexpr bool operator<=(Frame other) const noexcept {
        return index_ <= other.index_;
    }

    constexpr bool operator>(Frame other) const noexcept {
        return index_ > other.index_;
    }

    constexpr bool operator>=(Frame other) const noexcept {
        return index_ >= other.index_;
    }

private:
    FrameIndex index_{0};
};

} // namespace beast

#endif // BEAST_TIME_H
