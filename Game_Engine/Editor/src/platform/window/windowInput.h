//
// Created by lenovo on 10/24/2023.
//

#ifndef LEARN_RECORD_WINDOWINPUT_H
#define LEARN_RECORD_WINDOWINPUT_H

#include "yicvot_editor/input.h"

namespace miku{

    class WindowsInput : public Input{
    protected:
        virtual bool isKeyPressedImpl(int keycode) override;
        virtual bool isMouseButtonPressedImpl(int button) override;
        virtual std::pair<float, float> getMousePositionImpl() override;
        virtual float getMouseXImpl() override;
        virtual float getMouseYImpl() override;
    };

}

#endif //LEARN_RECORD_WINDOWINPUT_H
