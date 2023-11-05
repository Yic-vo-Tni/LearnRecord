//
// Created by lenovo on 10/23/2023.
//

#ifndef LEARN_RECORD_LAYERSTACK_H
#define LEARN_RECORD_LAYERSTACK_H

#include "Core.h"
#include "layer.h"

namespace miku{
    class YIC_API LayerStack{
    public:
        LayerStack();
        ~LayerStack();

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);
        void popLayer(Layer* layer);
        void popOverlay(Layer* layer);

        std::vector<Layer*>::iterator begin() { return layers_.begin(); }
        std::vector<Layer*>::iterator end() { return layers_.end(); }

    private:
        std::vector<Layer*> layers_;
        std::vector<Layer*>::iterator layerInsert_;
    };
}

#endif //LEARN_RECORD_LAYERSTACK_H
