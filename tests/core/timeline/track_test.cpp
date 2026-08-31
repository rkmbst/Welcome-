#include <cassert>
#include <memory>

#include "../../../native/beast_engine/core/timeline/track.h"
#include "../../../native/beast_engine/core/timeline/clip.h"
#include "../../../native/beast_engine/core/time/time.h"

int main() {
    using namespace beast;

    Track track(
        1,
        TrackType::Video,
        "Video 1"
    );

    auto clip =
        std::make_unique<Clip>(
            100,
            500
        );

    clip->setTimelineStart(
        Time::fromSeconds(5.0)
    );

    clip->setTimelineDuration(
        Time::fromSeconds(10.0)
    );

    assert(
        track.addClip(
            std::move(clip)
        )
    );

    assert(track.clipCount() == 1);

    Clip* found =
        track.findClip(100);

    assert(found != nullptr);

    /*
     * Clip interval:
     *
     * [5s, 15s)
     */

    auto active =
        track.activeClips(
            Time::fromSeconds(7.0)
        );

    assert(active.size() == 1);
    assert(active[0]->id() == 100);

    auto inactive =
        track.activeClips(
            Time::fromSeconds(15.0)
        );

    assert(inactive.empty());

    return 0;
}
