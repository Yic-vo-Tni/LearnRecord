//
// Created by lenovo on 10/10/2023.
//

#ifndef LEARN_RECORD_EVENT_H
#define LEARN_RECORD_EVENT_H

#include "yicvot_editor/Core.h"

namespace miku {
    enum class EventType {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::type;} \
                                virtual EventType GetEventType() const override {return GetStaticType(); }\
                                virtual const char* GetName() const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}


    class YIC_API Event {
        friend class EventDispatchaer;

    public:
        virtual EventType GetEventType() const = 0;
        virtual const char *GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string toString() const { return GetName(); }

        inline bool IsInCategory(EventCategory category) {
            return GetCategoryFlags() & category;
        }

        bool handled_ = false;
    };

    class EventDispatcher {
        template<class T>
        using eventFn = std::function<bool(T&) >;
    public:
        EventDispatcher(Event& event) : event_(event){}


        template<typename T>
        bool dispatch(eventFn<T> func){
            if (event_.GetEventType() == T::GetStaticType()){
                event_.handled_ = func(*(T*)&event_);
                return true;
            }
            return false;
        }

    private:
        Event& event_;
    };

    inline std::ostream& operator << (std::ostream& ostream, const Event& e){
        return ostream << e.toString();
    }

}

#endif //LEARN_RECORD_EVENT_H
