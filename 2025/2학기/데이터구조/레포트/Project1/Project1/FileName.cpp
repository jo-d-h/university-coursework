#include <iostream>
using namespace std;

const int MAX_NODE = 100;

//노드 구조체
typedef struct {
    int idx;//논리적 트리 인덱스
    int key;//실제 저장값
} SparseNode;

//희소배열, 노드개수
SparseNode tree[MAX_NODE];
int nodeCount = 0;

//물리적 위치 찾는 함수, 해당 idx 가진 노드 없으면 -1 반환
int findByIndex(int idx) {
    for (int i = 0; i < nodeCount; i++) {
        if (tree[i].idx == idx)
            return i;
    }
    return -1;
}

//최소값 탐색
int findMinIndexInSubtree(int idx) {
    int cur = idx;
    int pos;

    while (true) {
        pos = findByIndex(cur);
        if (pos == -1) return -1;//서브트리에 노드가 없으면 종료

        int left = findByIndex(cur * 2);
        if (left == -1) return cur;//왼쪽 자식이 없으면 최소값

        cur = cur * 2;//왼쪽자식으로 이동
    }
}

//pos 위치의 노드를 제거하는 함수, 마지막 노드 끌어와서 덮어쓴 뒤 nodeCount 1 감소
void removeFromSparseArray(int pos) {
    tree[pos] = tree[nodeCount - 1];
    nodeCount--;
}

// 삽입 함수
void insertKey(int k) {
    //트리가 비어 있을 때 첫 노드는 항상 idx=1
    if (nodeCount == 0) {
        tree[0].idx = 1;
        tree[0].key = k;
        nodeCount = 1;
        return;
    }

    int idx = 1;//루트에서 시작
    while (true) {
        int pos = findByIndex(idx);

        //현재 idx에 노드가 없으면 새노드 위치
        if (pos == -1) {
            tree[nodeCount].idx = idx;
            tree[nodeCount].key = k;
            nodeCount++;
            return;
        }

        //같은 key가 있으면 중복 삽입 방지 후 종료
        if (k == tree[pos].key) return;

        //왼쪽, 오른쪽 자식으로 이동
        if (k < tree[pos].key)
            idx = idx * 2;
        else
            idx = idx * 2 + 1;
    }
}

//특정 논리 idx 위치 노드 삭제 함수
void deleteAtIndex(int idx) {
    int pos = findByIndex(idx);
    if (pos == -1) return;//해당 idx에 노드 없으면 종료

    int leftPos = findByIndex(idx * 2);
    int rightPos = findByIndex(idx * 2 + 1);

    //자식이 없는 리프 노드
    if (leftPos == -1 && rightPos == -1) {
        removeFromSparseArray(pos);
        return;
    }

    //자식이 하나
    if (leftPos != -1 && rightPos == -1) {
        tree[pos].key = tree[leftPos].key;
        deleteAtIndex(idx * 2);
        return;
    }

    if (leftPos == -1 && rightPos != -1) {
        tree[pos].key = tree[rightPos].key;
        deleteAtIndex(idx * 2 + 1);
        return;
    }

    //자식이 둘 다 있을 때
    int succIdx = findMinIndexInSubtree(idx * 2 + 1);
    int succPos = findByIndex(succIdx);
    tree[pos].key = tree[succPos].key;
    deleteAtIndex(succIdx);
}

//삭제 함수
void deleteKey(int k) {
    int idx = 1;

    while (true) {
        int pos = findByIndex(idx);

        if (pos == -1) return;//트리에 해당 값 없으면 종료

        if (k == tree[pos].key) {
            deleteAtIndex(idx);
            return;
        }

        if (k < tree[pos].key)
            idx = idx * 2;
        else
            idx = idx * 2 + 1;
    }
}

//검색 함수
bool searchKey(int k) {
    int idx = 1;

    while (true) {
        int pos = findByIndex(idx);

        if (pos == -1) return false;//더 내려갈 노드가 없으면 실패
        if (tree[pos].key == k) return true;//같은 key 찾으면 성공

        if (k < tree[pos].key)
            idx = idx * 2;//왼쪽 자식 이동
        else
            idx = idx * 2 + 1;//오른쪽 자식 이동
    }
}

//상태 출력 함수
void printSparseArray() {
    cout << "현재 희소배열 상태 (nodeCount=" << nodeCount << "):\n";
    cout << "  i\tidx\tkey\n";
    for (int i = 0; i < nodeCount; i++) {
        cout << "  [" << i << "]\t"
            << tree[i].idx << "\t"
            << tree[i].key << "\n";
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    //삽입
    int insertList[15] = {
        50, 30, 70, 20, 40, 60, 80,
        10, 25, 35, 45, 55, 65, 75, 85
    };

    cout << "=== 15회 삽입 ===\n";
    for (int i = 0; i < 15; i++) {
        insertKey(insertList[i]);
            cout << "\n[" << (i + 1) << "번째 삽입 후 상태]\n";
            printSparseArray();
    }

    //삽입 끝나고 15개의 값에 대해 검색
    cout << "\n=== 15회 검색 ===\n";
    for (int i = 0; i < 15; i++) {
        bool result = searchKey(insertList[i]);
        cout << "search(" << insertList[i] << ") → "
            << (result ? "FOUND" : "NOT FOUND") << "\n";
    }

    //삭제
    int del[10] = { 20, 40, 30, 60, 50, 10, 25, 70, 35, 80 };

    cout << "\n=== 10회 삭제 ===\n";
    for (int i = 0; i < 10; i++) {
        deleteKey(del[i]);
            cout << "\n[" << (i + 1) << "번째 삭제 후 상태]\n";
            printSparseArray();
    }

    return 0;
}
