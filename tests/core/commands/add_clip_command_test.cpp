#include <gtest/gtest.h>

#include <memory>

#include "../../../native/beast_engine/core/commands/add_clip_command.h"
#include "../../../native/beast_engine/core/document/document.h"
#include "../../../native/beast_engine/core/editor/editor.h"
#include "../../../native/beast_engine/core/project/project.h"
#include "../../../native/beast_engine/core/project/sequence.h"
#include "../../../native/beast_engine/core/state/revision.h"
#include "../../../native/beast_engine/core/timeline/clip.h"
#include "../../../native/beast_engine/core/timeline/track.h"

using namespace beast;

TEST(AddClipCommandTest, ExecuteAddsClipToTrack) {
    auto project =
        std::make_unique<Project>(
            1,
            "Test"
        );

    auto sequence =
        std::make_unique<Sequence>(
            10,
            "Main"
        );

    auto track =
        std::make_unique<Track>(
            20,
            TrackType::Video,
            "Video"
        );

    const TrackId trackId =
        track->id();

    sequence->addTrack(
        std::move(track)
    );

    project->addSequence(
        std::move(sequence)
    );

    Editor editor(
        std::move(project)
    );

    auto clip =
        std::make_unique<Clip>(
            100,
            500
        );

    clip->setTimelineStart(
        Time::fromSeconds(0.0)
    );

    clip->setTimelineDuration(
        Time::fromSeconds(5.0)
    );

    const ClipId clipId =
        clip->id();

    auto command =
        std::make_unique<AddClipCommand>(
            trackId,
            std::move(clip)
        );

    EXPECT_EQ(
        editor.revision().value(),
        0
    );

    EXPECT_EQ(
        editor.execute(
            std::move(command)
        ),
        CommandResult::Success
    );

    EXPECT_EQ(
        editor.revision().value(),
        1
    );

    const Sequence* activeSequence =
        editor
            .state()
            .document()
            .project()
            .activeSequence();

    ASSERT_NE(
        activeSequence,
        nullptr
    );

    const Track* activeTrack =
        activeSequence->findTrack(
            trackId
        );

    ASSERT_NE(
        activeTrack,
        nullptr
    );

    ASSERT_NE(
        activeTrack->findClip(
            clipId
        ),
        nullptr
    );
}

TEST(AddClipCommandTest, UndoRemovesClip) {
    auto project =
        std::make_unique<Project>(
            2,
            "Test"
        );

    auto sequence =
        std::make_unique<Sequence>(
            10,
            "Main"
        );

    auto track =
        std::make_unique<Track>(
            20,
            TrackType::Video,
            "Video"
        );

    const TrackId trackId =
        track->id();

    sequence->addTrack(
        std::move(track)
    );

    project->addSequence(
        std::move(sequence)
    );

    Editor editor(
        std::move(project)
    );

    auto clip =
        std::make_unique<Clip>(
            100,
            500
        );

    const ClipId clipId =
        clip->id();

    auto command =
        std::make_unique<AddClipCommand>(
            trackId,
            std::move(clip)
        );

    EXPECT_EQ(
        editor.execute(
            std::move(command)
        ),
        CommandResult::Success
    );

    EXPECT_EQ(
        editor.undo(),
        CommandResult::Success
    );

    const Sequence* activeSequence =
        editor
            .state()
            .document()
            .project()
            .activeSequence();

    ASSERT_NE(
        activeSequence,
        nullptr
    );

    const Track* activeTrack =
        activeSequence->findTrack(
            trackId
        );

    ASSERT_NE(
        activeTrack,
        nullptr
    );

    EXPECT_EQ(
        activeTrack->findClip(
            clipId
        ),
        nullptr
    );
}

TEST(AddClipCommandTest, RedoRestoresClip) {
    auto project =
        std::make_unique<Project>(
            3,
            "Test"
        );

    auto sequence =
        std::make_unique<Sequence>(
            10,
            "Main"
        );

    auto track =
        std::make_unique<Track>(
            20,
            TrackType::Video,
            "Video"
        );

    const TrackId trackId =
        track->id();

    sequence->addTrack(
        std::move(track)
    );

    project->addSequence(
        std::move(sequence)
    );

    Editor editor(
        std::move(project)
    );

    auto clip =
        std::make_unique<Clip>(
            100,
            500
        );

    const ClipId clipId =
        clip->id();

    auto command =
        std::make_unique<AddClipCommand>(
            trackId,
            std::move(clip)
        );

    ASSERT_EQ(
        editor.execute(
            std::move(command)
        ),
        CommandResult::Success
    );

    ASSERT_EQ(
        editor.undo(),
        CommandResult::Success
    );

    ASSERT_EQ(
        editor.redo(),
        CommandResult::Success
    );

    const Sequence* activeSequence =
        editor
            .state()
            .document()
            .project()
            .activeSequence();

    ASSERT_NE(
        activeSequence,
        nullptr
    );

    const Track* activeTrack =
        activeSequence->findTrack(
            trackId
        );

    ASSERT_NE(
        activeTrack,
        nullptr
    );

    EXPECT_NE(
        activeTrack->findClip(
            clipId
        ),
        nullptr
    );
}
