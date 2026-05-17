#include<iostream>
#include"distributed_cache.h"
#include<vector>
using namespace std;


void distributed_cache :: add_node(string id,int capacity){
    cache_node* newnode=new cache_node(id,capacity);

    nodes.push_back(newnode);
    hashring.add_node(newnode);
}

void distributed_cache :: remove_node(string id){
    cache_node*removeNode=nullptr;

    for(auto it:nodes){
        if(it->node_id==id){
            removeNode=it;
            break;
        }
    }

    if(removeNode==nullptr){
        cout<<" Node Not Found "<<endl;
        return;
    }

    vector<pair<string,int>>data = removeNode->getNodeData();
    hashring.remove_node(id);

    for(auto it=nodes.begin();it!=nodes.end();it++){
        if((*it)->node_id == id){
            nodes.erase(it);
            break;
        }
    }

    for(auto &it : data){
        string key=it.first;
        int value=it.second;

        cache_node* newnode=hashring.get_node(key);
        if(newnode != nullptr){
            newnode->put(key,value);
        }
    }
    delete removeNode;
    cout<<" Node Removed Successfully "<<endl;
}

void distributed_cache :: put(string key,int val){
    cache_node* targetNode=hashring.get_node(key);

    if(targetNode == nullptr){
        cout<<"No Node Available "<<endl;
        return;
    }
    targetNode->put(key,val);
}

int distributed_cache :: get(string key){
    cache_node* targetNode=hashring.get_node(key);

    if(targetNode==nullptr){
        cout<<"No Node Available "<<endl;
        return -1;
    }
    return targetNode->get(key);
}

void distributed_cache :: display(){
    cout<<"DISTRIBUTED CACHE SYSTEM  "<<endl;
    hashring.display_ring();

    for(cache_node* it:nodes){
        it->display();
    }

}