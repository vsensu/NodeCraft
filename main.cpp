#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

// 运行时报错是因为动态库环境还没有进行配置， Run -> Edit Configuration -> Environment variables -> path=D:\Qt\QtXXX\XXX\msvc2017\bin
// 如果是64位 path=D:\Qt\QtXXX\XXX\msvc2017_64\bin
// 同时CMakeLists.txt 改为 set(CMAKE_PREFIX_PATH D:/Qt/QtXXX/XXX/msvc2017_64/lib/cmake)
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QWidget w;
    w.show();
    return app.exec();
}
