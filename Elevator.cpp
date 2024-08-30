// Programmer: Erin Lee
// Student id: 1805820
// File: Elevator.cpp

#include "Elevator.h"
#include "Panel.h"
#include "Building.h"
#include "Rider.h"

#include <iostream>
#include <vector>
using namespace std;

#include <cstdlib>

// constructor for elevator class
Elevator::Elevator(unsigned int capacity, int speed, int start)
        : CAPACITY(capacity), speed(speed), location(Building::floors[start])
{
  // initialize elevator metrics
  atFloorIndex = -1;
  direction = Elevator::IDLE;
  timer = 0; 

  // add buttons to panel for each floor of building
  for(int i = 0; i < Building::FLOORS; i++)
  {
    panel.addButton(Building::floors[i].label);
  }
}

// overloaded output stream operator outputs info with formatted output
ostream& operator<<(ostream& out, const Elevator& e)
{
  out << "Elevator at ";
  out.width(5);
  out << e.location;
  out.width(12);

  // determine if elevator is going up, down, or idle
  switch(e.direction)
  {
    // elevator is going up
    case Elevator::UP:
    {
      out << "going UP";
      break;
    }

    // elevator is going down
    case Elevator::DOWN:
    {
      out << "going DOWN";
      break;
    }

    // elevator is neither, so it is idle
    case Elevator::IDLE:
    {
      out << "IDLE";
      break;
    }
  }

  out.width(4);
  out << e.getNumberOfRiders() << " riders";
  out.width(17);

  // check if elevator door is open
  // if yes, display remaining time before closing
  if (e.atFloorIndex != -1)
    out << " door is OPEN|" << e.timer;

  // elevator door is not open
  else
  {
    out.width(16);
    out << " ";
  }

  // output panel info
  out << " " << e.panel;

  // return output stream
  return out;
}

// function will open elevator door at specific floor
void Elevator::openDoorTo(int f)
{
  // clears floor's button from panel
  // will set current floor index to specified door
  panel.clear(Building::floors[f].label);
  atFloorIndex = f;
  resetTimer();
}

// function will add rider to evelator
void Elevator::board(const Rider& r)
{
  // presses button to rider's destination floor on panel
  // will declare direction based on their destination
  riders.push_back(r);
  panel.press(Building::floors[r.to].label);
  direction = r.goingUp ? UP : DOWN;
  resetTimer();
}

// function checks if elevator has rider whose destination is the current floor 
bool Elevator::hasRiderForFloor() const
{
  // check if elevator is open
  // not open, there are no riders that need to get out
  if (!isOpen())
    return false;

  // move through entire vector of riders
  for (int i = 0; i < riders.size(); i++)
  {
    // check if rider's destination and current floor match
    // if yes, rider is found
    if (riders[i].to == atFloorIndex)
      return true;
  }

  // no riders found on current floor
  return false;
}

// member function removes riders from elevator
void Elevator::removeRider()
{
  // move through entire vector of riders
  for (int i = 0; i < riders.size(); i++)
  {
    // check if rider's destination and current floor of elevator match
    if (riders[i].to == atFloorIndex)
    {
      // erase rider from vector and clear panel button for current floor
      // also reset elevator's timer
      riders.erase(riders.begin() + i);
      panel.clear(Building::floors[riders[i].to].label);
      resetTimer();
      break;
    }
  }
}