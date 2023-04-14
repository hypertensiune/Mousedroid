#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <windows.h>

class InputManager
{
    public:
        InputManager();
        ~InputManager();

        struct Command 
        {
            int type;
            std::vector<double> args;
        };   

        static const int CLICK = 1;
        static const int RIGHT_CLICK = 2;
        static const int DOWN = 3;
        static const int UP = 4;
        static const int MOVE = 5;
        static const int SCROLL = 6;
        static const int KEY_PRESS = 7;

        static int MOVE_SENSITIVITY;
        static int SCROLL_SENSITIVITY;
        
        struct KEY_PAIR
        {
            char c;
            int keycode;
            bool shouldShift;
        };

        // Map all special chars to windows keycodes
        // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
        std::vector<KEY_PAIR> KEYMAP =
        {
            {'!', 0x31, true},
            {'@', 0x32, true},
            {'#', 0x33, true},
            {'$', 0x34, true},
            {'%', 0x35, true},
            {'^', 0x36, true},
            {'&', 0x37, true},
            {'*', 0x38, true},
            {'(', 0x39, true},
            {')', 0x30, true},
            {'`', VK_OEM_3, false},
            {'~', VK_OEM_3, true},
            {'-', VK_OEM_MINUS, false},
            {'_', VK_OEM_MINUS, true},
            {'=', VK_OEM_PLUS, false},
            {'+', VK_OEM_PLUS, true},
            {';', VK_OEM_1, false},
            {':', VK_OEM_1, true},
            {'\'', VK_OEM_7, false},
            {'\"', VK_OEM_7, true},
            {'/', VK_OEM_2, false},
            {'?', VK_OEM_2, true},
            {',', VK_OEM_COMMA, false},
            {'<', VK_OEM_COMMA, true},
            {'.', VK_OEM_PERIOD, false},
            {'>', VK_OEM_PERIOD, true},
            {'[', VK_OEM_4, false},
            {'{', VK_OEM_4, true},
            {']', VK_OEM_6, false},
            {'}', VK_OEM_6, true},
            {'\\', VK_OEM_5, false},
            {'|', VK_OEM_5, true},
            {' ', VK_SPACE, false},
            {'\n', VK_RETURN, false}, // ENTER
            {(char)127, VK_BACK, false} // BACKSPACE
        };

        void execute(std::string message);
        
        std::pair<int, bool> getFromKeyMap(char c);

    private:
        /**
         * @brief Parse the message received from client to a command that will be executed
         * @param in 
         * @return Command 
         */
        Command parse_message(std::string in);

        void exec_command(Command cmd);

        int parse_char(char c, bool &shiftPressed);

        void click();
        void right_click();
        void move(int dx, int dy);
        void scroll(int dy);
        void down();
        void up();
        void send_key(char c);
};