#include<iostream>
#include"cache_node.h"
#include<string>

cache_node :: cache_node(string id,int capacity): node_id(id),cache(capacity){

}
void cache_node :: put(string key,int val){
    cache.put(key,val);
}

int cache_node :: get(string key){
    return cache.get(key);
}

void cache_node :: display(){
    cout<<"cache Node: "<<node_id<<endl;
    cache.display();
    cout<<endl;
}

vector<pair<string,int>> cache_node :: getNodeData(){
    return cache.getAllData();
}