//
// Created by lenovo on 10/24/2023.
//

#ifndef LEARN_RECORD_IMGUILAYER_H
#define LEARN_RECORD_IMGUILAYER_H

#include "yicvot_editor/layer.h"
#include "yicvot_editor/Application.h"
#include "yicvot_editor/Event/MouseEvent.h"
#include "yicvot_editor/Event/KeyEvent.h"

namespace miku{

    class YIC_API ImguiLayer : public Layer{
    public:
        ImguiLayer();
        ~ImguiLayer();

        void onAttach();
        void onDetach();
        void onUpdate();
        void onEvent(Event& event);

    private:
        float time_ = 0.f;

        bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool onMouseButtonReleaseEvent(MouseButtonReleaseEvent& e);
        bool onMouseMovedEvent(MouseMovedEvent& e);
        bool onMouseScrolledEvent(MouseScrolledEvent& e);
        bool onKeyPressedEvent(KeyPressedEvent& e);
        bool onKeyReleaseEvent(KeyReleasedEvent& e);
        bool onKeyTypeEvent(KeyEvent& e);
        bool onWindowResizedEvent(WindowResizeEvent& e);
    };

}

#endif //LEARN_RECORD_IMGUILAYER_H
