#ifndef CONSISTENT_HASH_H
#define CONSISTENT_HASH_H
#include "cache_node.h"
#include<iostream>
#include<string>
#include<map>
#include<functional>
using namespace std;

class consistent_hash{
    private:
        map<unsigned long long,cache_node*> ring;
    public:
        void add_node(cache_node* node);
        void remove_node(string node_id);
        cache_node* get_node(string key);
        void display_ring();
        unsigned long long hash_function(string key);
};


#endif