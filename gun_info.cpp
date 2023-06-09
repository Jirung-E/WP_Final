#include "gun_info.h"

#include "images.h"

int Gun::damage(int gun_id) {
    switch(gun_id) {
    case scar_h:
        return 8;
    case m16:
        return 5;
    case mp_44:
        return 12;
    case mg_42:
        return 3;
    case awp:
        return 80;
    }
    return 0;
}

int Gun::shoot_speed(int gun_id) {
    switch(gun_id) {
    case scar_h:
        return 7;
    case m16:
        return 4;
    case mp_44:
        return 10;
    case mg_42:
        return 3;
    case awp:
        return 80;
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
        return 30;
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
        return 0;
    }
    return 0;
}