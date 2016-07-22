#ifndef MOVE_H
#define MOVE_H
#include <QVector>
#include <ostream>
enum direction{ Left=0, Right=1, Up=2, Down=3, Nothing =4};

class move{
public:
    move();
    ~move();

    bool canGoOn();

    unsigned int startX, startY, endX, endY;
    direction MoveDirection;
    QVector< direction> nextSteps;
};


std::ostream& operator<<(std::ostream& stream, const move& value);
/*
{
    stream << "StartX: " << value.startX << " StartY: " << value.startY << " endX " << value.endX << " endY: " << value.endY << std::endl;
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
    stream << "Next Directions";
    for(int i=0; i<value.nextSteps.size(); ++i){
        stream << value.nextSteps[i];
    }
    return stream;
}
*/



#endif // MOVE_H
