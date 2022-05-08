#ifndef Headers_H_
#define Headers_H_

class Ant
{

    pos coords;
public:
    Ant();
    ~Ant();

    virtual void Move();
    virtual void Attack();

};

class AntWarrior : Ant
{
    //
};
class AntRanged : Ant
{
    //
};
class AntTank : Ant
{

};

class pos
{
public:

    int x;
    int y;

    pos(int x, int y)
    : x(x),y(y)
    {}
    pos(pos given)
    : x(given.x), y(given.y)
    {}

};

class Uroven
{
    const int attackAdd;
    const int healthAdd;

public:
    Uroven(int attackAdd, int healthAdd)
    : attackAdd(attackAdd), healthAdd(healthAdd)
    {}

    

};

class Pravek : Uroven
{

};
class Starovek : Uroven
{

};
class Novovek : Uroven
{

};

#endif