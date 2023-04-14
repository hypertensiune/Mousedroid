#include "InputManager.hpp"

InputManager::InputManager() { }
InputManager::~InputManager() { }

int InputManager::MOVE_SENSITIVITY = 1;
int InputManager::SCROLL_SENSITIVITY = 1;

std::pair<int, bool> InputManager::getFromKeyMap(char c)
{
    for(auto k : KEYMAP)
    {
        if(k.c == c)
            return {k.keycode, k.shouldShift};
    }

    return {0, 0};
}

void InputManager::execute(std::string message)
{
    Command cmd = parse_message(message);
    exec_command(cmd);
}

InputManager::Command InputManager::parse_message(std::string in)
{
    Command cmd;
    size_t pos = 0;

    cmd.type = in[0] - '0'; 
    in = in.substr(2);

    std::istringstream sstream(in);
    std::string tok;

    int i = 0;
    int l = cmd.type < KEY_PRESS ? 2 : INT_MAX;

    while(std::getline(sstream, tok, '_') && i < l)
    {
        cmd.args.push_back(std::atof(tok.c_str()));
        i++;
    }

    return cmd;
}

void InputManager::exec_command(Command cmd)
{
    switch(cmd.type)
    {
        case InputManager::MOVE:
            move(-(int)cmd.args[0], -(int)cmd.args[1]);
            break;
        
        case InputManager::CLICK:
            click();
            break;

        case InputManager::RIGHT_CLICK:
            right_click();
            break;

        case InputManager::SCROLL:
            scroll(cmd.args[0]);
            break;

        case InputManager::DOWN:
            down();
            break;

        case InputManager::UP:
            up();
            break;

        case InputManager::KEY_PRESS:
            for(int c : cmd.args)
            {
                send_key(c);
            }
            break;
    }
}

int InputManager::parse_char(char c, bool &shiftPressed)
{
    shiftPressed = false;

    if(c >= '0' && c <= '9')
        return c;

    if(c >= 'a' && c <= 'z')
        return c - 32;

    if(c >= 'A' && c <= 'Z')
    {
        shiftPressed = true;
        return c;
    }

    else
    {
        auto res = getFromKeyMap(c);
        shiftPressed = res.second;
        return res.first;
    }
}

void InputManager::click()
{
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

    SendInput(1, &input, sizeof(INPUT));
}

void InputManager::right_click()
{
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;

    SendInput(1, &input, sizeof(INPUT));
}

void InputManager::move(int dx, int dy)
{
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = (int)(MOVE_SENSITIVITY / 10.0 * dx);
    input.mi.dy = (int)(MOVE_SENSITIVITY / 10.0 * dy);

    SendInput(1, &input, sizeof(INPUT));
}

void InputManager::scroll(int scroll_amount)
{
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = (int)(10 * (scroll_amount * (SCROLL_SENSITIVITY / 10.0)));

    SendInput(1, &input, sizeof(INPUT));
}

void InputManager::down()
{
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    SendInput(1, &input, sizeof(INPUT));
}

void InputManager::up()
{
    INPUT input = {};

    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(1, &input, sizeof(INPUT));
}

void InputManager::send_key(char c)
{
    INPUT inputs[4] = {};
    ZeroMemory(inputs, sizeof(inputs));

    bool isShiftPressed;
    int keycode = parse_char(c, isShiftPressed);

    if(isShiftPressed)
    {
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = VK_SHIFT;

        inputs[3].type = INPUT_KEYBOARD;
        inputs[3].ki.wVk = VK_SHIFT;
        inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    }

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = keycode;

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = keycode;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}
