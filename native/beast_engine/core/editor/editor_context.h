#ifndef BEAST_EDITOR_CONTEXT_H
#define BEAST_EDITOR_CONTEXT_H

namespace beast {

class EditorState;

class EditorContext final {
public:
    explicit EditorContext(EditorState& state) noexcept;

    EditorContext(const EditorContext&) = delete;
    EditorContext& operator=(const EditorContext&) = delete;

    EditorState& state() noexcept;
    const EditorState& state() const noexcept;

private:
    EditorState& state_;
};

} // namespace beast

#endif // BEAST_EDITOR_CONTEXT_H
