#include "time.h"

namespace beast {

/* ============================================================
 * Time
 * ============================================================ */

Time Time::fromSeconds(
    double seconds
) noexcept {
    return Time(
        static_cast<Tick>(
            seconds *
            static_cast<double>(kTicksPerSecond)
        )
    );
}

double Time::seconds() const noexcept {
    return static_cast<double>(ticks_) /
           static_cast<double>(kTicksPerSecond);
}

/* ============================================================
 * Rational Frame Rate
 * ============================================================ */

double FrameRate::fps() const noexcept {
    if (denominator == 0) {
        return 0.0;
    }
    return static_cast<double>(numerator) /
           static_cast<double>(denominator);
}

/* ============================================================
 * Frame
 * ============================================================ */

Time Frame::timeAt(
    FrameRate rate
) const noexcept {
    if (rate.denominator == 0) {
        return Time::zero();
    }
    const Tick ticks =
        index_ *
        static_cast<Tick>(rate.numerator) *
        kTicksPerSecond /
        static_cast<Tick>(rate.denominator);
    return Time(ticks);
}

} // namespace beast
