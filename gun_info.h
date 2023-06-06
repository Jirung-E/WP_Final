#pragma once

//awp는 볼트를 당겨야 탄피가 배출되므로 딜레이를 주어 탄피를 생성한다.
//오류가 나서 BOOL 대신에 int로 대체
extern int cat_ready;
extern int awp_cat_delay;

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
}

