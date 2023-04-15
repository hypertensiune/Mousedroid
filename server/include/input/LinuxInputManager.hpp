#pragma once

#ifdef __unix__

#include "InputManager.hpp"

#include <linux/uinput.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

namespace InputManager
{
    class Linux : public Base
    {
        public:
            Linux();

            std::vector<KEY_PAIR> KEYMAP = 
            {
                {'1', KEY_1, false},
                {'!', KEY_1, true},
                {'2', KEY_2, false},
                {'@', KEY_2, true},
                {'3', KEY_3, false},
                {'#', KEY_3, true},
                {'4', KEY_4, false},
                {'$', KEY_4, true},
                {'5', KEY_5, false},
                {'%', KEY_5, true},
                {'6', KEY_6, false},
                {'^', KEY_6, true},
                {'7', KEY_7, false},
                {'&', KEY_7, true},
                {'8', KEY_8, false},
                {'*', KEY_8, true},
                {'9', KEY_9, false},
                {'(', KEY_9, true},
                {'0', KEY_0, false},
                {')', KEY_0, true},
                {'`', KEY_GRAVE, false},
                {'~', KEY_GRAVE, true},
                {'-', KEY_MINUS, false},
                {'_', KEY_MINUS, true},
                {'=', KEY_EQUAL, false},
                {'+', KEY_EQUAL, true},
                {'[', KEY_LEFTBRACE, false},
                {'{', KEY_LEFTBRACE, true},
                {']', KEY_RIGHTBRACE, false},
                {'}', KEY_RIGHTBRACE, true},
                {';', KEY_SEMICOLON, false},
                {':', KEY_SEMICOLON, true},
                {'\'', KEY_APOSTROPHE, false},
                {'"', KEY_APOSTROPHE, true},
                {',', KEY_COMMA, false},
                {'<', KEY_COMMA, true},
                {'.', KEY_DOT, false},
                {'>', KEY_DOT, true},
                {'/', KEY_SLASH, false},
                {'?', KEY_SLASH, true},
                {'A', KEY_A, true},
                {'B', KEY_B, true},
                {'C', KEY_C, true},
                {'D', KEY_D, true},
                {'E', KEY_E, true},
                {'F', KEY_F, true},
                {'G', KEY_G, true},
                {'H', KEY_H, true},
                {'I', KEY_I, true},
                {'J', KEY_J, true},
                {'K', KEY_K, true},
                {'L', KEY_L, true},
                {'M', KEY_M, true},
                {'N', KEY_N, true},
                {'O', KEY_O, true},
                {'P', KEY_P, true},
                {'Q', KEY_Q, true},
                {'R', KEY_R, true},
                {'S', KEY_S, true},
                {'T', KEY_T, true},
                {'U', KEY_U, true},
                {'V', KEY_V, true},
                {'W', KEY_W, true},
                {'X', KEY_X, true},
                {'Y', KEY_Y, true},
                {'Z', KEY_Z, true},
                {'\n', KEY_ENTER, false},
                {(char)127, KEY_DELETE, false}
            };

            std::pair<int, bool> getFromKeyMap(char c);

        private:
            int fd;
            uinput_setup usetup;

            /**
             * Transforms a char to a win32 keycode
             * https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h
             */
            int parse_char(char c, bool &shiftPressed);

            void emit(int fd, int type, int code, int val);

            void click() override;
            void right_click() override;
            void move(int dx, int dy) override;
            void scroll(int scroll_amount) override;
            void down() override;
            void up() override;
            void send_key(char c) override;
    };
}

#endif