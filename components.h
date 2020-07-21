//
// Created by vsensu on 2020/7/19.
//

#ifndef NODECRAFT_COMPONENTS_H
#define NODECRAFT_COMPONENTS_H

struct position {
    float x;
    float y;
};

struct velocity {
    float dx;
    float dy;
};

struct RectNode {
    int w, h;
    int r, g, b;
};

struct FPS {
    std::uint64_t value;
};

struct Screen {
    int w, h;
};

#endif //NODECRAFT_COMPONENTS_H
