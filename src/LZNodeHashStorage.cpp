//
//  LZNodeHashStorage.cpp
//  max-external
//
//  Created by Benjamin Smith on 11/11/18.
//

#include <stdio.h>

#include "../include/algs/lzms/LZNodeHashStorage.hpp"

using namespace vmm_algs_lzms;

void LZNodeHashStorage::put(int key, LZmsNode* node) {
    //            if (storage == NULL) {
    //                storage = new TIntObjectHashMap(2);
    //            }
    storage.insert( std::pair<int, LZmsNode* >(key, node) );
}

LZmsNode* LZNodeHashStorage::get(int key) {
    //        if (storage == null) {
    //          return null;
    //        }
    std::map< int, LZmsNode* >::iterator it = storage.find(key);
    if (it != storage.end())
        return (LZmsNode*) it->second;
    else
        return NULL;
}

std::vector<LZmsNode*>* LZNodeHashStorage::toArray() {
    /**@ron handle null storage(!)*/
    //            LZNode[] result = new LZNode[objects.length];
    std::vector<LZmsNode*>* result = new std::vector<LZmsNode*>();
    
    for (std::map< int, LZmsNode* >::iterator it = storage.begin(); it != storage.end(); it++)
        result->push_back(it->second);
    //            System.arraycopy(objects, 0, result, 0, objects.length);
    
    return result;
    }
