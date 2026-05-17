#include<iostream>
#include"lru.h"
#include<string>
#ifndef CACHE_NODE_H
#define CACHE_NODE_H
using namespace std;

class cache_node{
    public:
        string node_id;
        LRUcache cache;
        void put(string key,int val);
        int get(string key);
        void display();
        cache_node(string id,int capacity);
        vector<pair<string,int>>getNodeData();
};
#endif