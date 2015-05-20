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
#include <memory>

namespace Arya
{
    using std::string;
    using std::multimap;
    using std::shared_ptr;
    using std::unique_ptr;
    using std::make_shared;
    using std::make_unique;

    template <typename T> class ResourceManager {
        private:
            typedef multimap<string, shared_ptr<T> > ResourceContainer;
            ResourceContainer resources;

        public:
            ResourceManager(){ defaultResource = nullptr; };
            virtual ~ResourceManager(){ unloadAll(); }

            //Will load the resource if not already loaded
            shared_ptr<T> getResource( string filename )
            {
                typename ResourceContainer::iterator iter = resources.find(filename);
                if (iter != resources.end())
                    return iter->second;

                shared_ptr<T> ret = loadResource(filename);
                if (ret) return ret;
                return defaultResource;
            }

            void unloadAll()
            {
                // This will clear all shared_ptr objects
                // automatically deleting everything if needed
                resources.clear();
            }

            bool resourceLoaded( string name ){
                typename ResourceContainer::iterator iter = resources.find(name);
                return (iter != resources.end());
            }

        protected:
            //Must be implemented by subclass and must use addResource to add the resource
            virtual shared_ptr<T> loadResource( string filename )=0;

            shared_ptr<T> defaultResource;

            void addResource( string name, shared_ptr<T> res ){
                //resources.insert( ResourceContainer::value_type( name, res ) );
                resources.insert( make_pair( name, res ) );
            }
    };
}
