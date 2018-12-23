#ifndef READFILE_H
#define READFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include "Structure.h"

using namespace std;

class ReadFile {
public:
    static Information readDataFile(string filename) {
        ifstream ifs(filename.c_str());
        int count = 0;
        int total_fac_count, total_cus_count;
        int fac_count = 0, cus_count = 0;
        vector<Factory> fac_list;
        vector<Customer> cus_list;

        ifs >> total_fac_count >> total_cus_count;

        for(int i = 0; i < total_fac_count; i++) {
            double cap, cost;
            ifs >> cap >> cost;
            Factory fac(i, (int)cap, (int)cost);
            fac_list.push_back(fac);
        }

        for(int i = 0; i < total_cus_count; i++) {
            double need;
            ifs >> need;
            Customer cus(i, (int)need);
            cus_list.push_back(cus);
        }

        for(int i = 0; i < total_fac_count; i++) {
            for(int j = 0; j < total_cus_count; j++) {
                double cost;
                ifs >> cost;
                cus_list[j].costs.push_back(cost);
            }
        }

        Information info(total_fac_count, total_cus_count, fac_list, cus_list);
        return info;
    }

    static void writeDataFile(const Information& info, const int& totalCost,
                              const vector<int>& Y, const vector<int>& cusToFac, string filename = "./result_Greedy.txt") {
        ofstream ofs;
        ofs.open(filename.c_str(), ios::out | ios::app);
        ofs << "=" << endl << totalCost << endl;
        for(int i = 0; i < Y.size(); i++) {
            ofs << Y[i] << " ";
        }
        ofs << endl;
        int show = 0;
        for(int i = 0; i < cusToFac.size(); i++) {
            ofs << cusToFac[i] << " ";
        }
        ofs << endl << "=" << endl;
    }
};

class showData {
public:
    static void display(const Information& info, const int& totalCost,
                        const vector<int>& Y, const vector<int>& cusToFac, string filename = "./result_Greedy.txt") {
        // 输出结果
        /*cout << "result: " << endl << totalCost << endl;
        cout << "isOpen: " << endl;
        for(int i = 0; i < Y.size(); i++) {
            cout << Y[i] << " ";
        }
        cout << endl << "Customer to Factory:" << endl;
        int show = 0;
        for(int i = 0; i < cusToFac.size(); i++) {
            show++;
            cout << cusToFac[i] << " ";
            if(show == 10) {
                cout << endl;
                show = 0;
            }
        }*/
        ReadFile::writeDataFile(info, totalCost, Y, cusToFac, filename);
    }

    static void displayList(const Information& info) {
        cout << info.total_fac_count << " " << info.total_cus_count << endl;
        for(int i = 0; i < info.total_fac_count; i++) {
            cout << info.fac_list[i];
        }
        for(int i = 0; i < info.total_cus_count; i++) {
            cout << info.cus_list[i];
        }
    }
};

#endif