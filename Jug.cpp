#include "Jug.h"

// #include <iomanip>

using std::cout;
using std::endl;

// jug constructor
Jug::Jug(unsigned inCa, unsigned inCb, unsigned inN, unsigned incfA, unsigned incfB, unsigned inceA, unsigned inceB, unsigned incpAB, unsigned incpBA)
    : Ca(inCa), Cb(inCb), N(inN), cfA(incfA), cfB(incfB), ceA(inceA), ceB(inceB), cpAB(incpAB), cpBA(incpBA) {}

// populate graph with all possible states
void Jug::fillGraph() {
    for(unsigned a = 0; a <= Ca; ++a) { // jug a
        for(unsigned b = 0; b <= Cb; ++b) { // jug b
            // compute action fill A -> A will have Ca gallons, B unchanged
            // compute action fill B -> A unchanged, B will have Cb gallons
            unsigned fillA = state_number(Ca,b, Ca,Cb);
            unsigned fillB = state_number(a,Cb, Ca,Cb);

            // set the appropriate amount to zero
            unsigned emptyA = state_number(0,b, Ca,Cb);
            unsigned emptyB = state_number(a,0, Ca,Cb);

            // For the transfers, we can only pour in as much as we have
            // in the "from" jug OR as much room as we have in the "to" jug
            unsigned transferFromA = std::min(a, Cb-b);
            unsigned AtoB = state_number(a - transferFromA, b + transferFromA, Ca,Cb);
            unsigned transferFromB = std::min(b, Ca-a);
            unsigned BtoA = state_number(a+transferFromB,b-transferFromB, Ca,Cb);

            // add to graph
            graph.push_back(State(a, b, fillA, fillB, emptyA, emptyB, AtoB, BtoA));
        }
    }
}

// returns state number -> used for vector index
unsigned Jug::state_number(unsigned a, unsigned b, unsigned Ca, unsigned Cb) {
  // error handling -> should not affect program
  if (a > Ca or b > Cb) {
    throw std::runtime_error("domain error");
  }
  return a * (Cb + 1) + b;
}

// used to check input and find the solution if one exists
int Jug::solve(string &solution) {
    solution = "";  // default
    // invalid inputs -> returns -1
    if (N > 1000 || Cb > 1000 || Ca > Cb || N > Cb || Ca == 0 || Cb == 0) { return -1; }
    fillGraph();    // call helpers after bounds checking, etc
    setDistances(0);    // set paths
    // inputs are valid, but solution does not exist -> returns 0
    if (graph[N].distance == infinity) { return 0; }
    // if continuing, input is valid and there is a solution
    unsigned currIndex = N;
    solution = "success " + std::to_string(graph[N].distance);
    // traverse backwards and append new steps to front of string
    while (currIndex != 0) {
        // finds most recent traversal method in path, adds to front of string
        // most recent method was fill A
        if (currIndex == graph[graph[currIndex].previous].fillA) {
            solution = "fill A\n" + solution;
        }
        // most recent method was fill B
        if (currIndex == graph[graph[currIndex].previous].fillB) {
            solution = "fill B\n" + solution;
        }
        // most recent method was empty A
        if (currIndex == graph[graph[currIndex].previous].emptyA) {
            solution = "empty A\n" + solution;
        }
        // most recent method was empty B
        if (currIndex == graph[graph[currIndex].previous].emptyB) {
            solution = "empty B\n" + solution;
        }
        // most recent method was pour A B
        if (currIndex == graph[graph[currIndex].previous].AtoB) {
            solution = "pour A B\n" + solution;
        }
        // most recent method was pour B A
        if (currIndex == graph[graph[currIndex].previous].BtoA) {
            solution = "pour B A\n" + solution;
        }
        currIndex = graph[currIndex].previous;  // increment backwards
    }
    return 1;   // solution found and stored in solution string
}

// visits state and adjusts distances of other states it can directly access accordingly
void Jug::setDistances(unsigned start) {
    if (graph.size() == 0) { return; }  // graph is empty
    if (start == 0) { graph[start].distance = 0; }  // currently at origin state
    graph[start].visited = true;
    // check/set distances for fill A state
    if ((graph[start].distance + cfA) < graph[graph[start].fillA].distance) {
        graph[graph[start].fillA].distance = graph[start].distance + cfA;
        graph[graph[start].fillA].previous = start;
    }
    // check/set distances for fill B state
    if ((graph[start].distance + cfB) < graph[graph[start].fillB].distance) {
        graph[graph[start].fillB].distance = graph[start].distance + cfB;
        graph[graph[start].fillB].previous = start;
    }
    // check/set distances for empty A state
    if ((graph[start].distance + ceA) < graph[graph[start].emptyA].distance) {
        graph[graph[start].emptyA].distance = graph[start].distance + ceA;
        graph[graph[start].emptyA].previous = start;
    }
    // check/set distances for empty B state
    if ((graph[start].distance + ceB) < graph[graph[start].emptyB].distance) {
        graph[graph[start].emptyB].distance = graph[start].distance + ceB;
        graph[graph[start].emptyB].previous = start;
    }
    // check/set distances for A to B state
    if ((graph[start].distance + cpAB) < graph[graph[start].AtoB].distance) {
        graph[graph[start].AtoB].distance = graph[start].distance + cpAB;
        graph[graph[start].AtoB].previous = start;
    }
    // check/set distances for B to A state
    if ((graph[start].distance + cpBA) < graph[graph[start].BtoA].distance) {
        graph[graph[start].BtoA].distance = graph[start].distance + cpBA;
        graph[graph[start].BtoA].previous = start;
    }
    unsigned distance = infinity;
    unsigned current = infinity;
    // traverse graph to find next smallest distance state that has not been visited
    for(unsigned i = 0; i < graph.size(); ++i) {
        // if distance at index is less than smallest distance thus far and has not been visited
        if (graph[i].distance < distance && !graph[i].visited) {
            distance = graph[i].distance;
            current = i;
        }
    }
    if (distance == infinity) { return; }   // all states visited
    setDistances(current);  // recursive call
}