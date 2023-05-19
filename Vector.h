#pragma once


class Vector {
public:
    double x;
    double y;

public:
    Vector& operator+=(const Vector& other);
    Vector operator*(const double& n) const;
    Vector operator/(const double& n) const;
    Vector& operator/=(const double& n);
    Vector& operator*=(const double& n);

    Vector unit() const;
    double scalar() const;
};