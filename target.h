#pragma once
void draw_target(HDC mdc, double mx, double my, double var);

static double var; //총을 오래 사격할 수록 반동으로 인해 정확도가 떨어짐, 수치가 증가할 수록 분산도가 커짐