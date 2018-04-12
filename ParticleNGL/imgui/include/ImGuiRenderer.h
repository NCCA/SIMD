#pragma once

#include <QObject>
#include <imgui.h>
#include <memory>
#include <ngl/Types.h>
#include <ngl/AbstractVAO.h>
#include <memory>
class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

namespace QtImGui {

class WindowWrapper
{
public:
    virtual ~WindowWrapper() {}
    virtual void installEventFilter(QObject *object) = 0;
    virtual QSize size() const = 0;
    virtual qreal devicePixelRatio() const = 0;
    virtual bool isActive() const = 0;
    virtual QPoint mapFromGlobal(const QPoint &p) const = 0;
};

class ImGuiRenderer : public QObject {
    Q_OBJECT
public:
    void initialize(WindowWrapper *window);
    void newFrame();

    bool eventFilter(QObject *watched, QEvent *event);

    static ImGuiRenderer *instance();

private:
    ImGuiRenderer() {}

    void onMousePressedChange(QMouseEvent *event);
    void onWheel(QWheelEvent *event);
    void onKeyPressRelease(QKeyEvent *event);

    void renderDrawList(ImDrawData *draw_data);
    bool createFontsTexture();
    bool createDeviceObjects();

    std::unique_ptr<WindowWrapper> m_window;
    double       g_Time = 0.0;
    bool         g_MousePressed[3] = { false, false, false };
    float        g_MouseWheel = 0.0f;
    GLuint       g_FontTexture = 0;
////  std::unique_ptr<ngl::AbstractVAO> m_vao;
//    std::unique_ptr<ngl::AbstractVAO> vao;

};

}
