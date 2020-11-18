//
// Created by PaperL on 2020/11/9.
//

#include "blocklist.h"

Node::Node() {
#ifdef PaperL_Debug
    //cout << "In Constructor \"Node\":" << endl;
#endif
    offset = -1;
    memset(str, 0, sizeof(str));
}

Node::Node(const int &arg1, const string &arg2) {
#ifdef PaperL_Debug
    //cout << "In Constructor \"Node\" Plus:" << endl;
#endif
    offset = arg1;
    memset(str, 0, sizeof(str));
    strcpy(str, arg2.c_str());
}

Node &Node::operator=(const Node &right) {
    if (this == &right)
        return *this;
    offset = right.offset;
    strcpy(str, right.str);
    return *this;
}

bool Node::operator<(const Node &x) const {
    string s1 = str, s2 = x.str;
    return s1 < s2;
}

Block::Block() {
#ifdef PaperL_Debug
    cout << "In Constructor \"Block\":" << endl;
#endif
    nxt = -1;
    pre = -1;
    num = 0;
}

Block &Block::operator=(const Block &right) {
    if (this == &right)
        return *this;
    nxt = right.nxt;
    pre = right.pre;
    num = right.num;
    for (int i = 0; i < num; ++i)
        array[i] = right.array[i];
    for (int i = num; i < BLOCK_SIZE; ++i)
        array[i] = Node();
    return *this;
}

inline int blocklist::nextBlock(const int &offset) {//获取下一个块的文件内绝对地址
#ifdef PaperL_Debug
    cout << "In Function \"nextBlock\":" << endl;
#endif
    int temp;
    //fip.open(fname, ios::in | ios::binary);
    fip.seekg(offset, ios::beg);
    fip.read(reinterpret_cast<char *>(&temp), 4);
    //fip.close();
#ifdef PaperL_Debug
    cout << "nxtPtr = " << temp << endl;
#endif
    return temp;
}

inline void blocklist::delBlock(const int &offset) {//删除块
#ifdef PaperL_Debug
    cout << "In Function \"delBlock\":" << endl;
#endif
    int nxt, pre;
    //fip.open(fname, ios::in | ios::binary);
    //fop.open(fname, ios::in | ios::out | ios::binary);

    fip.seekg(offset, ios::beg);//读入待删除块的nxt和pre
    fip.read(reinterpret_cast<char *>(&nxt), 4);
    fip.read(reinterpret_cast<char *>(&pre), 4);

    fop.seekp(pre, ios::beg);//类似链表删除节点
    fop.write(reinterpret_cast<char *>(&nxt), 4);
    fop.seekp(nxt + 4, ios::beg);
    fop.write(reinterpret_cast<char *>(&pre), 4);

    //fip.close();
    //fop.close();
}

void blocklist::mergeBlock(const int &offset1, const int &offset2) {//合并相邻块
    //todo 这边可能可以优化，用数组直接读入两个块的头数据
#ifdef PaperL_Debug
    cout << "In Function \"mergeBlock\":" << endl;
#endif
    //fi.open(fname, ios::in | ios::binary);
    //fi2.open(fname, ios::in | ios::binary);
    //fo.open(fname, ios::in | ios::out | ios::binary);

    Block tempBlock1, tempBlock2;//读入待合并块
    fip.seekg(offset1, ios::beg);
    fip.read(reinterpret_cast<char *>(&tempBlock1), sizeof(Block));
    fip2.seekg(offset2, ios::beg);
    fip2.read(reinterpret_cast<char *>(&tempBlock2), sizeof(Block));

    //合并块
    for (int i = 0; i < tempBlock2.num; ++i)
        tempBlock1.array[tempBlock1.num + i] = tempBlock2.array[i];
    tempBlock1.num += tempBlock2.num;
    //tempBlock1.nxt = tempBlock2.nxt; 链表指针操作在delBlock中完成

    //写入offset1
    fop.seekp(offset1, ios::beg);
    fop.write(reinterpret_cast<char *>(&tempBlock1), sizeof(Block));

    //删除offset2
    delBlock(offset2);

    /* 同上，链表指针操作在delBlock中完成
    //将offset2下一个块的pre指向offset1
    if (tempBlock1.nxt != -1) {
        fo.seekp(tempBlock1.nxt + 4, ios::beg);
        fo.write(reinterpret_cast<char *>(&offset1), 4);
    }*/

    //fi.close();
    //fi2.close();
    //fo.close();
}

void blocklist::splitBlock(const int &offset) {//leftNum为offset块保留Node个数
#ifdef PaperL_Debug
    cout << "In Function \"splitBlock\":" << endl;
#endif
    //fi.open(fname, ios::in | ios::binary);
    //fo.open(fname, ios::in | ios::out | ios::binary);
    //fi2.open(fname, ios::in | ios::binary);
    //fo2.open(fname, ios::in | ios::out | ios::binary);

    //读入offset块
    Block tempBlock;
    fip.seekg(offset, ios::beg);
    fip.read(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

    //设定新块
    //todo 此处会产生未删除的无用数据
    for (int i = 0; i < tempBlock.num - BLOCK_SPLIT_LEFT; ++i)
        tempBlock.array[i] = tempBlock.array[i + BLOCK_SPLIT_LEFT];
    for (int i = tempBlock.num - BLOCK_SPLIT_LEFT; i < tempBlock.num; ++i)
        tempBlock.array[i] = Node();//清空剩下Node
    tempBlock.num -= BLOCK_SPLIT_LEFT;
    tempBlock.pre = offset;

    fop.seekp(0, ios::end);
    int temp1 = fop.tellp(), temp2;
/*#ifdef PaperL_Debug
    //cout << "ios::end = " << temp1 << endl;
#endif*/
    //修改旧块的 nxt 和 num
    fop2.seekp(offset, ios::beg);
    fop2.write(reinterpret_cast<char *>(&temp1), 4);
    temp2 = BLOCK_SPLIT_LEFT;
    fop2.seekp(4, ios::cur);//跳过 pre
    fop2.write(reinterpret_cast<char *>(&temp2), 4);

    //fop.close();
    //fop.open(fname,ios::app);
    //fop.seekp(0, ios::end);
/*#ifdef PaperL_Debug
    cout << "ios::end2 = " << fop.tellp() << endl;
#endif*/
    //fop.clear();
    fop.write(reinterpret_cast<char *>(&tempBlock), sizeof(Block));//新块写入文件末
/*#ifdef PaperL_Debug
    cout << "ios::end2 = " << fop.tellp() << endl;
#endif*/
    fop.close();
    //fi.close(), fo.close();
    //fi2.close(), fo2.close();
}

blocklist::blocklist(const string &arg) : fname(arg) {//构造函数
#ifdef PaperL_Debug
    cout << "In Constructor \"blocklist\" :" << endl;
#endif
    //const string fname = arg; //todo 现在的常量初始化大概是正确的吧
    fi.open(fname, ios::in | ios::binary);
    if (!fi.is_open()) {
        fi.close();
#ifdef PaperL_Debug
        cout << "Discover index file \"" << fname << "\" is missing." << endl;
        //cout << "Creating blank file \"" << fname << "\" ..." << endl;
#endif
        fi.open(fname, ios::out | ios::trunc | ios::binary);
    }
    fi.close();

    fi.open(fname, ios::in | ios::binary);
    fi2.open(fname, ios::in | ios::binary);
    fip.open(fname, ios::in | ios::binary);
    fip2.open(fname, ios::in | ios::binary);

    fo.open(fname, ios::in | ios::out | ios::binary);
    fo2.open(fname, ios::in | ios::out | ios::binary);
    fop.open(fname, ios::in | ios::out | ios::binary);
    fop2.open(fname, ios::in | ios::out | ios::binary);
}

blocklist::~blocklist() {
#ifdef PaperL_Debug
    cout << "In Destructor \"blocklist\" :" << endl;
#endif
    fi.close();
    fi2.close();
    fip.close();
    fip2.close();

    fo.close();
    fo2.close();
    fop.close();
    fop2.close();
}

void blocklist::findNode(const string &key, vector<int> &array) {
#ifdef PaperL_Debug
    cout << "In Function \"findNode\":" << endl;
#endif
    //fi.open(fname, ios::in | ios::binary);
    //fi2.open(fname, ios::in | ios::binary);

    array.clear();
    fi2.seekg(0, ios::end);//文件末指针
    fi.seekg(16, ios::beg);//第一个块的array[0].str起始位置
    if (fi.tellg() < fi2.tellg()) {//遍历查找node所在块
        string s;
        char temps[64];
        //读入第一个块第一个Node.str
        fi.read(temps, 64);//char数组理论上不用强制类型转换
        s = temps;

        if (key < s) {
#ifdef PaperL_Debug
            cout << "\"findNode\" finish : Less than minum" << endl;
#endif
            return;
        }

        int lastp = 0;//lastp为node应在块的位置//todo　此处逻辑修正
        int nodep = 0;//nodep为正在遍历的块的位置
        while (key >= s) {
            if (key > s)
                lastp = nodep;
            nodep = nextBlock(nodep);
            if (nodep == -1)break;
            fi.seekg(nodep + 16, ios::beg);//读入nodep块的第一个Node.str
            fi.read(temps, 64);
            s = temps;
        }
        //比第一个块首元素还要小则不存在
        Node tempNode;//开一个空Node存入key，用于lower_bound
        strcpy(tempNode.str, key.c_str());

        Block tempBlock;//读入整个块
        fi.seekg(lastp, ios::beg);//指向node可能所在块头
        fi.read(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

/*#ifdef PaperL_Debug
            cout << "debugPrint------addNode-----\\/" << endl;
            cout << fname << endl;
            cout << "blockOffset: "<<lastp << endl;
            cout << "nxt: " << tempBlock.nxt << endl;
            cout << "pre: " << tempBlock.pre << endl;
            cout << "num: " << tempBlock.num << endl;
            for (int ii = 0; ii < BLOCK_SIZE; ++ii) {
                cout << ii << ": offset = " << tempBlock.array[ii].offset
                     << "\tstr = \"" << tempBlock.array[ii].str << "\"" << endl;
            }
            cout << "debugPrint------------------/\\" << endl;
#endif*/

        int pos;//用二分lower_bound找到第一个Node[i].str大于等于key的i
        pos = lower_bound(tempBlock.array, tempBlock.array + tempBlock.num, tempNode) - tempBlock.array;

        //将符合要求的Node加入array
        s = tempBlock.array[pos].str;
        while (pos < tempBlock.num && s == key) {
#ifdef PaperL_Debug
            cout << "Find Node: offset = " << tempBlock.array[pos].offset
                 << ", str = \"" << tempBlock.array[pos].str << "\"\n";
#endif
            array.push_back(tempBlock.array[pos++].offset);
            s = tempBlock.array[pos].str;
        }

        //如果搜索到块末，继续搜索下一个块
        while (pos == tempBlock.num) {
#ifdef PaperL_Debug
            cout << "    Search to End of Block" << endl;
#endif
            lastp = nextBlock(lastp);//读入下一个块
            if (lastp == -1) break;
            fi.seekg(lastp, ios::beg);
            fi.read(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

/*#ifdef PaperL_Debug
                cout << "debugPrint------addNode-----\\/" << endl;
                cout << fname << endl;
                cout << "blockOffset: 0" << endl;
                cout << "nxt: " << tempBlock.nxt << endl;
                cout << "pre: " << tempBlock.pre << endl;
                cout << "num: " << tempBlock.num << endl;
                for (int ii = 0; ii < BLOCK_SIZE; ++ii) {
                    cout << ii << ": offset = " << tempBlock.array[ii].offset
                         << "\tstr = \"" << tempBlock.array[ii].str << "\"" << endl;
                }
                cout << "debugPrint------------------/\\" << endl;
#endif*/

            pos = 0;
            s = tempBlock.array[0].str;
            while (pos < tempBlock.num && s == key) {
                array.push_back(tempBlock.array[pos++].offset);
                s = tempBlock.array[pos].str;
            }
        }

    }
    //fi.close();
    //fi2.close();
#ifdef PaperL_Debug
    cout << "\"findNode\" finish." << endl;
#endif
}

void blocklist::addNode(const Node &node) {
#ifdef PaperL_Debug
    cout << "In Function \"addNode\":" << endl;
    cout << "offset: " << node.offset << " ;\tstr: \"" << node.str << "\"" << endl;
#endif
    //fi.open(fname, ios::in | ios::binary);
    //fo.open(fname, ios::in | ios::out | ios::binary);
    //fi2.open(fname, ios::in | ios::binary);

    fi2.seekg(0, ios::end);//文件末指针
    fi.seekg(16, ios::beg);//第一个块的array[0].str起始位置

    if (fi.tellg() > fi2.tellg()) {//第一次添加Node,新建块与Node
        Block tempBlock;
        tempBlock.num = 1;
        tempBlock.array[0] = node;//array[0]为node
        fo.seekp(0, ios::beg);
        fo.write(reinterpret_cast<char *>(&tempBlock), sizeof(Block));
    } else {//在合适的块加入node
        int lastp = 0;//lastp为node应插入的块的位置
        int nodep = 0;//nodep为正在遍历的块的位置
        string s, keyString;
        char temps[64];

        fi.read(temps, 64);
        s = temps;
        keyString = node.str;
        while (s <= keyString) {
            lastp = nodep;
            nodep = nextBlock(nodep);
            if (nodep == -1)break;
            fi.seekg(nodep + 16, ios::beg);
            fi.read(temps, 64);
            s = temps;
        }
        //这里保证 lastp >= 0
        fi.seekg(lastp, ios::beg);//指向块头

        Block tempBlock;//读入整个块
        fi.seekg(lastp, ios::beg);
        fi.read(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

        int pos;//用二分lower_bound找到第一个Node[i].str大于等于key的i
        pos = lower_bound(tempBlock.array, tempBlock.array + tempBlock.num, node) - tempBlock.array;
        //找到最大 i 使得 Node[i].str == key, 则 pos = i + 1 减少时间复杂度
        /*s = tempBlock.array[pos].str;
        while (pos < tempBlock.num && s == keyString)
            s = tempBlock.array[++pos].str;*///todo 一个时间复杂度优化

        //插入node
        for (int i = tempBlock.num - 1; i >= pos; --i)
            tempBlock.array[i + 1] = tempBlock.array[i];
        tempBlock.array[pos] = node;
        ++tempBlock.num;

        //将插入完成的块写入文件
        fo.seekp(lastp, ios::beg);
        fo.write(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

        if (tempBlock.num > BLOCK_SPLIT_THRESHOLD)//如果块内元素数量大于315，则分裂块
            splitBlock(lastp);
    }
#ifdef PaperL_Debug
    cout << "\"addNode\" finish." << endl;
#endif
    //fi.close(), fo.close();
    //fi2.close();
}

int blocklist::deleteNode(const Node &node) {
#ifdef PaperL_Debug
    cout << "In Function \"deleteNode\":" << endl;
#endif
    //fi.open(fname, ios::in | ios::binary);
    //fi2.open(fname, ios::in | ios::binary);

    fi2.seekg(0, ios::end);//文件末指针
    fi.seekg(16, ios::beg);//第一个块的array[0].str起始位置
    if (fi.tellg() >= fi2.tellg()) return -1;//操作失败

    //遍历查找node所在块
    string s, keyString;
    char temps[64];
    fi.read(temps, 64);//读入第一个块第一个Node.str
    s = temps;
    keyString = node.str;

    if (keyString < s) return -1;//操作失败

    int lastp = 0;//lastp为node应在块的位置
    int nodep = 0;//nodep为正在遍历的块的位置
    while (keyString >= s) {
        if (keyString > s)
            lastp = nodep;
        nodep = nextBlock(nodep);
        if (nodep == -1)break;
        fi.seekg(nodep + 16, ios::beg);//读入nodep块的第一个Node.str
        fi.read(temps, 64);
        s = temps;
    }

#ifdef PaperL_Debug
    cout << "    deleteNode lastp = " << lastp << endl;
#endif
    Block tempBlock;//读入整个块
    fi.seekg(lastp, ios::beg);//指向node可能所在块
    fi.read(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

    int pos;//用二分lower_bound找到第一个Node[i].str大于等于node.str的i
    pos = lower_bound(tempBlock.array, tempBlock.array + tempBlock.num, node) - tempBlock.array;

    //如果找到符合要求的Node，删除，并写回文件
    s = tempBlock.array[pos].str;
    while (pos < tempBlock.num && s == keyString) {
        if (tempBlock.array[pos].offset == node.offset) {//需要str和offset都匹配
            --tempBlock.num;
            for (int i = pos; i < tempBlock.num; ++i) //删除tempBlock.array[pos]
                tempBlock.array[i] = tempBlock.array[i + 1];
            tempBlock.array[tempBlock.num] = Node();

            //写入文件
            fo.seekp(lastp, ios::beg);
            fo.write(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

            //读入下一个块的 num，如果两个块之和够小则合并
            int temp;
            nodep = nextBlock(lastp);
            if (nodep != -1) {
                fi.seekg(nodep + 8, ios::beg);
                fi.read(reinterpret_cast<char *>(&temp), 4);
                if (temp + tempBlock.num < BLOCK_MERGE_THRESHOLD)//todo 合并块的判定为两块元素数和<160
                    mergeBlock(lastp, nodep);
            }
            return 0;//操作成功
        }
        s = tempBlock.array[++pos].str;
    }

    //如果搜索到块末，继续搜索下一个块
    while (pos == tempBlock.num) {
        lastp = nextBlock(lastp);//读入下一个块
        if (lastp == -1) break;
        fi.seekg(lastp, ios::beg);
        fi.read(reinterpret_cast<char *>(&tempBlock), sizeof(Block));
        pos = 0;
        s = tempBlock.array[0].str;
        while (pos < tempBlock.num && s == keyString) {
            if (tempBlock.array[pos].offset == node.offset) {
                --tempBlock.num;
                for (int i = pos; i < tempBlock.num; ++i)
                    tempBlock.array[i] = tempBlock.array[i + 1];
                tempBlock.array[tempBlock.num] = Node();

                fo.seekp(lastp, ios::beg);
                fo.write(reinterpret_cast<char *>(&tempBlock), sizeof(Block));

                int temp;
                nodep = nextBlock(lastp);
                if (nodep != -1) {
                    fi.seekg(nodep + 8, ios::beg);
                    fi.read(reinterpret_cast<char *>(&temp), 4);
                    if (temp + tempBlock.num < BLOCK_MERGE_THRESHOLD)
                        mergeBlock(lastp, nodep);
                }
                return 0;
            }
            s = tempBlock.array[++pos].str;
        }
    }

    return -1;//操作失败
}

#ifdef PaperL_Debug

void blocklist::debugPrint() {
    cout << "debugPrint------------------\\/" << endl;
    cout << fname << endl;
    int debugP;
    Block debugB;

    fip.seekg(0, ios::beg);
    fip.read(reinterpret_cast<char *>(&debugB), sizeof(Block));
    debugP = debugB.nxt;
    cout << "blockOffset: 0" << endl;
    cout << "nxt: " << debugB.nxt << endl;
    cout << "pre: " << debugB.pre << endl;
    cout << "num: " << debugB.num << endl;
    for (int ii = 0; ii < BLOCK_SIZE; ++ii) {
        cout << ii << ": offset = " << debugB.array[ii].offset
             << "\tstr = \"" << debugB.array[ii].str << "\"" << endl;
    }

    while (debugP != -1) {
        fip.seekg(debugP, ios::beg);
        fip.read(reinterpret_cast<char *>(&debugB), sizeof(Block));
        cout << "blockOffset: " << debugP << endl;
        debugP = debugB.nxt;
        cout << "nxt: " << debugB.nxt << endl;
        cout << "pre: " << debugB.pre << endl;
        cout << "num: " << debugB.num << endl;
        for (int ii = 0; ii < BLOCK_SIZE; ++ii) {
            cout << ii << ": offset = " << debugB.array[ii].offset
                 << "\tstr = \"" << debugB.array[ii].str << "\"" << endl;
        }
    }

    cout << "debugPrint------------------/\\" << endl;
}

#endif