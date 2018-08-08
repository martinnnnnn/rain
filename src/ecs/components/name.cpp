#include "name.h"



namespace rain
{
    CName* CreateName(const char* _name)
    {
        CName* nameComponent = (CName*)malloc(sizeof(CName));
        nameComponent->componentType = ComponentType::NAME;
        nameComponent->name = (char*)malloc((sizeof(_name) + 1));
        strcpy_s(nameComponent->name, sizeof(nameComponent->name) + 1, _name);

        return nameComponent;
    }

    void InitName(CName* _nameComponent, const char* _name)
    {
        _nameComponent->componentType = ComponentType::NAME;
        _nameComponent->name = (char*)malloc((sizeof(_name) + 1));
        strcpy_s(_nameComponent->name, sizeof(_nameComponent->name) + 1, _name);
    }
}