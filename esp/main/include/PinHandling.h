//
// Created by lukaszk on 06.07.2020.
//

#ifndef APP_TEMPLATE_PINHANDLING_H
#define APP_TEMPLATE_PINHANDLING_H

class PinHandling {
public:
    PinHandling();
    void Run();
    static void ChangeStateOnImpuls(void * state);
};



#endif //APP_TEMPLATE_PINHANDLING_H
