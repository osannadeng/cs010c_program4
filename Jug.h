#pragma once
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using std::string;
using std::istreambuf_iterator;
using std::ifstream;

// default value for state parameters before set
const unsigned infinity = std::numeric_limits<unsigned>::max();

struct State {
   unsigned a; // amount in jug A
   unsigned b; // amount in jug B

   // where to go on each of the six actions
   unsigned fillA;
   unsigned fillB;
   unsigned emptyA;
   unsigned emptyB;
   unsigned AtoB;
   unsigned BtoA;

   // for Dijkstra's method
   unsigned previous;
   unsigned distance;
   bool visited;

   // state constructor -> parameters not filled out default
   State(unsigned a = infinity, unsigned b = infinity,
	      unsigned fillA = infinity, unsigned fillB = infinity,
	      unsigned emptyA = infinity, unsigned emptyB = infinity,
	      unsigned AtoB = infinity, unsigned BtoA = infinity)
         : a(a), b(b),
         fillA(fillA), fillB(fillB),
         emptyA(emptyA), emptyB(emptyB),
         AtoB(AtoB), BtoA(BtoA),
         previous(infinity), distance(infinity),
         visited(false) {}
};

class Jug {
 private:
    std::vector<State> graph;
    unsigned Ca;	// max gallons in jug A
    unsigned Cb;	// gallons in jug B
    unsigned N;	// target state to reach/gallons to get in jug B
    unsigned cfA; // cost to fill jug A
    unsigned cfB;	// cost to fill jug B
    unsigned ceA;	// cost to empty jug A
    unsigned ceB;	// cost to empty jug B
    unsigned cpAB;   // cost to pour from A to B
    unsigned cpBA;	// cost to pour from B to A

 public:
    Jug(unsigned inCa, unsigned inCb, unsigned inN, unsigned incfA, unsigned incfB, unsigned inceA, unsigned inceB, unsigned incpAB, unsigned incpBA);

    // solve is used to check input and find the solution if one exists
    // returns -1 if invalid inputs. solution set to empty string.
    // returns 0 if inputs are valid but a solution does not exist. solution set to empty string.
    // returns 1 if solution is found and stores solution steps in solution string.
    int solve(string &solution);
 private:
    unsigned state_number(unsigned a, unsigned b, unsigned Ca, unsigned Cb);
    void fillGraph();
    void setDistances(unsigned start);
};