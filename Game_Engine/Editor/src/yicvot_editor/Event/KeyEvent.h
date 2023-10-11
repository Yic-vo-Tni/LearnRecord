//
// Created by lenovo on 10/10/2023.
//

#ifndef LEARN_RECORD_KEYEVENT_H
#define LEARN_RECORD_KEYEVENT_H

#include "Event.h"
#include "sstream"

namespace miku {
    class YIC_API KeyEvent : public Event {
    public:
        inline int getKeyCode() const { return keyCode_; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        KeyEvent(int keycode) : keyCode_(keycode) {}

        int keyCode_;
    };

    class YIC_API KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), repeatCount_(repeatCount) {}

        inline int getRepeatCount() const { return repeatCount_; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "keyPressedEvent: " << keyCode_ << "(" << repeatCount_ << " repeats)";
            return ss.str();
        };

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        int repeatCount_;
    };


    class YIC_API KeyReleasedEvent : public KeyEvent{
    public:
        KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

        std::string toString() const override{
            std::stringstream ss;
            ss << " KeyReleasedEvent " << keyCode_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

}

#endif //LEARN_RECORD_KEYEVENT_H
