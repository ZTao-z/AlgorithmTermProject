#ifndef ALGORITHM1_H
#define ALGORITHM1_H

#include "Structure.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Greedy {
public:

    static int run(const Information& info){
        vector<bool> restrict_f(info.total_fac_count);
        for(int i = 0; i < info.total_fac_count; i++){
            restrict_f[i] = true;
        }
        State result = findR(info, restrict_f);
        // 显示
        showData::display(info, getStateCost(info, result), result.n_isOpen, result.n_cusToFac, "./result_Greedy.txt");
        return getStateCost(info, result);
    }

    static State findR(const Information& info, vector<bool>& restrict_f) {
        State res;
        State p = findResult(info, restrict_f);
        if(!p.isValid) {
            return p;
        }
        int c = getStateCost(info, p);
        int rC = c;
        int pos = -1;
        res = p;
        for(int i = 0; i < info.total_fac_count; i++) {
            if(restrict_f[i]) {
                restrict_f[i] = false;
                State temp = findResult(info, restrict_f);
                if(temp.isValid) {
                    int tC = getStateCost(info, temp);
                    if(rC > tC){
                        rC = tC;
                        res = temp;
                        pos = i;
                    }
                }
                restrict_f[i] = true;
            }
        }
        if(pos != -1){
            restrict_f[pos] = false;
            return findR(info, restrict_f);
        }
        else{
            return res;
        }
    }

    static State findResult(const Information& info, vector<bool> restrict_f) {
        vector<int> cusToFac(info.total_cus_count);
        vector<int> f_rest(info.total_fac_count);
        vector<int> isOpen(info.total_fac_count);
        int totalCost = 0;

        for(int i = 0; i < info.total_fac_count; i++) {
            f_rest[i] = info.fac_list[i].capacity;
        }

        for(int customer = 0; customer < info.total_cus_count; customer++) {
            /*找最近工厂*/
            vector<int> cost_list = info.cus_list[customer].costs;
            vector<int> rec;
            int actual_f_num = -1;
            sort(cost_list.begin(), cost_list.end());
            for(int c = 0; c < cost_list.size(); c++) {
                vector<int>::iterator ptr = find(rec.begin(), rec.end(), cost_list[c]);
                if(ptr == rec.end()) {
                    vector<int> temp_fac_list;
                    rec.push_back(cost_list[c]);

                    // 找到最近的工厂
                    for(int i = 0; i < info.cus_list[customer].costs.size(); i++) {
                        if(info.cus_list[customer].costs[i] == cost_list[c] && restrict_f[i]) {
                            temp_fac_list.push_back(i);
                        }
                    }
                    // 判断该工厂是否有空位
                    int f_num = -1, minFacCost = 1000000;
                    for(int i = 0; !temp_fac_list.empty() && i < temp_fac_list.size(); i++) {
                        int n = temp_fac_list[i];
                        if(f_rest[n] >= info.cus_list[customer].need && info.fac_list[n].cost < minFacCost) {
                            f_num = n;
                            minFacCost = info.fac_list[n].cost;
                        }
                    }
                    // 将顾客放入工厂
                    if(f_num != -1) {
                        actual_f_num = f_num;
                        break;
                    }
                }
            }
            if(actual_f_num == -1){
                return State(false);
            }
            else {
                if(isOpen[actual_f_num] == 0) {
                    isOpen[actual_f_num] = 1;
                }
                cusToFac[customer] = actual_f_num;
                f_rest[actual_f_num] -= info.cus_list[customer].need;
            }
        }

        State res(cusToFac, f_rest, isOpen);
        return res;
    }

    static int getStateCost(const Information& info, State& curState) {
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