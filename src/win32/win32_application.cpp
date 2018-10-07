#include "win32_application.h"

#include <windows.h>

#include "win32_window.h"
#include "ogl/ogl_renderer.h"

#include "core/sparse_set.h"
#include "ecs/ecs_entity.h"

void set_test()
{
	using DefaultTBD = TBD<u32>;
    using etypetrait = EntityTrait<u32>;
    using etype = typename etypetrait::entity_type;
    using eversion = typename etypetrait::version_type;


    SparseSet<etype> set;
    bool empty1 = set.empty();

    set.construct(32);
    set.construct(62);
    set.construct(12);
    set.construct(42);

    bool empty2 = set.empty();

    bool c1 = set.contains(32);
    //bool c2 = set.contains(-1);
    bool c3 = set.contains(123);

    u32 size1 = set.get(62);


    //set.destroy(-1);
    set.destroy(42);
    //set.destroy(123);
    set.destroy(62);

}

int Application::init(HINSTANCE _hinstance, const std::string& _config)
{
    hinstance = _hinstance;

	using DefaultTBD = TBD<u32>;
	using etypetrait = EntityTrait<u32>;
	using etype = typename etypetrait::entity_type;
	using eversion = typename etypetrait::version_type;

	DefaultTBD tbd;

	etype e1 = tbd.create();
	etype e2 = tbd.create();
	etype e3 = tbd.create();
	etype e4 = tbd.create();

	tbd.assign<Transform>(e1);
	tbd.assign<Physics>(e1);
	tbd.assign<Transform>(e2, glm::vec3(1, 1, 1));
	tbd.assign<Name>(e3, "hello");

	tbd.destroy(e4);
	etype e5 = tbd.create();
	etype e6 = tbd.create();
	etype e7 = tbd.create();

	tbd.destroy(e1);
	tbd.destroy(e2);



	//using etypetrait = EntityTrait<u32>;
	//using etype = typename etypetrait::entity_type;
	//using eversion = typename etypetrait::version_type;


	//std::vector<SparseSet<etype>*> pools;

	//using fam = TypeId<struct Hey>;

	//if (fam::get<Transform>() >= pools.size())
	//{
	//	pools.resize(fam::get<Transform>() + 1);
	//}

	//pools[fam::get<Transform>()] = new Pool<etype, Transform>();

	//if (fam::get<Name>() >= pools.size())
	//{
	//	pools.resize(fam::get<Name>() + 1);
	//}

	//pools[fam::get<Name>()] = new Pool<etype, Name>();


    GETWINDOW.initialize(hinstance, 800, 600, 0);

    renderer = new Renderer();
    renderer->initialize();
    GETWINDOW.show();

    return 0;
}

void Application::update()
{
    
}

void Application::render()
{
    renderer->render();
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{
    Application* app = new Application();
    app->init(hinstance, "");

	MSG msg;
	bool quit = false;
	while (!quit)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				quit = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

        // *** game logic update *** // 
        app->update();
        // *** needs a better game loop too *** // 

		if (GETWINDOW.is_initialized())
		{
			app->render();
			GETWINDOW.present();
		}

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            GETWINDOW.shutdown();
        }
	}

	return msg.lParam;
}
