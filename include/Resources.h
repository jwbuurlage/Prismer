//Base class for TextureManager, ModelManager, SoundManager and so on
//This class supplies public functions:
//      getResource - returns resource if loaded, or calls loadResource if not
//      unloadAll - deletes all resources
//      resourceLoaded - check if a resource is loaded
//The sub class must implement only 'loadResource'
//      This implementation must call addResource() to add it to the resource list

#pragma once
#include <string>
#include <map>

using std::string;
using std::multimap;

namespace Arya
{
    template <typename T> class ResourceManager {
        public:
            ResourceManager(){ defaultResource = 0; };
            virtual ~ResourceManager(){ unloadAll(); }

            //Will load the resource if not already loaded
            T* getResource( string filename )
            {
                ResourceContainer::iterator iter = resources.find(filename);
                if( iter != resources.end() )
                    return (T*)(iter->second);
                T* ret = loadResource(filename);
                if(ret) return ret;
                return defaultResource;
            }

            void unloadAll()
            {
                for( ResourceContainer::iterator iter = resources.begin(); iter != resources.end(); ++iter ){
                    T* resource = (T*)iter->second;
                    delete resource; //This will call the deconstructor
                }
                resources.clear();
            }

            bool resourceLoaded( string name ){
                ResourceContainer::iterator iter = resources.find(name);
                return (iter != resources.end());
            }

        private:
            typedef multimap<string,void*> ResourceContainer;
            ResourceContainer resources;

        protected:
            //Must be implemented by subclass and use addResource to add the resource
            virtual T* loadResource( string filename )=0;

            T* defaultResource;

            void addResource( string name, T* res ){
                resources.insert( ResourceContainer::value_type( name, res ) );
            }
    };
}
