#pragma once

//awp�� ��Ʈ�� ��ܾ� ź�ǰ� ����ǹǷ� �����̸� �־� ź�Ǹ� �����Ѵ�.
//������ ���� BOOL ��ſ� int�� ��ü
extern int cat_ready;
extern int awp_cat_delay;

namespace Gun {
    int damage(int gun_id);         // ���ݷ�
    int shoot_speed(int gun_id);    // �߻� �ӵ�
    int max_ammo(int gun_id);       // �ִ� źâ
    int reload_delay(int gun_id);   // ������ ������
    int reload_speed(int gun_id);   // ������ �ӵ�
    int recoil(int gun_id);         // ź����
    int shake(int gun_id);          // ȭ�� ��鸲 ����
    int shake_time(int gun_id);     // ȭ�� ��鸲 �ð�
    int price(int gun_id);          // ����
}

