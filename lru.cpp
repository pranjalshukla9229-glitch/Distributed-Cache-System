#include<iostream>
#include<unordered_map>
#include<string>
#include "lru.h"
using namespace std;

Node :: Node(string k,int v){
    key=k;
    value=v;

    next=prev=nullptr;
}

LRUcache :: LRUcache(int cap){
    capacity=cap;
    
    head=new Node(" ",-1);
    tail=new Node(" ",-1);

    head->next=tail; //dummy nodes to avoid edge cases
    tail->prev=head;

}

void LRUcache :: remove_Node(Node* node){

    node->prev->next=node->next;
    node->next->prev=node->prev;

}

void LRUcache :: insert_front(Node*node){

    node->next=head->next;
    head->next->prev=node;
    head->next=node;
    node->prev=head;

}

int LRUcache :: get(string key){

    if(mp.find(key)==mp.end()){
        return -1;
    }  
    Node* newnode=mp[key];
    
    remove_Node(newnode);
    insert_front(newnode);

    return newnode->value;
}

void LRUcache :: put(string key,int val){

    if(mp.find(key)!=mp.end()){ //if key alwready exists
        Node*newnode=mp[key];
        newnode->value=val;
        remove_Node(newnode);
        insert_front(newnode);
        return;
    }

    if(mp.size()==capacity){
        Node* lastnode=tail->prev;
        mp.erase(lastnode->key);
        remove_Node(lastnode);
        delete lastnode;
    }
    Node*newnode=new Node(key,val);
    insert_front(newnode);
    mp[key]=newnode;
}

void LRUcache :: display(){
    Node*temp=head->next;
    cout<<"CACHE MEMORY : ";
    while(temp!=tail){
        cout<<temp->key<<"->"<<temp->value<<" ";
        temp=temp->next;
    }
    cout<<endl;
}

vector<pair<string,int>> LRUcache:: getAllData(){
    vector<pair<string,int>>data;

    Node* curr=head->next;

    while(curr!=tail){
        data.push_back({curr->key,curr->value});
        curr=curr->next;
    }
    return data;
}