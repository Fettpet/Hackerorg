#include "move.h"

move::move(){
    startX = -1;
    startY = -1;
    endX = -1;
    endY = -1;

    MoveDirection = Nothing;
}
move::~move() {

}

bool move::canGoOn(){
    return !nextSteps.isEmpty();
}

std::ostream& operator<<(std::ostream& stream, const move& value){
    stream << "StartX: " << value.startX << " StartY: " << value.startY << " endX " << value.endX << " endY: " << value.endY;
    stream << " moveDirection ";
    if(value.MoveDirection == Right){
        stream << "Right";
    }
    if(value.MoveDirection == Nothing){
        stream << "Nothing";
    }
    if(value.MoveDirection == Left){
        stream << "Left";
    }
    if(value.MoveDirection == Up){
        stream << "Up";
    }
    if(value.MoveDirection == Down){
        stream << "Down";
    }
    stream << "Next Directions(" << value.nextSteps.size() <<")";
    for(int i=0; i<value.nextSteps.size(); ++i){
        if(value.nextSteps[i] == Right){
            stream << " Right";
        }
        if(value.nextSteps[i]  == Left){
            stream << "Left";
        }
        if(value.nextSteps[i]  == Up){
            stream << "Up";
        }
        if(value.nextSteps[i]  == Down){
            stream << "Down";
        }
    }
    return stream;
}

