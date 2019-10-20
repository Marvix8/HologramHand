#ifndef _Gesture_h
#define _Gesture_h

#include "Hand.h"

// Acciones a realizar en aplicación Android
#define PLAY_PAUSE          0
#define NEXT                1
#define PREVIOUS            2
#define SPEED_X1            3
#define SPEED_X2            4
#define SPEED_X05           5
#define NOT_VALID_GESTURE   8
#define NOT_MOVEMENT        9

class Gesture {
    private:
        int previousGesture;
        int newGesture;

    public:
        /*
        * Constructor de la clase Gesture.
        */
        Gesture() {
            this->previousGesture = (int) NOT_MOVEMENT;
            this->newGesture = (int) NOT_MOVEMENT;
        }

        int processGesture(){


            return this->newGesture;
        }

}

#endif
