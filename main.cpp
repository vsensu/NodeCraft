#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

#include <thread>
#include <iostream>
#include <chrono>

#include "entt.hpp"
#include "RenderWidget.h"

#include "components.h"

double g_update_freq = 16.666;
int g_init_w = 600;
int g_init_h = 400;

void update_pos(std::uint64_t dt, entt::registry &registry)
{
    registry.view<position, velocity>().each([dt](auto &pos, auto &vel)
    {
        // gets all the components of the view at once ...

        pos.x += vel.dx * dt;
        pos.y += vel.dy * dt;

        // ...
    });
}

void update_collision(entt::registry &registry)
{
    Screen screen;
    registry.view<Screen>().each([&screen](auto &sc){
        screen = sc;
    });
    registry.view<position, velocity, RectNode>().each([&screen](auto &pos, auto &vel, auto &node)
    {
        if(pos.x >= screen.w - node.w || pos.x <= 0) vel.dx = -vel.dx;
        if(pos.y <= 0 || pos.y >= screen.h - node.h) vel.dy = -vel.dy;

        if(pos.x < 0) pos.x = 0;
        if(pos.x > screen.w - node.w) pos.x = screen.w - node.w;
        if(pos.y < 0) pos.y = 0;
        if(pos.y > screen.h - node.h) pos.y = screen.h - node.h;
    });
}

void update_fps(std::uint64_t fps, entt::registry &registry)
{
    registry.view<FPS>().each([fps](auto &c) {
        c.value = fps;
    });
}

void hello()
{
   std::cout << "hello world\n";
}

bool g_update_ecs = true;
entt::registry registry;

void update_ecs()
{
    // 初始创建一些entity
    for(auto i = 0; i < 10; ++i) {
        auto entity = registry.create();
        registry.emplace<position>(entity, i * 1.f, i * 1.f);
        registry.emplace<velocity>(entity, i * 0.1f, i * 0.1f);
        registry.emplace<RectNode>(entity, 100, 100, rand()%256, rand()%256, rand()%256);
    }

    // fps singleton component
    auto fps_entity = registry.create();
    registry.emplace<FPS>(fps_entity, 0u);

    // screen size singleton component
    auto screen_entity = registry.create();
    registry.emplace<Screen>(screen_entity, g_init_w, g_init_h);

    // ecs循环
    auto previous = std::chrono::steady_clock::now();
    double lag = 0;
    while(g_update_ecs)
    {
        auto current = std::chrono::steady_clock::now();
        auto elapsed_seconds = std::chrono::duration<double>(current-previous).count();
        auto elapsed_ms = elapsed_seconds*1000;
        previous = current;
        lag += elapsed_ms;

        while(lag >= g_update_freq)
        {
            std::cout << "elapsed time: " << lag << "ms\n";
            update_pos(g_update_freq, registry);
            update_collision(registry);
            update_fps(1000.0/lag, registry);
            lag -= g_update_freq;
        }
    }
}

// 运行时报错是因为动态库环境还没有进行配置， Run -> Edit Configuration -> Environment variables -> path=D:\Qt\QtXXX\XXX\msvc2017\bin
// 如果是64位 path=D:\Qt\QtXXX\XXX\msvc2017_64\bin
// 同时CMakeLists.txt 改为 set(CMAKE_PREFIX_PATH D:/Qt/QtXXX/XXX/msvc2017_64/lib/cmake)
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    RenderWidget w;
    w.SetRegistry(registry);
    w.resize(g_init_w, g_init_h);
    w.show();

    std::thread t(update_ecs);

    app.exec();

    g_update_ecs = false;
    t.join();
    return 0;
}
