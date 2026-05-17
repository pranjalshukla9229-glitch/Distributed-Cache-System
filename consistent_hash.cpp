#include"consistent_hash.h"

unsigned long long consistent_hash :: hash_function(string key){
    return hash<string>{}(key);
}

void consistent_hash :: add_node(cache_node* node){
    unsigned long long hash_value=hash_function(node->node_id);
    ring[hash_value]=node;

    cout<<"Node Added: "<<"Node ID: "<<node->node_id<<" Hash: "<<hash_value<<endl;
}

void consistent_hash :: remove_node(string node_id){
    unsigned long long hash_value=hash_function(node_id);
    if(ring.find(hash_value) != ring.end()){
        ring.erase(hash_value);
        cout<<"Node Removed:"<<"Node ID: "<<node_id<<endl;
    }
    else{
        cout<<"Node Not Found"<<endl;
    }
}

cache_node* consistent_hash :: get_node(string key){
    if(ring.empty()){
        cout<<"Ring is Empty"<<endl;
        return nullptr;
    }
    unsigned long long hash_value=hash_function(key);
    cout<<"Key: "<<key<<" Key Hash: "<<hash_value<<" ";

    auto it=ring.lower_bound(hash_value);

    if(it==ring.end()){
        it=ring.begin();
    }
    cout<<" Mapped Node: "<<it->second->node_id<<endl;
    return it->second;
}

void consistent_hash :: display_ring(){
    cout<<"HASH RING: ";
    for(auto &it : ring){
        cout<<it.second->node_id<<" -> "<<endl;
    }
}