Robot ::Robot(std::string name, int hp, int atk, double hit_rate)
{
    Name=name;
    Health=hp;
    Attack=atk;
    HitRate=hit_rate;
}
bool Robot::Survive()
{
    if(Health>0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void Robot::Hit(Robot &target)
{

}
void Robot::Hit(Building &target)
{

}
Building::Building(std::string name, int hp)
{
    Name=name;
    Health=hp;
    Invincible=false;
    Protected=false;
}
bool Building::Survive()
{
    if(Health>0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}