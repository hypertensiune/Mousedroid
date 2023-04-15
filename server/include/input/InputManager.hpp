#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#ifdef __unix__
    #include <climits>
#endif

namespace InputManager
{
    struct Command 
    {
        int type;
        std::vector<double> args;
    };   

    const int CLICK = 1;
    const int RIGHT_CLICK = 2;
    const int DOWN = 3;
    const int UP = 4;
    const int MOVE = 5;
    const int SCROLL = 6;
    const int KEY_PRESS = 7;

    extern int MOVE_SENSITIVITY;
    extern int SCROLL_SENSITIVITY;
    
    struct KEY_PAIR
    {
        char c;
        int keycode;
        bool shouldShift;
    };

    /**
     * @brief Base class to handle all input events.
     * Specialized os classes are dervived from this.
     */
    class Base
    {
        public:
            void execute(std::string message)
            {
                Command cmd = parse_message(message);
                exec_command(cmd);
            }

            virtual std::pair<int, bool> getFromKeyMap(char c) = 0;

        private:
            /**
             * @brief Parse the message received from client to a command that will be executed
             * @param in 
             * @return Command 
             */
            Command parse_message(std::string in)
            {
                Command cmd;
                size_t pos = 0;

                cmd.type = in[0] - '0'; 
                in = in.substr(2);

                std::istringstream sstream(in);
                std::string tok;

                int i = 0;
                int l = cmd.type < 8 ? 2 : INT_MAX;

                while(std::getline(sstream, tok, '_') && i < l)
                {
                    cmd.args.push_back(std::atof(tok.c_str()));
                    i++;
                }

                return cmd;
            }

            void exec_command(Command cmd)
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

            virtual int parse_char(char c, bool &shiftPressed) = 0;

            virtual void click() = 0;
            virtual void right_click() = 0;
            virtual void move(int dx, int dy) = 0;
            virtual void scroll(int dy) = 0;
            virtual void down() = 0;
            virtual void up() = 0;
            virtual void send_key(char c) = 0;
    };
}