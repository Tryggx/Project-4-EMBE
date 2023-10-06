class P_controller
{
public:
    virtual double update(double ref, double actual);
    P_controller(double Kp);

private:
    volatile double Kp;

};