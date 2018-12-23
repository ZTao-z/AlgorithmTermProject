#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Factory {
public:
    int num;
    int capacity;
    int cost;
    Factory(int n, int cap, int cos) {
        this->num = n;
        this->capacity = cap;
        this->cost = cos;
    }

    friend ostream& operator<<(ostream& os, const Factory& f) {
        os << "Factory: " << f.num << " Cap: " << f.capacity << " Cost: " << f.cost << endl;
        return os;
    }
};

class Customer {
public:
    int num;
    int need;
    vector<int> costs;
    Customer(int n, int ne) {
        this->num = n;
        this->need = ne;
    }

    friend ostream& operator<<(ostream& os, const Customer& f) {
        os << "Customer: " << f.num << " Need: " << f.need << " Costs: ";
        for(int i = 0; i < f.costs.size(); i++) {
            os << f.costs[i] << ", ";
        }
        os << endl;
        return os;
    }
};

class Information {
public:
    int total_fac_count;
    int total_cus_count;
    vector<Factory> fac_list;
    vector<Customer> cus_list;
    Information() {
        total_cus_count = total_fac_count = 0;
    }
    Information(int f, int c, vector<Factory>& fl, vector<Customer>& cl) {
        total_fac_count = f;
        total_cus_count = c;
        fac_list = fl;
        cus_list = cl;
    }
};

class State {
public:
    vector<int> n_cusToFac;
    vector<int> n_f_rest;
    vector<int> n_isOpen;
    bool isValid;
    State(){
        isValid = true;
    }
    State(vector<int>& c, vector<int>& f, vector<int>& i) {
        n_cusToFac = c;
        n_f_rest = f;
        n_isOpen = i;
        isValid = true;
    }
    State(bool c) {
        isValid = c;
    }
};

#endif