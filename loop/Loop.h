#ifndef _LOOP_H_
#define _LOOP_H_


class Loop {
public:
    Loop(){}
    virtual ~Loop(){}
public:
    bool Init();
    void UnInit();
public:
    void Run();
    void Stop();
private:
    bool run = false;
    PollEvent* pe = nullptr;
};

#endif 
