#include "ResourceManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

ResourceManager *ResourceManager::m_pxInstance=0;

ResourceManager *ResourceManager::Instance(){
    if(m_pxInstance==0){
        m_pxInstance=new ResourceManager();
    }
    return m_pxInstance;
}

ResourceManager::ResourceManager()
{
    LoadResourceMap();
}

ResourceManager::~ResourceManager()
{
    //dtor
}

void ResourceManager::LoadResourceMap(){
    std::fstream xF;
    xF.open(RESOURCE_MAP_PATH,std::fstream::in);
    if(!xF.is_open()){return;}
    int iKey=-1;
    std::string sVal;
    std::cout<<"Reading resource map file..\n";
    while(!xF.eof()){
        xF>>std::skipws>>iKey>>sVal;
        std::cout<<iKey<<": "<<sVal<<"\n";
        m_xResourceMap[iKey]=sVal;
    }
}

const std::string & ResourceManager::GetResourceByKey(int p_iKey){
    if(m_xResourceMap.find(p_iKey)==m_xResourceMap.end()){
        return RES_NOVAL;
    }

    return m_xResourceMap[p_iKey];
}
char *ResourceManager::ReadResourceFile(const std::string & p_Path){
        std::string sPath=RESOURCE_DIR+p_Path;
         std::fstream xF(sPath.c_str(),std::fstream::in);
        if(!xF.is_open()){return NULL;}
        int iLen=0;
        xF.seekg(0,xF.end);
        iLen=xF.tellg();
        xF.seekg(0,xF.beg);
        char *buff=new char[iLen+1];
        xF.read(buff,iLen);
        xF.close();
        buff[iLen]='\0';
        return buff;
}
const std::string ResourceManager::ResourceMapAsString(){
        std::string sResult;
        std::ostringstream xTest;
      //  char sTmp[sizeof(int)*8+1];
        std::map<int,std::string>::iterator it=m_xResourceMap.begin();
        for(;it!=m_xResourceMap.end();++it){
            xTest<<it->first<<","<<it->second<<"\n";
        }
        sResult=xTest.str();
        return sResult;
}
