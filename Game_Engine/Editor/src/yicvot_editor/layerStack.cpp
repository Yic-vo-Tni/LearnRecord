//
// Created by lenovo on 10/23/2023.
//

#include "layerStack.h"

namespace miku{

    LayerStack::LayerStack()  {
        layerInsert_ = layers_.begin();
    }

    LayerStack::~LayerStack()  {
        for(Layer* layer : layers_){
            delete layer;
        }
    }

    void LayerStack::pushLayer(miku::Layer *layer) {
        layerInsert_ = layers_.emplace(layerInsert_, layer);
    }

    void LayerStack::pushOverlay(miku::Layer *layer) {
        layers_.emplace_back(layer);
    }

    void LayerStack::popLayer(miku::Layer *layer) {
        auto item = std::find(layers_.begin(), layers_.end(), layer);
        if(item != layers_.end()){
            layers_.erase(item);
            layerInsert_--;
        }
    }

    void LayerStack::popOverlay(miku::Layer *layer) {
        auto item = std::find(layers_.begin(), layers_.end(), layer);
        if(item != layers_.end()){
            layers_.erase(item);
        }
    }

}