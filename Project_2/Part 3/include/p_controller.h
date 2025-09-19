#pragma once
class P_controller {
public:
    explicit P_controller(double kp): Kp(kp) {}
    int update(double ref, double actual) {
        double u = Kp * (ref - actual);
        if (u < 0) u = 0;
        if (u > 255) u = 255;
        return (int)u;
    }
private:
    double Kp;
};
