#pragma once

namespace Gun {
    int damage(int gun_id);         // 공격력
    int shoot_speed(int gun_id);    // 발사 속도
    int max_ammo(int gun_id);       // 최대 탄창
    int reload_delay(int gun_id);   // 재장전 딜레이
    int reload_speed(int gun_id);   // 재장전 속도
    int recoil(int gun_id);         // 탄퍼짐
    int shake(int gun_id);          // 화면 흔들림 정도
    int shake_time(int gun_id);     // 화면 흔들림 시간
    int price(int gun_id);          // 가격
    int speed_info(int gun_id);     //연사력 정보
}

