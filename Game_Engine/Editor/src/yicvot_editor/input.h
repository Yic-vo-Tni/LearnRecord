//
// Created by lenovo on 10/24/2023.
//

#ifndef LEARN_RECORD_INPUT_H
#define LEARN_RECORD_INPUT_H

#include "Core.h"

namespace miku{

    class YIC_API Input{
    public:
        inline static bool isKeyPressed(int keycode) { return instance_->isKeyPressedImpl(keycode); }
        inline static bool isMouseButtonPressed(int keycode) { return instance_->isMouseButtonPressedImpl(keycode); }
        inline static std::pair<float, float> getMousePosition() { return instance_->getMousePositionImpl(); }
        inline static bool getMouseX() { return instance_->getMouseXImpl(); }
        inline static bool getMouseY() { return instance_->getMouseYImpl(); }


    protected:
        virtual bool isKeyPressedImpl(int keycode) = 0;
        virtual bool isMouseButtonPressedImpl(int button) = 0;
        virtual std::pair<float, float> getMousePositionImpl() = 0;
        virtual float getMouseXImpl() = 0;
        virtual float getMouseYImpl() = 0;

    private:
        static Input* instance_;
    };

}

#endif //LEARN_RECORD_INPUT_H
