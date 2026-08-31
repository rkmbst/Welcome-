#include <cassert>

#include "../../../native/beast_engine/core/time/time.h"

int main() {
    using namespace beast;

    const Time zero = Time::zero();

    assert(zero.ticks() == 0);
    assert(zero.seconds() == 0.0);

    const Time oneSecond =
        Time::fromSeconds(1.0);

    assert(
        oneSecond.ticks() ==
        kTicksPerSecond
    );

    const Time halfSecond =
        Time::fromSeconds(0.5);

    assert(
        halfSecond.ticks() ==
        kTicksPerSecond / 2
    );

    const Time sum =
        oneSecond + halfSecond;

    assert(
        sum.ticks() ==
        kTicksPerSecond +
        kTicksPerSecond / 2
    );

    const Time difference =
        sum - oneSecond;

    assert(
        difference.ticks() ==
        kTicksPerSecond / 2
    );

    return 0;
}
