#pragma once



#ifdef PART_1_A21

class Timer_msec
{
public:
    Timer_msec();
    void init(int frequency);
};
#endif

#ifdef PART_2_A21
class Timer_msec
{
public:
    Timer_msec();
    void init(int frequency, float duty_cycle);
};
#endif

#if defined(PART_3_A21) || !defined(PART_1_A21) && !defined(PART_2_A21) && !defined(PART_3_A21)  
class Timer_msec
{
public:
    Timer_msec();
    void init(int frequency, float duty_cycle);
    void set_duty_cycle(float duty_cycle);
};
#endif
