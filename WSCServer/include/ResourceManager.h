#ifndef RESOUCEMANAGER_H
#define RESOUCEMANAGER_H

#include <map>
#include <string>

#define RESOURCE_DIR "resource/"
#define RESOURCE_MAP_PATH RESOURCE_DIR"/map.dat"
#define RES_NOVAL "NoVal"

class ResourceManager
{
    public:
        static ResourceManager *Instance();
        virtual ~ResourceManager();

        const std::string & GetResourceByKey(int p_iKey);
        char *ReadResourceFile(const std::string & p_sPath);
        const std::string ResourceMapAsString();
    protected:
    private:
        ResourceManager();
        void LoadResourceMap();

        std::map<int,std::string> m_xResourceMap;

        static ResourceManager *m_pxInstance;
};

#endif // RESOUCEMANAGER_H
