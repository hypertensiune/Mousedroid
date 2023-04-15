#pragma once

#ifdef _WIN32

#include "InputManager.hpp"
#include <windows.h>
#include <map>

namespace InputManager
{
    /**
     * @brief Class for handling input events on windows.
     */
   class Windows : public Base
    {
        public:
            Windows();
            ~Windows();

            // Map all special chars to windows keycodes
            std::vector<KEY_PAIR> KEYMAP =
            {
                {'!', 0x31, true}, // !
                {'@', 0x32, true}, // @
                {'#', 0x33, true}, // #
                {'$', 0x34, true}, // $
                {'%', 0x35, true}, // %
                {'^', 0x36, true}, // ^
                {'&', 0x37, true}, // &
                {'*', 0x38, true}, // *
                {'(', 0x39, true}, // (
                {')', 0x30, true}, // )
                {'`', VK_OEM_3, false},
                {'~', VK_OEM_3, true},
                {'-', VK_OEM_MINUS, false}, // -
                {'_', VK_OEM_MINUS, true}, // _
                {'=', VK_OEM_PLUS, false}, // =
                {'+', VK_OEM_PLUS, true}, // +
                {';', VK_OEM_1, false}, // ;
                {':', VK_OEM_1, true}, // :
                {'\'', VK_OEM_7, false}, // '
                {'\"', VK_OEM_7, true}, // "
                {'/', VK_OEM_2, false}, // /
                {'?', VK_OEM_2, true}, // ?
                {',', VK_OEM_COMMA, false}, // ,
                {'<', VK_OEM_COMMA, true}, // <
                {'.', VK_OEM_PERIOD, false}, // .
                {'>', VK_OEM_PERIOD, true}, // >
                {'[', VK_OEM_4, false}, // [
                {'{', VK_OEM_4, true}, // {
                {']', VK_OEM_6, false}, // ]
                {'}', VK_OEM_6, true}, // }
                {'\\', VK_OEM_5, false}, // \/
                {'|', VK_OEM_5, true}, // |
                {' ', VK_SPACE, false}, // space
                {'\n', VK_RETURN, false}, // ENTER
                {(char)127, VK_BACK, false} // BACKSPACE
            };
            
            std::pair<int, bool> getFromKeyMap(char c) override;

        private:
            /**
             * Transforms a char to a win32 keycode
             * https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
             */
            int parse_char(char c, bool &shiftPressed) override;

            virtual void click() override;
            virtual void right_click() override;
            virtual void move(int dx, int dy) override;
            virtual void scroll(int dy) override;
            virtual void down() override;
            virtual void up() override;
            virtual void send_key(char c) override;
    };
}

#endif