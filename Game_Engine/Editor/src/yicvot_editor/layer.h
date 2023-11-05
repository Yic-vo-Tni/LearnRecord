//
// Created by lenovo on 10/23/2023.
//

#ifndef LEARN_RECORD_LAYER_H
#define LEARN_RECORD_LAYER_H

#include "Core.h"
#include "Event/Event.h"

namespace miku{

    class YIC_API Layer{
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate() {}
        virtual void onEvent(Event& event) {}

        inline const std::string& getName() const { return debugName_; }

    protected:
        std::string debugName_;
    };

}

#endif //LEARN_RECORD_LAYER_H
