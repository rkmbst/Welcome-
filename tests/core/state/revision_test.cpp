#include <cassert>

#include "../../../native/beast_engine/core/state/revision.h"

int main() {
    using namespace beast;

    const Revision initial =
        Revision::initial();

    assert(initial.value() == 0);

    const Revision next =
        initial.next();

    assert(next.value() == 1);
    assert(next > initial);
    assert(next != initial);

    const Revision third =
        next.next();

    assert(third.value() == 2);
    assert(third > next);

    return 0;
}
