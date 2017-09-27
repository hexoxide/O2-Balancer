#include "O2/ResultManager.h"
#include <iostream>

using namespace O2;


ResultManager::ResultManager(const std::string& filename){
    this->output.open(filename);
    if(!this->output.is_open() || !this->output.good()){
        throw "Not open";
    }
    //this->last =  last = std::chrono::system_clock::now();
}


void ResultManager::addTimeFrame(uint16_t frame){
   // auto end = std::chrono::system_clock::now();
  //  auto elapsed_seconds = end - last;
  //  this->output << frame << "," << elapsed_seconds.count() << "\n";
   // this->last = end;
}


ResultManager::~ResultManager(){
    this->output.close();
}