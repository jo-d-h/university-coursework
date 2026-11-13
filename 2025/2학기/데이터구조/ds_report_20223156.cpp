#include <iostream>
using namespace std;

const int MAX_NODE = 100;

// 희소배열에 저장되는 노드 구조체
typedef struct {
    int idx;   // 논리적 트리 인덱스 (1, 2i, 2i+1, ...)
    int key;   // 실제 저장 값
} SparseNode;

// 희소배열과 노드 개수
SparseNode tree[MAX_NODE];
int nodeCount = 0;

// ------------------------------------------------------
// idx에 해당하는 노드의 "물리적" 위치(i)를 찾는 함수
// 해당 idx를 가진 노드가 없으면 -1 반환
// ------------------------------------------------------
int findByIndex(int idx) {
    for (int i = 0; i < nodeCount; i++) {
        if (tree[i].idx == idx)
            return i;
    }
    return -1;
}

// ------------------------------------------------------
// 최소값 탐색: 시작 인덱스 idx를 루트로 하는 서브트리에서
// 왼쪽 자식(2*idx)을 따라가며 가장 왼쪽(최소값) 인덱스를 찾는다.
// (보고서의 findMinIndexInSubtree 개념과 동일)
// ------------------------------------------------------
int findMinIndexInSubtree(int idx) {
    int cur = idx;
    int pos;

    while (true) {
        pos = findByIndex(cur);
        if (pos == -1) return -1;     // 이 서브트리에 노드가 없다면 종료

        int left = findByIndex(cur * 2);
        if (left == -1) return cur;   // 더 이상 왼쪽 자식이 없으면 최소값

        cur = cur * 2;                // 왼쪽 자식으로 이동
    }
}

// ------------------------------------------------------
// 희소배열에서 pos 위치의 노드를 제거하는 함수
// 마지막 노드를 끌어와서 덮어쓴 뒤 nodeCount를 1 감소시킨다.
// ------------------------------------------------------
void removeFromSparseArray(int pos) {
    tree[pos] = tree[nodeCount - 1];
    nodeCount--;
}

// ------------------------------------------------------
// 삽입 함수: BST 규칙(왼쪽 <, 오른쪽 >)을 이용하여
// 적절한 논리 인덱스를 찾고, 해당 idx가 비어 있으면 삽입
// ------------------------------------------------------
void insertKey(int k) {
    // 트리가 비어 있을 때 첫 노드는 항상 idx=1 (루트)
    if (nodeCount == 0) {
        tree[0].idx = 1;
        tree[0].key = k;
        nodeCount = 1;
        return;
    }

    int idx = 1;  // 루트에서 시작
    while (true) {
        int pos = findByIndex(idx);

        // 현재 idx에 노드가 없으면, 여기가 새 노드 위치
        if (pos == -1) {
            tree[nodeCount].idx = idx;
            tree[nodeCount].key = k;
            nodeCount++;
            return;
        }

        // 이미 같은 key가 있으면 중복 삽입 방지 후 종료
        if (k == tree[pos].key) return;

        // BST 규칙에 따라 왼쪽 또는 오른쪽 자식으로 이동
        if (k < tree[pos].key)
            idx = idx * 2;
        else
            idx = idx * 2 + 1;
    }
}

// ------------------------------------------------------
// 특정 논리 인덱스 idx 위치의 노드를 삭제하는 함수
// 리프 / 자식 1개 / 자식 2개 경우를 모두 처리
// ------------------------------------------------------
void deleteAtIndex(int idx) {
    int pos = findByIndex(idx);
    if (pos == -1) return;  // 해당 idx에 노드가 없으면 종료

    int leftPos  = findByIndex(idx * 2);
    int rightPos = findByIndex(idx * 2 + 1);

    // 1) 자식이 없는 리프 노드인 경우
    if (leftPos == -1 && rightPos == -1) {
        removeFromSparseArray(pos);
        return;
    }

    // 2) 자식이 하나인 경우 (왼쪽 자식만 있는 경우)
    if (leftPos != -1 && rightPos == -1) {
        tree[pos].key = tree[leftPos].key; // 자식의 key를 끌어올림
        deleteAtIndex(idx * 2);            // 자식 위치를 다시 삭제
        return;
    }

    // 2) 자식이 하나인 경우 (오른쪽 자식만 있는 경우)
    if (leftPos == -1 && rightPos != -1) {
        tree[pos].key = tree[rightPos].key;
        deleteAtIndex(idx * 2 + 1);
        return;
    }

    // 3) 자식이 둘 다 있는 경우
    // 오른쪽 서브트리에서 최소값을 가지는 노드의 idx를 찾는다.
    int succIdx = findMinIndexInSubtree(idx * 2 + 1);
    int succPos = findByIndex(succIdx);

    // 후속 노드의 key를 현재 노드로 복사
    tree[pos].key = tree[succPos].key;

    // 실제 삭제는 후속 노드 위치에서 수행
    deleteAtIndex(succIdx);
}

// ------------------------------------------------------
// 키 값 k를 가진 노드를 찾아 삭제하는 함수
// 루트(idx=1)에서 시작하여 BST 탐색을 통해 대상 idx를 찾은 뒤
// deleteAtIndex를 호출하여 실제 삭제를 수행한다.
// ------------------------------------------------------
void deleteKey(int k) {
    int idx = 1;

    while (true) {
        int pos = findByIndex(idx);

        if (pos == -1) return;  // 트리에 해당 값이 없으면 종료

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

// ------------------------------------------------------
// 검색 함수: 값 k가 트리에 존재하면 1, 없으면 0 반환
// 논리적 트리(idx)를 따라 내려가며 findByIndex로 노드 존재 여부 확인
// ------------------------------------------------------
bool searchKey(int k) {
    int idx = 1;

    while (true) {
        int pos = findByIndex(idx);

        if (pos == -1) return false;               // 더 내려갈 노드가 없으면 실패
        if (tree[pos].key == k) return true;       // 같은 key를 찾으면 성공

        if (k < tree[pos].key)
            idx = idx * 2;        // 왼쪽 자식으로 이동
        else
            idx = idx * 2 + 1;    // 오른쪽 자식으로 이동
    }
}

// ------------------------------------------------------
// 현재 희소배열 상태를 출력하는 함수
// 보고서에서 배열 표를 만들 때 이 출력 결과를 활용할 수 있다.
// ------------------------------------------------------
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

// ------------------------------------------------------
// main 함수:
// - 15회 삽입
// - 15회 검색
// - 10회 삭제
// - 삽입 11~15번째 직후, 삭제 1~5번째 직후 배열 상태 출력
//   → 보고서 그림 7-x에 해당하는 데이터로 사용 가능
// ------------------------------------------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 15개의 삽입 데이터 (보고서 내용과 일관되게 사용하면 됨)
    int insertList[15] = {
        50, 30, 70, 20, 40, 60, 80,
        10, 25, 35, 45, 55, 65, 75, 85
    };

    cout << "=== 15회 삽입 ===\n";
    for (int i = 0; i < 15; i++) {
        insertKey(insertList[i]);
        if (i >= 10) {  // 11~15번째 삽입 후 상태 출력
            cout << "\n[" << (i + 1) << "번째 삽입 후 상태]\n";
            printSparseArray();
        }
    }

    // 삽입이 모두 끝난 뒤, 15개의 값에 대해 검색 수행
    cout << "\n=== 15회 검색 ===\n";
    for (int i = 0; i < 15; i++) {
        bool result = searchKey(insertList[i]);
        cout << "search(" << insertList[i] << ") → "
             << (result ? "FOUND" : "NOT FOUND") << "\n";
    }

    // 10개의 삭제 데이터 (예: 보고서 설명과 맞게 자유롭게 설정 가능)
    int del[10] = {20, 40, 30, 60, 50, 10, 25, 70, 35, 80};

    cout << "\n=== 10회 삭제 ===\n";
    for (int i = 0; i < 10; i++) {
        deleteKey(del[i]);
        if (i < 5) {  // 1~5번째 삭제 후 상태 출력
            cout << "\n[" << (i + 1) << "번째 삭제 후 상태]\n";
            printSparseArray();
        }
    }

    return 0;
}