#ifdef __unix__

#include "input/LinuxInputManager.hpp"

namespace InputManager
{
    int MOVE_SENSITIVITY = 1;
    int SCROLL_SENSITIVITY = 1;

    Linux::Linux()
    {
        fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

        ioctl(fd, UI_SET_EVBIT, EV_KEY);
        ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
        ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);

        for(int i = 0; i <= 257; i++)
        {
            ioctl(fd, UI_SET_KEYBIT, i);
        }

        ioctl(fd, UI_SET_EVBIT, EV_REL);
        ioctl(fd, UI_SET_RELBIT, REL_X);
        ioctl(fd, UI_SET_RELBIT, REL_Y);
        ioctl(fd, UI_SET_RELBIT, REL_WHEEL);

        memset(&usetup, 0, sizeof(usetup));
        usetup.id.bustype = BUS_USB;
        usetup.id.vendor = 0x1234;
        usetup.id.product = 0x1234;
        strcpy(usetup.name, "Mousedroid");

        ioctl(fd, UI_DEV_SETUP, &usetup);
        ioctl(fd, UI_DEV_CREATE);

        sleep(1);
    }

    void Linux::emit(int fd, int type, int code, int val)
    {
        input_event ie;

        ie.type = type;
        ie.code = code;
        ie.value = val;
        ie.time.tv_sec = 0;
        ie.time.tv_usec = 0;

        write(fd, &ie, sizeof(ie));
    }

    std::pair<int, bool> Linux::getFromKeyMap(char c)
    {
        if(c >= 'a' && c <= 'z')
            c -= 32;

        for(auto k : KEYMAP)
        {
            if(k.c == c)
                return {k.keycode, k.shouldShift};
        }

        return {0, 0};
    }

    int Linux::parse_char(char c, bool &shiftPressed)
    {
        shiftPressed = false;

        auto res = getFromKeyMap(c);
        shiftPressed = res.second;

        if(c >= 'a' && c <= 'z')    
            shiftPressed = false;

        return res.first;
    }

    void Linux::click()
    {
        emit(fd, EV_KEY, BTN_LEFT, 1);
        emit(fd, EV_SYN, SYN_REPORT, 0);
        emit(fd, EV_KEY, BTN_LEFT, 0);
        emit(fd, EV_SYN, SYN_REPORT, 0);
    }

    void Linux::right_click()
    {
        emit(fd, EV_KEY, BTN_RIGHT, 1);
        emit(fd, EV_SYN, SYN_REPORT, 0);
        emit(fd, EV_KEY, BTN_RIGHT, 0);
        emit(fd, EV_SYN, SYN_REPORT, 0);
    }

    void Linux::move(int dx, int dy)
    {
        emit(fd, EV_REL, REL_X, (int)(MOVE_SENSITIVITY / 10.0 * dx));
        emit(fd, EV_REL, REL_Y, (int)(MOVE_SENSITIVITY / 10.0 * dy));
        emit(fd, EV_SYN, SYN_REPORT, 0);
    }

    void Linux::scroll(int scroll_amount)
    {
        emit(fd, EV_REL, REL_WHEEL, (int)((scroll_amount * (SCROLL_SENSITIVITY / 10.0))));
        emit(fd, EV_SYN, SYN_REPORT, 0);
    }

    void Linux::down()
    {
        emit(fd, EV_KEY, BTN_LEFT, 1);
        emit(fd, EV_SYN, SYN_REPORT, 0);
    }

    void Linux::up()
    {
        emit(fd, EV_KEY, BTN_LEFT, 0);
        emit(fd, EV_SYN, SYN_REPORT, 0);
    }

    void Linux::send_key(char c)
    {
        bool shiftPressed;
        int code = parse_char(c, shiftPressed);

        if(shiftPressed)
        {
            emit(fd, EV_KEY, KEY_LEFTSHIFT, 1);
            emit(fd, EV_SYN, SYN_REPORT, 0);
        }

        emit(fd, EV_KEY, code, 1);
        emit(fd, EV_SYN, SYN_REPORT, 0);
        emit(fd, EV_KEY, code, 0);
        emit(fd, EV_SYN, SYN_REPORT, 0);

        if(shiftPressed)
        {
            emit(fd, EV_KEY, KEY_LEFTSHIFT, 0);
            emit(fd, EV_SYN, SYN_REPORT, 0);
        }
    }
}

#endif