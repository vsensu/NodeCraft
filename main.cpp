#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

#include <thread>
#include <iostream>
#include <chrono>

#include "entt.hpp"
#include "RenderWidget.h"

#include "components.h"

double g_update_freq = 16.666;

void update_pos(std::uint64_t dt, entt::registry &registry) {
    registry.view<position, velocity>().each([dt](auto &pos, auto &vel) {
        // gets all the components of the view at once ...

        pos.x += vel.dx * dt;
        pos.y += vel.dy * dt;

        // ...
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

    auto entity = registry.create();
    registry.emplace<FPS>(entity, 0u);

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
    w.show();

    std::thread t(update_ecs);

    app.exec();

    g_update_ecs = false;
    t.join();
    return 0;
}
