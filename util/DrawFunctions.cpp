#include "DrawFunctions.h"
#include "DxLib.h"
#include <cassert>

namespace Graph {

    //グラフを読み込む関数
    int Graph::loadGraph(const char* path)
    {
        int handle = LoadGraph(path);
        assert(handle != -1);

        return handle;
    }

    //読み込んだグラフを引数の値によって分割描画する
    int Graph::drawRectRotaGraph(int x, int y, int left, int top, int width, int height, float scale, float angle, int handle, bool transFlag, bool tumFlag)
    {
        return DrawRectRotaGraph(x, y, left, top, width, height, scale, angle, handle, transFlag, tumFlag);
    }

}

