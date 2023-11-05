//
// Created by lenovo on 10/9/2023.
//
#include <miku.h>

class ExampleLayer : public miku::Layer{
public:
    ExampleLayer() : miku::Layer("Example"){

    }

    void onUpdate() override{
        //YIC_INFO("ExampleLayer:: update");

        if(miku::Input::isKeyPressed(YIC_KEY_TAB)){
            YIC_TRANCE("Tab key is pressed!");
        }
    }

    void onEvent(miku::Event& event) override{
        //YIC_TRANCE("{0}", event);
        if(event.GetEventType() == miku::EventType::KeyPressed){
            miku::KeyPressedEvent& e = (miku::KeyPressedEvent&)event;
            YIC_TRANCE("{0}", (char)e.getKeyCode());
        }
    }
};

class Yic : public miku::Application{
public:
    Yic(){
        pushLayer(new ExampleLayer());
        pushOverlay(new miku::ImguiLayer());
    }

    ~Yic(){

    }
};

miku::Application* miku::createApplication() {
    return new Yic();
}