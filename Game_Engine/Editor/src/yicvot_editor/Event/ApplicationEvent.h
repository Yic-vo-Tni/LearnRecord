//
// Created by lenovo on 10/10/2023.
//

#ifndef LEARN_RECORD_APPLICATIONEVENT_H
#define LEARN_RECORD_APPLICATIONEVENT_H

#include "sstream"
#include "Event.h"

namespace miku{
    class YIC_API WindowResizeEvent : public Event{
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : width_(width), height_(height){}

        inline unsigned int getWidth() const { return width_; }
        inline unsigned int getHeight() const { return height_; }

        std::string toString() const override {
            std::stringstream stringStream;
            stringStream << " WidowResizeEvent: " << width_ << "," << height_;
            return stringStream.str();
        }

        EVENT_CLASS_TYPE(WindowResize);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);


    private:
        unsigned int width_, height_;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppTickEvent : public Event
    {
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppRenderEvent : public Event
    {
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };


}

#endif //LEARN_RECORD_APPLICATIONEVENT_H