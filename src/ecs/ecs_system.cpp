//
//
//#include <vector>
//#include <iostream>
//#include <stdint.h>
//
//#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
//
//typedef int8_t i8;
//typedef int16_t i16;
//typedef int32_t i32;
//typedef int64_t i64;
//
//typedef uint8_t u8;
//typedef uint16_t u16;
//typedef uint32_t u32;
//typedef uint64_t u64;
//
//void PrintBit(size_t const size, void const * const ptr)
//{
//    unsigned char *b = (unsigned char*)ptr;
//    unsigned char byte;
//    int i, j;
//
//    for (i = size - 1; i >= 0; i--)
//    {
//        for (j = 7; j >= 0; j--)
//        {
//            byte = (b[i] >> j) & 1;
//            printf("%u", byte);
//        }
//    }
//    puts("");
//}
//
//
////------------------------------------------
//struct vec3
//{
//    float x;
//    float y;
//    float z;
//};
//
//struct GLSLVariable
//{
//    unsigned int id;
//    char* name;
//    u32 size;
//};
//
////------------------------------------------
//
//struct Component
//{
//    // entity uses a bitmask to register which component it contains
//    // this enum doesnt represent bitflags
//    // it is used to shift bits the right number of times to make a bitmask -- i think that should work
//    enum class Type : u32
//    {
//        TRANSFORM = 0,
//        MOVEMENT,
//        SCRIPT,
//        PHYSICS,
//        MODEL,
//        MAX_COUNT
//    };
//
//    Component::Type type;
//};
//
//struct Transform : public Component
//{
//    vec3 position;
//    vec3 rotation;
//    vec3 scale;
//};
//
//struct Movement : public Component
//{
//    vec3 direction;
//    float speed;
//};
//
//struct Script : Component
//{
//    void(*Init)(void);
//    void(*Update)(void);
//    void(*Shutdown)(void);
//};
//
//struct Physics : Component
//{
//    float weight;
//    float friction;
//    bool grounded;
//};
//
//struct Model : Component
//{
//    vec3* vertices;
//    vec3* indices;
//    u32 size;
//};
//
//Component* CreateTransform()
//{
//    Transform* transform = (Transform*)malloc(sizeof(Transform));
//    transform->type = Component::Type::TRANSFORM;
//    transform->position = { 0, 0, 0 };
//    transform->rotation = { 0, 0, 0 };
//    transform->scale = { 1, 1, 1 };
//    return transform;
//}
//
//Component* CreateMovement()
//{
//    Movement* movement = (Movement*)malloc(sizeof(Movement));
//    movement->type = Component::Type::MOVEMENT;
//    movement->direction = { 0, 0, 0 };
//    movement->speed = 0;
//    return movement;
//}
//
//Component* CreateScript(void(*Init)(void), void(*Update)(void), void(*Shutdown)(void))
//{
//    Script* script = (Script*)malloc(sizeof(Script));
//    script->type = Component::Type::SCRIPT;
//    script->Init = Init;
//    script->Update = Update;
//    script->Shutdown = Shutdown;
//    return script;
//}
//
//Component* CreatePhysics()
//{
//    Physics* physics = (Physics*)malloc(sizeof(Physics));
//    physics->type = Component::Type::PHYSICS;
//    physics->friction = 0;
//    physics->grounded = false;
//    physics->weight = 0;
//    return physics;
//}
//
//Component* CreateModel()
//{
//    Model* model = (Model*)malloc(sizeof(Model));
//    model->type = Component::Type::MODEL;
//    model->indices = nullptr;
//    model->vertices = nullptr;
//    model->size = 0;
//    return model;
//}
//
//
////------------------------------------
//
//// TODO(martin): add stepcount
//struct Entity
//{
//    u64 id;
//    char* name;
//    u64 flags;
//    Component** components;
//    u32 size;
//    u32 capacity;
//};
//
//Entity* CreateEntity(u64 _id, const char* _name, u32 _capacity = 0)
//{
//    Entity* entity = (Entity*)calloc(1, sizeof(Entity));
//    entity->id = _id;
//
//    entity->name = (char*)malloc((sizeof(_name) + 1));
//    strcpy_s(entity->name, sizeof(entity->name) + 1, _name);
//    entity->capacity = _capacity;
//    if (entity->capacity > 0)
//    {
//        entity->components = (Component**)calloc(entity->capacity, sizeof(Component*));
//    }
//
//    return entity;
//}
//
//void AddComponent(Entity* _entity, Component* _component)
//{
//    u64 flag = (1 << (u64)_component->type);
//    if ((_entity->flags & flag) == 0)
//    {
//        _entity->flags |= flag;
//
//        if (_entity->size == _entity->capacity)
//        {
//            bool wasEmpty = _entity->capacity == 0;
//            _entity->capacity += 6;
//            if (wasEmpty)
//            {
//                _entity->components = (Component**)calloc(_entity->capacity, sizeof(Component*));
//            }
//            else
//            {
//                realloc(_entity->components, _entity->capacity);
//            }
//        }
//
//        _entity->components[_entity->size] = _component;
//        _entity->size++;
//    }
//}
//
//void RemoveComponent(Entity* _entity, Component::Type _type)
//{
//    u64 flag = (1 << (u64)_type);
//    if ((_entity->flags & flag) != 0)
//    {
//        Component** end = _entity->components + _entity->size;
//        Component** iter = _entity->components;
//
//        while (iter < end)
//        {
//            if ((*iter)->type == _type)
//            {
//                free(*iter);
//                //iter = &(_entity->components[_entity->size - 1]);
//                iter = _entity->components + (_entity->size - 1);
//                _entity->flags &= ~(1 << flag);
//                _entity->size--;
//                return;
//            }
//            iter++;
//        }
//
//    }
//}
//
//Component* FindComponent(Entity* _entity, Component::Type _type)
//{
//    u64 flag = (1 << (u32)_type);
//    if ((_entity->flags & flag) != 0)
//    {
//        Component** end = _entity->components + _entity->size;
//        Component** iter = _entity->components;
//
//        while (iter < end)
//        {
//            if ((*iter)->type == _type)
//            {
//                return *iter;
//            }
//            iter++;
//        }
//    }
//
//    return nullptr;
//}
//
//bool FitsRequirements(Entity* _entity, u64 _bitmask)
//{
//    if ((_entity->flags & _bitmask) == _bitmask)
//    {
//        return true;
//    }
//    return false;
//}
//
//
////----------------------------------------------------
//
//struct EntityContainer
//{
//    Entity** entities;
//    u32 size;
//    u32 capacity;
//    u32 resizeStep;
//};
//
//EntityContainer* CreateEntityContainer(u32 _capacity, u32 _resizeStep = 0)
//{
//    EntityContainer* container = (EntityContainer*)malloc(sizeof(EntityContainer));
//    container->size = 0;
//    container->resizeStep = _resizeStep;
//    container->capacity = _capacity;
//
//    if (container->capacity > 0)
//    {
//        container->entities = (Entity**)calloc(container->capacity, sizeof(Entity*));
//    }
//    return container;
//}
//
//void AddEntity(EntityContainer* _container, Entity* _entity)
//{
//    if (_container->size == _container->capacity)
//    {
//        bool wasEmpty = (_container->capacity == 0);
//        _container->capacity += _container->resizeStep;
//        if (wasEmpty)
//        {
//            _container->entities = (Entity**)calloc(_container->capacity, sizeof(Entity*));
//        }
//        else
//        {
//            realloc(_container->entities, _container->capacity);
//        }
//    }
//
//    _container->entities[_container->size] = _entity;
//    _container->size++;
//}
//
//void RemoveEntity(EntityContainer* _container, Entity* _ent)
//{
//    Entity** end = _container->entities + _container->size;
//    Entity** iter = _container->entities;
//
//    while (iter < end)
//    {
//        if ((*iter) == _ent)
//        {
//            free(*iter);
//            iter = _container->entities + (_container->size - 1);
//            _container->size--;
//            return;
//        }
//        iter++;
//    }
//}
//
//
//
//void EntityTests();
//
//int main(void)
//{
//    // CONTAINER CREATION
//    EntityContainer* container = CreateEntityContainer(10);
//    // ADDING ENTITIES
//    std::vector<Entity*> tempvec;
//
//    for (u32 i = 0; i < 20; i++)
//    {
//        Entity* ent = CreateEntity(i, "ent");
//        tempvec.push_back(ent);
//        AddEntity(container, ent);
//    }
//
//    std::cout << std::endl << "after additions : " << container->size << "," << container->capacity << std::endl;
//
//    for (u32 i = 0; i < 18;)
//    {
//        RemoveEntity(container, tempvec[i]);
//        i += 2;
//    }
//    std::cout << std::endl << "after additions : " << container->size << "," << container->capacity << std::endl;
//
//    // REMOVING ENTITIES
//
//
//
//    //EntityTests();
//
//    system("PAUSE");
//    return 0;
//}
//
//
//
//
//
//
//
//struct LinearMemoryAllocator
//{
//    void* memory;
//    u32 size;
//};
//
//struct StackMemoryAllocator
//{
//
//};
//
//struct PoolMemoryAllocator
//{
//
//};
//
//void EntityTests()
//{
//    // ENTITY TEST CREATION
//    Entity* ent1 = CreateEntity(1, "ent1", 0);
//    Entity* ent2 = CreateEntity(2, "ent2", 1);
//
//    // ENTITY TEST ADD COMPONENT
//    Component* comp1 = (Transform*)calloc(1, sizeof(Transform));
//    comp1->type = Component::Type::TRANSFORM;
//    Transform* comp2 = (Transform*)malloc(sizeof(Transform));
//    comp2->type = Component::Type::TRANSFORM;
//    comp2->scale = { 1, 1, 1 };
//    comp2->position = { 0, 0, 0 };
//    comp2->rotation = { 0, 90, 0 };
//    AddComponent(ent1, comp1);
//    AddComponent(ent2, comp2);
//
//    std::cout << "Entity creation : "
//        << std::endl << "-> id : " << ent1->id
//        << std::endl << "-> name : " << ent1->name
//        << std::endl << "-> flags : " << std::endl;
//    PrintBit(sizeof(ent1->flags), &ent1->flags);
//    std::cout << "-> components ? : " << (ent1->components != nullptr)
//        << std::endl << "-> size : " << ent1->size
//        << std::endl << "-> capacity : " << ent1->capacity
//        << std::endl << std::endl;
//
//    std::cout << "Entity creation : "
//        << std::endl << "-> id : " << ent2->id
//        << std::endl << "-> name : " << ent2->name
//        << std::endl << "-> flags : " << std::endl;
//    PrintBit(sizeof(ent2->flags), &ent2->flags);
//    std::cout << "-> components ? : " << (ent2->components != nullptr)
//        << std::endl << "-> size : " << ent2->size
//        << std::endl << "-> capacity : " << ent2->capacity
//        << std::endl << std::endl;
//
//
//
//
//    // ENTITY TEST FIND COMPONENT
//    Transform* tran1 = (Transform*)FindComponent(ent1, Component::Type::TRANSFORM);
//    if (tran1)
//    {
//        std::cout << std::endl << "found transform in ent1 : "
//            << std::endl << "(" << tran1->position.x << "," << tran1->position.y << "," << tran1->position.z << ")"
//            << std::endl << "(" << tran1->scale.x << "," << tran1->scale.y << "," << tran1->scale.z << ")"
//            << std::endl << "(" << tran1->rotation.x << "," << tran1->rotation.y << "," << tran1->rotation.z << ")";
//    }
//    else
//    {
//        std::cout << std::endl << "ERROR :: FINDCOMPONENT transform in ent1 failed" << std::endl;
//    }
//
//    Transform* tran2 = (Transform*)FindComponent(ent2, Component::Type::TRANSFORM);
//    if (tran2)
//    {
//        std::cout << std::endl << "found transform in ent2 : "
//            << std::endl << "(" << tran2->position.x << "," << tran2->position.y << "," << tran2->position.z << ")"
//            << std::endl << "(" << tran2->scale.x << "," << tran2->scale.y << "," << tran2->scale.z << ")"
//            << std::endl << "(" << tran2->rotation.x << "," << tran2->rotation.y << "," << tran2->rotation.z << ")";
//    }
//    else
//    {
//        std::cout << std::endl << "ERROR :: FINDCOMPONENT transform in ent2 failed" << std::endl;
//    }
//
//    // ENTITY TEST REMOVE COMPONENT
//    Physics* comp3 = (Physics*)calloc(1, sizeof(Physics));
//    comp3->type = Component::Type::PHYSICS;
//    comp3->grounded = true;
//    comp3->weight = 127;
//    comp3->friction = 42;
//
//    AddComponent(ent1, comp3);
//    Physics* phy1 = (Physics*)FindComponent(ent1, Component::Type::PHYSICS);
//    if (phy1)
//    {
//        std::cout << std::endl << "found physics in ent1 : "
//            << std::endl << "friction : " << phy1->friction << ", grounded : " << (phy1->grounded ? "true" : "false") << ", weight :" << phy1->weight << std::endl;
//    }
//    else
//    {
//        std::cout << std::endl << "ERROR :: FINDCOMPONENT physics in ent1 failed" << std::endl;
//    }
//    RemoveComponent(ent1, Component::Type::PHYSICS);
//    Physics* phy2 = (Physics*)FindComponent(ent1, Component::Type::PHYSICS);
//    if (phy2)
//    {
//        std::cout << std::endl << "ERROR :: physics still in ent1" << std::endl;
//    }
//    else
//    {
//        std::cout << std::endl << "removed worked ! " << std::endl;
//    }
//
//    // ENTITY TEST FITS REQUIREMENTS
//    comp3 = (Physics*)calloc(1, sizeof(Physics));
//    comp3->type = Component::Type::PHYSICS;
//    comp3->grounded = true;
//    comp3->weight = 127;
//    comp3->friction = 42;
//    AddComponent(ent1, comp3);
//    u64 mask1 = (1 << (u64)Component::Type::TRANSFORM) | (1 << (u64)Component::Type::PHYSICS);
//    u64 mask2 = (1 << (u64)Component::Type::TRANSFORM) | (1 << (u64)Component::Type::MODEL);
//    u64 mask3 = (1 << (u64)Component::Type::TRANSFORM) | (1 << (u64)Component::Type::PHYSICS) | (1 << (u64)Component::Type::MODEL);
//    u64 mask4 = (1 << (u64)Component::Type::TRANSFORM);
//    u64 mask5 = 0;
//
//    bool fits1 = FitsRequirements(ent1, mask1);
//    bool fits2 = FitsRequirements(ent1, mask2);
//    bool fits3 = FitsRequirements(ent1, mask3);
//    bool fits4 = FitsRequirements(ent1, mask4);
//    bool fits5 = FitsRequirements(ent1, mask5);
//
//    std::cout << std::endl << "1 (true) : " << (fits1 ? "true" : "false")
//        << std::endl << "2 (false) : " << (fits2 ? "true" : "false")
//        << std::endl << "3 (false) : " << (fits3 ? "true" : "false")
//        << std::endl << "4 (true) : " << (fits4 ? "true" : "false")
//        << std::endl << "5 (true) : " << (fits5 ? "true" : "false") << std::endl;
//
//    std::cout << std::endl;
//}