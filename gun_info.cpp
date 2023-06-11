#include "gun_info.h"

#include "images.h"

int Gun::damage(int gun_id) {
    switch(gun_id) {
    case scar_h:
        return 9;
    case m16:
        return 6;
    case mp_44:
        return 15;
    case mg_42:
        return 4;
    case awp:
        return 100;
    case m1: 
        return 30;
    case h_brush:
        return 2;
    }
    return 0;
}

int Gun::shoot_speed(int gun_id) {
    switch(gun_id) {
    case scar_h:
        return 7;
    case m16:
        return 5;
    case mp_44:
        return 12;
    case mg_42:
        return 3;
    case awp:
        return 100;
    case m1:
        return 0;
    case h_brush:
        return 30;
    }
    return 0;
}

int Gun::max_ammo(int gun_id) {
    switch(gun_id) {
    case scar_h:
        return 30;
    case m16:
        return 40;
    case mp_44:
        return 20;
    case mg_42:
        return 300;
    case awp:
        return 5;
    case m1:
        return 8;
    case h_brush:
        return 10;
    }
    return 0;
}

int Gun::reload_delay(int gun_id) {
    switch(gun_id) {
    case mg_42:
        return 5;
        break;

    case awp:
        return 2;
        break;
    }
    return 0;
}

int Gun::reload_speed(int gun_id) {
    switch(gun_id) {
    case scar_h:
        return 3;
    case m16:
        return 4;
    case mp_44:
        return 2;
    case mg_42:
        return 2;
    case awp:
        return 2;
    case m1:
        return 5;
    case h_brush:
        return 1;
    }
    return 0;
}

int Gun::recoil(int gun_id) {
    switch(gun_id) {
    case scar_h:
        return 4;
    case m16:
        return 2;
    case mp_44:
        return 6;
    case mg_42:
        return 2;
    case awp:
        return 0;
    case m1:
        return 60;
    case h_brush:
        return 0;
    }
    return 0;
}

int Gun::shake(int gun_id) {
    switch (gun_id) {
    case scar_h:
        return 10;
    case m16:
        return 8;
    case mp_44:
        return 15;
    case mg_42:
        return 10;
    case awp:
        return 20;
    case m1:
        return 15;
    case h_brush:
        return 15;
    }
    return 0;
}

int Gun::shake_time(int gun_id) {
    switch (gun_id) {
    case scar_h:
        return 5;
    case m16:
        return 5;
    case mp_44:
        return 6;
    case mg_42:
        return 5;
    case awp:
        return 50;
    case m1:
        return 10;
    case h_brush: 
        return 10;
    }
    return 0;
}

int Gun::price(int gun_id) {
    switch(gun_id) {
    case scar_h:
        return 0;
    case m16:
        return 300;
    case mp_44:
        return 500;
    case mg_42:
        return 1000;
    case awp:
        return 1000;
    case m1:
        return 1200;
    case h_brush:
        return 2500;
    }
    return 0;
}

int Gun::speed_info(int gun_id) {
    switch (gun_id) {
    case scar_h:
        return 545;
    case m16:
        return 700;
    case mp_44:
        return 375;
    case mg_42:
        return 1500;
    case awp:
        return 46;
    case m1:
        return 0;
    case h_brush:
        return 200;
    }
    return 0;
}