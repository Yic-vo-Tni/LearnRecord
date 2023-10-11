//
// Created by lenovo on 10/10/2023.
//

#ifndef LEARN_RECORD_MOUSEEVENT_H
#define LEARN_RECORD_MOUSEEVENT_H

#include "Event.h"

#include "sstream"

namespace miku{
    class YIC_API MouseMovedEvent : public Event{
    public:
        MouseMovedEvent(float x, float y) : mouseX_(x) , mouseY_(y) {}

        inline float getX() const { return mouseX_; }
        inline float getY() const { return mouseY_; }

        std::string toString() const override{
            std::stringstream ss;
            ss << "MouseMovedEvent: " << mouseX_ << " , " << mouseY_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float mouseX_, mouseY_;
    };

    class YIC_API MouseScrolledEvent : public Event{
    public:
        MouseScrolledEvent(float x, float y) : xOffset_(x), yOffset_(y) {}

        inline float getxOffset() const { return xOffset_; }
        inline float getyOffset() const { return yOffset_; }

        std::string toString() const override{
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << getxOffset() << ", " << getyOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float xOffset_, yOffset_;
    };

    class YIC_API MouseButtonEvent : public Event{
    public:
        inline int getMouseButton() const { return button_; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    protected:
        MouseButtonEvent(int button) : button_(button){}

        int button_;
    };

    class YIC_API MouseButtonPressedEvent : public MouseButtonEvent{
    public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button){}

        std::string toString() const override{
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << button_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)

    };

    class YIC_API MouseButtonReleaseEvent : public MouseButtonEvent{
    public:
        MouseButtonReleaseEvent(int button) : MouseButtonEvent(button){}

        std::string toString() const override{
            std::stringstream ss;
            ss << "MouseButtonReleaseEvent: " << button_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}

#endif //LEARN_RECORD_MOUSEEVENT_H
