#ifndef ALGORITHM2_H
#define ALGORITHM2_H

#include "Structure.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

class SA {
public:
    static int run(const Information& info){
        vector<int> cusToFac(info.total_cus_count);
        vector<int> f_rest(info.total_fac_count);
        vector<int> isOpen(info.total_fac_count);
        int totalCost = 0;

        for(int i = 0; i < info.total_fac_count; i++) {
            f_rest[i] = info.fac_list[i].capacity;
        }

        srand(time(NULL));

        for(int customer = 0; customer < info.total_cus_count; customer++){
            int f = rand() % info.total_fac_count;
            while(f_rest[f] < info.cus_list[customer].need){
                f = rand() % info.total_fac_count;
            }
            if(isOpen[f] == 0) {
                isOpen[f] = 1;
            }
            f_rest[f] -= info.cus_list[customer].need;
            cusToFac[customer] = f;
        }

        State InitialState(cusToFac, f_rest, isOpen), temp;
        temp = InitialState;

        double T = 100, end_T = 0.001, d = 0.99;
        int temp_cost = getTempCost(info, InitialState);

        while(T >= end_T) {
            int iteration = 0;
            while(iteration++ <= 1000) {
                State newState = getNewState(info, temp);
                double Ek = getTempCost(info, newState) - temp_cost;
                if( Ek < 0 ) {
                    temp = newState;
                    temp_cost = temp_cost + Ek;
                }
                else {
                    double p = exp(-Ek / T);
                    double R = rand() % 999 / 1000.0f;
                    if(R < p) {
                        temp = newState;
                        temp_cost = temp_cost + Ek;
                    }
                }
            }
            double t = rand() % 999 / 1000.0f;
            if(t > 0.9) {
                T *= (2-d);
            }
            else {
                T *= d;
            }
        }

        isOpen = temp.n_isOpen;
        cusToFac = temp.n_cusToFac;
        // 统计花费
        for(int factory = 0; factory < info.total_fac_count; factory++) {
            totalCost += info.fac_list[factory].cost * isOpen[factory];
        }
        for(int customer = 0; customer < info.total_cus_count; customer++) {
            totalCost += info.cus_list[customer].costs[cusToFac[customer]];
        }

        // 显示
        showData::display(info, totalCost, isOpen, cusToFac, "./result_SA.txt");
        return totalCost;
    }

    static State getNewState(const Information& info, const State& curState) {
        State newS_1 = trans1(info, curState);
        State newS_2 = trans2(info, newS_1);
        State newS_3 = trans1(info, newS_2);
        State newS_4 = trans1(info, newS_3);
        return newS_4;
    }

    static State trans1(const Information& info, const State& curState) {
        State newS = curState;
        int c1 = rand() % info.total_cus_count,
            f2 = rand() % info.total_fac_count;
        while(curState.n_cusToFac[c1] == f2 || curState.n_f_rest[f2] < info.cus_list[c1].need) {
            f2 = rand() % info.total_fac_count;
        }
        int f1 = curState.n_cusToFac[c1];
        newS.n_f_rest[f1] += info.cus_list[c1].need;
        newS.n_f_rest[f2] -= info.cus_list[c1].need;
        if(newS.n_isOpen[f2] == 0) {
            newS.n_isOpen[f2] = 1;
        }
        if(newS.n_f_rest[f1] == info.fac_list[f1].capacity){
            newS.n_isOpen[f1] = 0;
        }
        newS.n_cusToFac[c1] = f2;

        return newS;
    }

    static State trans2(const Information& info, const State& curState) {
        State newS = curState;
        int c1 = rand() % info.total_cus_count,
            c2 = rand() % info.total_cus_count;

        int f1 = curState.n_cusToFac[c1],
            f2 = curState.n_cusToFac[c2];
        while(c1 == c2 || curState.n_f_rest[f1] + info.cus_list[c1].need < info.cus_list[c2].need ||
              curState.n_f_rest[f2] + info.cus_list[c2].need < info.cus_list[c1].need) {
            c1 = rand() % info.total_cus_count;
            c2 = rand() % info.total_cus_count;
            f1 = curState.n_cusToFac[c1];
            f2 = curState.n_cusToFac[c2];
        }
        newS.n_f_rest[f1] += info.cus_list[c1].need - info.cus_list[c2].need;
        newS.n_f_rest[f2] += info.cus_list[c2].need - info.cus_list[c1].need;
        newS.n_cusToFac[c1] = f2;
        newS.n_cusToFac[c2] = f1;
        return newS;
    }

    static int getTempCost(const Information& info, State& curState) {
        int totalCost = 0;
        for(int factory = 0; factory < info.total_fac_count; factory++) {
            totalCost += info.fac_list[factory].cost * curState.n_isOpen[factory];
        }
        for(int customer = 0; customer < info.total_cus_count; customer++) {
            totalCost += info.cus_list[customer].costs[curState.n_cusToFac[customer]];
        }
        return totalCost;
    }
};

#endif