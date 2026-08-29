#include "time.h"

#include <cmath>

namespace beast {

constexpr Time Time::fromSeconds(double seconds) noexcept {
    return Time(
        static_cast<Tick>(
            std::llround(seconds * static_cast<double>(kTicksPerSecond))
        )
    );
}

double Time::seconds() const noexcept {
    return static_cast<double>(ticks_) /
           static_cast<double>(kTicksPerSecond);
}

double FrameRate::fps() const noexcept {
    if (denominator == 0) {
        return 0.0;
    }

    return static_cast<double>(numerator) /
           static_cast<double>(denominator);
}

Time Frame::timeAt(FrameRate rate) const noexcept {
    if (rate.numerator <= 0 || rate.denominator <= 0) {
        return Time::zero();
    }

    const double seconds =
        static_cast<double>(index_) *
        static_cast<double>(rate.denominator) /
        static_cast<double>(rate.numerator);

    return Time::fromSeconds(seconds);
}

} // namespace beast
