#include "DrawFunctions.h"
#include "DxLib.h"
#include <cassert>

namespace Graph {

    //�O���t��ǂݍ��ފ֐�
    int Graph::LoadGraph(const char* path)
    {
        int handle = DxLib::LoadGraph(path);
        assert(handle != -1);

        return handle;
    }

    //�ǂݍ��񂾃O���t�������̒l�ɂ���ĕ����`�悷��
    int Graph::DrawRectRotaGraph(float x, float y, int left, int top, int width, int height, float scale, float angle, int handle, bool transFlag, bool tumFlag)
    {
        return DrawRectRotaGraphF(x, y, left, top, width, height, scale, angle, handle, transFlag, tumFlag);
    }

}

