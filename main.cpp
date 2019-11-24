#include <bits/stdc++.h>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

const int X = 21;
const int Y = 23;

enum Orientation { 
    forward = 0,
    forwardLeft = 1,
    backLeft = 2,
    back = 3, 
    backRight = 4,
    forwardRight = 5
};

enum Speed {
    stop = 0,
    normal = 1,
    fast = 2
};

enum EntityType {ship, barrel, mine, cannonball};

struct Coord {
    int x = 0; 
    int y = 0;

    pair<int,int> evenNeighbor[6] = {{1, 0}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}};
    pair<int,int> oddNeighbor[6] = {{1, 0}, {1, -1}, {0, -1}, {-1, 0}, {0, 1}, {1, 1}};

    Coord() {
        x = y = 0;
    }

    Coord(int x, int y) {
        this -> x = x;
        this -> y = y;
    }

    Coord(const Coord &coord) {
        x = coord.x;
        y = coord.y;
    }

    bool isEvenRow() {
        return !(x&1);
    } 

    bool isValid() {
        return (x>-1 && x<X && y<Y && y>-1);
    }

    Coord getNeighbor(Orientation orientation) {
        switch(isEvenRow()) {
            case true: {
                int dx = evenNeighbor[orientation].first;
                int dy = evenNeighbor[orientation].second;
                return Coord(x + dx, y + dy);
                break;
            }
            case false: { 
                int dx = oddNeighbor[orientation].first;
                int dy = oddNeighbor[orientation].second;
                return Coord(x + dx, y + dy);
                break;
            }
        }
    }
};

class Entity {
    public: 
    int entityId;
    EntityType entityType;
    Coord coord;

    Entity(int entityId, EntityType entityType, Coord coord) {
        this -> entityId = entityId;
        this -> entityType = entityType;
        this -> coord = coord;
    }
};

class Ship: public Entity {
    public: 
    Orientation orientation;
    Speed speed;
    int stockLevel;

    Ship(int entityId, Orientation orientation, Coord coord, Speed speed, int stockLevel)
    : Entity(entityId, EntityType::ship, coord) {
        this -> orientation = orientation;
        this -> speed = speed;
        this -> stockLevel = stockLevel;
    }
};

class Barrel: public Entity {
    public: 
    int size;
    bool visited = false;

    Barrel(int entityId, Coord coord, int size): Entity(entityId, EntityType::barrel, coord) {
        this -> size = size;
    }

    void visit() {
        visited = true;
    }

    friend bool operator<(const Barrel &a,const Barrel &b) {
       return tie(a.size, a.coord.x, a.coord.y) < tie(b.size, b.coord.x, b.coord.y);
    }
};

class Mine: public Entity {
    public: 
    Mine(int entityId, Coord coord): Entity(entityId, EntityType::mine, coord) {}
};

class Cannonball: public Entity {
    public:
    int remainingTours;
    int shipId; 

    Cannonball(int entityId, Coord coord, int remainingTours, int shipId): Entity(entityId, EntityType::cannonball, coord) {
        this -> remainingTours = remainingTours;
        this -> shipId = shipId;
    }
};

vector<Barrel> barrels;
vector<Ship> myShips, ennemieShips;
vector<Mine> mines;
vector<Cannonball> cannonballs;

double getDistance(Entity a, Entity b) {
    return abs(a.coord.x-b.coord.x) + abs(a.coord.y - b.coord.y);
}

Barrel getNearestBarrel(Ship ship) {
    if(barrels.empty()) return Barrel(0, Coord(ship.coord.x, ship.coord.y), 0);
    Barrel nearestBarrel = barrels[0];
    int minDist = X + Y;
    for(auto barrel: barrels) {
        int distance = getDistance(ship, barrel);
        if((distance < minDist) && !barrel.visited) {
            minDist = distance;
            nearestBarrel = barrel;
        } else if(distance == minDist) {
            if(barrel.size > nearestBarrel.size) {
                nearestBarrel = barrel; 
            }
        }
    }
    if(minDist == 0)
        nearestBarrel.visit();
    return nearestBarrel;
}




int main()
{

    // game loop
    while (1) {
        myShips.clear();
        barrels.clear();
        ennemieShips.clear();
        mines.clear();
        cannonballs.clear();
        int myShipCount; // the number of remaining ships
        cin >> myShipCount; cin.ignore();
        int entityCount; // the number of entities (e.g. ships, mines or cannonballs)
        cin >> entityCount; cin.ignore();
        for (int i = 0; i < entityCount; i++) {
            int entityId;
            string entityType;
            int x;
            int y;
            int arg1;
            int arg2;
            int arg3;
            int arg4;
            cin >> entityId >> entityType;
            cin >> x >> y >> arg1 >> arg2 >> arg3 >> arg4;
            cin.ignore();
            if(entityType == "SHIP") {
                if(arg4) {
                    myShips.push_back(Ship(entityId, (Orientation)arg1, Coord(x, y), (Speed)arg2, arg3));
                } else {
                    ennemieShips.push_back(Ship(entityId, (Orientation)arg1, Coord(x, y), (Speed)arg2, arg3));
                }
            } else if(entityType == "BARREL") {
                barrels.push_back(Barrel(entityId, Coord(x, y), arg1));
            } else if(entityType == "MINE") {
                mines.push_back(Mine(entityId, Coord(x, y)));
            } else if(entityType == "CANNONBALL") {
                cannonballs.push_back(Cannonball(entityId, Coord(x, y), arg2, arg1));
            } else return -1;
        }
        for (int i = 0; i < myShipCount; i++) {

            // Write an action using cout. DON'T FORGET THE "<< endl"
            // To debug: cerr << "Debug messages..." << endl;
            // We suppose we have one ship per player
            Ship myShip = myShips[0];
            Barrel destBarrel = getNearestBarrel(myShip);
            cerr<<myShip.coord.x<<" "<<myShip.coord.y<<endl;
            cout << "MOVE "<<destBarrel.coord.x<<" "<< destBarrel.coord.y<< endl; // Any valid action, such as "WAIT" or "MOVE x y"
        }
    }
}