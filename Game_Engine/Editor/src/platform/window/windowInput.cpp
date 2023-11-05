//
// Created by lenovo on 10/24/2023.
//

#include "windowInput.h"
#include "GLFW/glfw3.h"
#include "yicvot_editor/Application.h"

namespace miku {

    Input* Input::instance_ = new WindowsInput();

    bool miku::WindowsInput::isKeyPressedImpl(int keycode) {
        auto window = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
        auto state = glfwGetKey(window, keycode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool miku::WindowsInput::isMouseButtonPressedImpl(int button) {
        auto window = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
        auto state = glfwGetMouseButton(window, button);

        return state == GLFW_PRESS;
    }

    std::pair<float, float> miku::WindowsInput::getMousePositionImpl() {
        auto window = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return {(float) xPos, (float) yPos};
    }

    float miku::WindowsInput::getMouseXImpl() {
        auto [x, y] = getMousePositionImpl();

        return x;
    }

    float miku::WindowsInput::getMouseYImpl() {
        auto [x, y] = getMousePositionImpl();

        return y;
    }
}

