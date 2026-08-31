#include "editor_context.h"

#include "../state/editor_state.h"

namespace beast {

EditorContext::EditorContext(
    EditorState& state
) noexcept
    : state_(state) {
}

EditorState& EditorContext::state() noexcept {
    return state_;
}

const EditorState& EditorContext::state() const noexcept {
    return state_;
}

} // namespace beast
