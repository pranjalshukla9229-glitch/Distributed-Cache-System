#ifndef LRU_H
#define LRU_H
#include<iostream>
#include<unordered_map>
#include<string>
#include<vector>
using namespace std;

class Node{
    public:
        string key;
        int value;

        Node*next;
        Node*prev;

        Node(string key,int value);
};

class LRUcache{
    private:
        int capacity;
        unordered_map<string,Node*>mp;

        Node*head;
        Node*tail;

        void remove_Node(Node* node);
        void insert_front(Node* node);
    
    public:
        LRUcache(int capacity);

        int get(string key);
        void put(string key,int val);
        void display();
        vector<pair<string,int>>getAllData();

};
#endif