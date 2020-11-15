//
// Created by PaperL on 2020/11/9.
//

#include "blocklist.h"

Node::Node() {
    offset = -1;
    isdel = 0;
    memset(str, 0, sizeof(str));
}

Node::Node(int arg1, string arg2) {
    offset = arg1;
    isdel = 0;
    memset(str, 0, sizeof(str));
    strcpy(str, arg2.c_str());
}

bool Node::operator<(const Node &x) const {
    string s1 = str, s2 = x.str;
    return s1 < s2;
}

Block::Block() {
    nxt = -1;
    pre = -1;
    num = 0;
}

inline int blocklist::nextBlock(int offset) {//获取下一个块的文件内绝对地址
    static int temp;
    fip.open(fname, ios::in | ios::binary);
    fip.seekg(offset, ios::beg);
    fip.read(reinterpret_cast<char *>(&temp), 4);
    fip.close();
    return temp;
}


blocklist::blocklist(string arg):fname(arg) {//构造函数
    //const string fname = arg; //todo 现在的常量初始化大概是正确的吧
    fi.open(fname, ios::in | ios::binary);
    if (!fi.is_open()) {
        fi.close();
        cout << "Discover index file \"" << fname << "\" is missing." << endl;
        cout << "Creating blank file \"" << fname << "\" ..." << endl;
        fi.open(fname, ios::out | ios::trunc | ios::binary);
    }
    fi.close();
}

void blocklist::delBlock(int offset) {//删除块
    int nxt, pre;
    fi.open(fname, ios::in | ios::binary);
    fo.open(fname, ios::in | ios::out | ios::binary);

    fi.seekg(offset, ios::beg);//读入待删除块的nxt和pre
    fi.read(reinterpret_cast<char *>(&nxt), 4);
    fi.read(reinterpret_cast<char *>(&pre), 4);

    fo.seekp(pre, ios::beg);//类似链表删除节点
    fo.write(reinterpret_cast<char *>(&nxt), 4);
    fo.seekp(nxt + 4, ios::beg);
    fo.write(reinterpret_cast<char *>(&pre), 4);

    fi.close();
    fo.close();
}

void blocklist::mergeBlock(int offset1, int offset2) {//合并相邻块
    //todo 这边可能可以优化，用数组直接读入两个块的头数据
    fi.open(fname, ios::in | ios::binary);
    fi2.open(fname, ios::in | ios::binary);
    fo.open(fname, ios::in | ios::out | ios::binary);

    Block tempBlock1, tempBlock2;//读入待合并块
    fi.seekg(offset1, ios::beg);
    fi.read(reinterpret_cast<char *>(&tempBlock1), sizeof(Block));
    fi2.seekg(offset2, ios::beg);
    fi2.read(reinterpret_cast<char *>(&tempBlock2), sizeof(Block));

    //合并块
    for (int i = 0; i < tempBlock2.num; ++i)
        tempBlock1.array[tempBlock1.num + i] = tempBlock2.array[i];
    tempBlock1.num += tempBlock2.num;
    tempBlock1.nxt = tempBlock2.nxt;

    //写入offset1
    fo.seekp(offset1, ios::beg);
    fo.write(reinterpret_cast<char *>(&tempBlock1), sizeof(Block));

    //将offset2下一个块的pre指向offset1
    if (tempBlock1.nxt != -1) {
        fo.seekp(tempBlock1.nxt + 4, ios::beg);
        fo.write(reinterpret_cast<char *>(&offset1), 4);
    }

    fi.close();
    fi2.close();
    fo.close();
}

void blocklist::splitBlock(int offset, int leftNum) {//leftNum为offset块保留Node个数
    fi.open(fname, ios::in | ios::binary);
    fo.open(fname, ios::in | ios::out | ios::binary);
    fi2.open(fname, ios::in | ios::binary);
    fo2.open(fname, ios::in | ios::out | ios::binary);

    //读入offset块
    Block tempBlock;
    fi.seekg(offset, ios::beg);
    fi.read(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

    //设定新块
    //todo 此处会产生未删除的无用数据
    for (int i = 0; i < tempBlock.num - leftNum; ++i)
        tempBlock.array[i] = tempBlock.array[i + leftNum];
    tempBlock.num -= leftNum;
    tempBlock.pre = offset;
    fo.seekp(0, ios::end);

    int temp = fo.tellp();//修改旧块的nxt和num
    fo2.seekp(offset, ios::beg);//todo 此处指针写入文件可能有误
    fo2.write(reinterpret_cast<char *>(&temp), 4);
    fo2.seekp(4, ios::cur);
    fo2.write(reinterpret_cast<char *>(&leftNum), 4);

    fo.write(reinterpret_cast<char *>(&tempBlock), sizeof(Block));//新块写入文件末

    fi.close(), fo.close();
    fi2.close(), fo2.close();
}

void blocklist::findNode(string key, vector<Node> &array) {
    fi.open(fname, ios::in | ios::binary);
    fi2.open(fname, ios::in | ios::binary);

    array.clear();
    fi2.seekg(0, ios::end);//文件末指针
    fi.seekg(20, ios::beg);//第一个块的array[0].str起始位置
    if (fi.tellg() < fi2.tellg()) {//遍历查找node所在块
        string s;
        char temps[64];
        fi.read(reinterpret_cast<char *>(temps), 64);//读入第一个块第一个Node.str
        s = temps;

        int lastp = -1;//lastp为node应在块的位置
        int nodep = 0;//nodep为正在遍历的块的位置
        while (s <= key) {
            lastp = nodep;
            nodep = nextBlock(nodep);
            if (nodep == -1)break;
            fi.seekg(nodep + 20, ios::beg);//读入nodep块的第一个Node.str
            fi.read(reinterpret_cast<char *>(temps), 64);
            s = temps;
        }
        if (lastp != -1) {
            fi.seekg(lastp, ios::beg);//指向node可能所在块头

            Node tempNode;//开一个空Node存入key，用于lower_bound
            strcpy(tempNode.str, key.c_str());

            Block tempBlock;//读入整个块
            fi.seekg(lastp, ios::beg);
            fi.read(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

            int pos;//用二分lower_bound找到第一个Node[i].str大于等于key的i
            pos = lower_bound(tempBlock.array, tempBlock.array + tempBlock.num, tempNode) - tempBlock.array;

            //将符合要求的Node加入array
            while (tempBlock.array[pos].str == key)
                array.push_back(tempBlock.array[pos++]);
        }
    }
    fi.close();
    fi2.close();
}

void blocklist::addNode(Node node) {//找东西优先二分
    //尽量不要把任何class读写拆开
    fi.open(fname, ios::in | ios::binary);
    fo.open(fname, ios::in | ios::out | ios::binary);
    fi2.open(fname, ios::in | ios::binary);

    fi2.seekg(0, ios::end);//文件末指针
    fi.seekg(20, ios::beg);//第一个块的array[0].str起始位置

    if (fi.tellg() > fi2.tellg()) {//*第一次*添加Node,新建块与Node
        Block tempBlock;
        tempBlock.num = 1;
        tempBlock.array[0] = node;
        fo.seekp(0, ios::beg);
        fo.write(reinterpret_cast<char *>(&tempBlock), sizeof(Block));//将node写入array[0]处
    } else {//在合适的块加入node
        int lastp = 0;//lastp为node应插入的块的位置
        int nodep = 0;//nodep为正在遍历的块的位置
        string s, s2;
        char temps[64];

        fi.read(temps, 64);
        s = temps;
        s2 = node.str;
        while (s <= s2) {
            lastp = nodep;
            nodep = nextBlock(nodep);
            if (nodep == -1)break;
            fi.seekg(nodep + 20, ios::beg);
            fi.read(temps, 64);
            s = temps;
        }
        fi.seekg(lastp, ios::beg);//指向块头

        Block tempBlock;//读入整个块
        fi.seekg(lastp, ios::beg);
        fi.read(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

        int pos;//用二分lower_bound找到第一个Node[i].str大于等于key的i
        pos = lower_bound(tempBlock.array, tempBlock.array + tempBlock.num, node) - tempBlock.array;

        //插入node
        for (int i = tempBlock.num - 1; i >= pos; --i)
            tempBlock.array[i + 1] = tempBlock.array[i];
        tempBlock.array[pos] = node;
        ++tempBlock.num;

        //将插入完成的块写入文件
        fo.seekp(lastp, ios::beg);
        fo.write(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

        if (tempBlock.num > 315)//如果块内元素数量大于315，则分裂块
            splitBlock(lastp, 160);
    }

    fi.close(), fo.close();
    fi2.close();
}

int blocklist::deleteNode(string key) {
    //主要代码来自 int blocklist::addNode
    fi.open(fname, ios::in | ios::binary);
    fi2.open(fname, ios::in | ios::binary);

    fi2.seekg(0, ios::end);//文件末指针
    fi.seekg(20, ios::beg);//第一个块的array[0].str起始位置
    if (fi.tellg() < fi2.tellg()) {//遍历查找node所在块
        string s;
        char temps[64];
        fi.read(reinterpret_cast<char *>(temps), 64);//读入第一个块第一个Node.str
        s = temps;

        int lastp = -1;//lastp为node应在块的位置
        int nodep = 0;//nodep为正在遍历的块的位置
        while (s <= key) {
            lastp = nodep;
            nodep = nextBlock(nodep);
            if (nodep == -1)break;
            fi.seekg(nodep + 20, ios::beg);//读入nodep块的第一个Node.str
            fi.read(reinterpret_cast<char *>(temps), 64);
            s = temps;
        }
        if (lastp != -1) {
            fi.seekg(lastp, ios::beg);//指向node可能所在块头

            Node tempNode;//开一个空Node存入key，用于lower_bound
            strcpy(tempNode.str, key.c_str());

            Block tempBlock;//读入整个块
            fi.seekg(lastp, ios::beg);
            fi.read(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

            int pos;//用二分lower_bound找到第一个Node[i].str大于等于key的i
            pos = lower_bound(tempBlock.array, tempBlock.array + tempBlock.num, tempNode) - tempBlock.array;

            //如果找到符合要求的Node，删除，并写回文件
            while (tempBlock.array[pos].str == key) {
                if (tempBlock.array[pos].isdel == 0) {
                    int temp = 1;
                    fo.open(fname, ios::in | ios::out | ios::binary);
                    fo.seekp(16 + pos * sizeof(Node), ios::beg);//todo 这里很可能会有bug
                    fo.write(reinterpret_cast<char *>(&temp), 4);
                    fo.close();
                    fi.close();
                    fi2.close();
                    return 0;//操作成功
                }
                ++pos;
            }
        }
    }
    fi.close();
    fi2.close();
    return -1;//操作失败
}