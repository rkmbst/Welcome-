Enter#ifndef BEAST_EDITOR_API_H
#define BEAST_EDITOR_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/*
 * Beast Media Studio
 * C ABI public interface
 *
 * Rules:
 * - Stable ABI
 * - No C++ types exposed
 * - No STL exposed
 * - Opaque handles only
 * - Dart accesses this layer through FFI
 */

#define BEAST_API_VERSION_MAJOR 1
#define BEAST_API_VERSION_MINOR 0

/* =========================================================
 * Opaque Handles
 * ========================================================= */

typedef struct BeastEditor BeastEditor;
typedef struct BeastProject BeastProject;
typedef struct BeastSequence BeastSequence;
typedef struct BeastTrack BeastTrack;
typedef struct BeastClip BeastClip;
typedef struct BeastAsset BeastAsset;

/* =========================================================
 * Result Codes
 * ========================================================= */

typedef enum BeastResult {
    BEAST_OK = 0,

    BEAST_ERROR_INVALID_ARGUMENT = 1,
    BEAST_ERROR_INVALID_HANDLE = 2,
    BEAST_ERROR_NOT_FOUND = 3,
    BEAST_ERROR_OUT_OF_MEMORY = 4,
    BEAST_ERROR_NOT_READY = 5,
    BEAST_ERROR_UNSUPPORTED = 6,
    BEAST_ERROR_INTERNAL = 100
} BeastResult;

/* =========================================================
 * Editor Lifecycle
 * ========================================================= */

BeastEditor* beast_editor_create(void);

void beast_editor_destroy(BeastEditor* editor);

/* =========================================================
 * Project
 * ========================================================= */

BeastResult beast_project_create(
    BeastEditor* editor,
    BeastProject** out_project
);

BeastResult beast_project_destroy(
    BeastEditor* editor,
    BeastProject* project
);

/* =========================================================
 * Sequence
 * ========================================================= */

BeastResult beast_sequence_create(
    BeastProject* project,
    BeastSequence** out_sequence
);

BeastResult beast_sequence_destroy(
    BeastProject* project,
    BeastSequence* sequence
);

/* =========================================================
 * Track
 * ========================================================= */

BeastResult beast_track_create(
    BeastSequence* sequence,
    BeastTrack** out_track
);

BeastResult beast_track_destroy(
    BeastSequence* sequence,
    BeastTrack* track
);

/* =========================================================
 * Clip
 * ========================================================= */

BeastResult beast_clip_create(
    BeastTrack* track,
    BeastClip** out_clip
);

BeastResult beast_clip_destroy(
    BeastTrack* track,
    BeastClip* clip
);

/* =========================================================
 * Asset
 * ========================================================= */

BeastResult beast_asset_create(
    BeastProject* project,
    const char* uri,
    BeastAsset** out_asset
);

BeastResult beast_asset_destroy(
    BeastProject* project,
    BeastAsset* asset
);

/* =========================================================
 * Timeline
 * ========================================================= */

typedef int64_t BeastTime;
typedef int64_t BeastFrame;

BeastResult beast_editor_seek(
    BeastEditor* editor,
    BeastTime time
);

BeastTime beast_editor_get_playhead(
    const BeastEditor* editor
);

/* =========================================================
 * Playback
 * ========================================================= */

BeastResult beast_editor_play(BeastEditor* editor);

BeastResult beast_editor_pause(BeastEditor* editor);

/* =========================================================
 * ABI / Version
 * ========================================================= */

uint32_t beast_api_version_major(void);

uint32_t beast_api_version_minor(void);

const char* beast_last_error(
    const BeastEditor* editor
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BEAST_EDITOR_API_H */
