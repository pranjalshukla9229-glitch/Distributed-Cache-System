#ifndef DISTRIBUTED_CACHE_H
#define DISTRIBUTED_CACHE_H
#include"consistent_hash.h"
#include<vector>
#include<iostream>
using namespace std;

class distributed_cache{
    private:
        consistent_hash hashring;
        vector<cache_node*>nodes;
    
    public:
        void add_node(string id,int capacity);
        void remove_node(string id);
        void put(string key,int val);
        int get(string key);
        void display();
};



#endif