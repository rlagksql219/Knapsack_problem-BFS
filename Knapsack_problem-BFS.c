#include<stdio.h>
#include<malloc.h>


int n = 5;
int W = 9;
int p[6] = { 0,20,30,35,12,3 };
int w[6] = { 0,2,5,7,3,1 };
int maxprofit = 0;

typedef struct node {
	int level;
	int profit;
	int weight;
	struct node* next;
}node;

typedef struct Queue {
	node* front; //맨 앞
	node* rear;
	int count;
}queue_of_node;


void initialize(queue_of_node** Q); //큐를 초기화하는 함수
int empty(queue_of_node* Q); //큐가 비어있는지 확인하는 함수
void enqueue(queue_of_node* Q, node* v); //노드를 큐에 넣는 함수
node* dequeue(queue_of_node* Q); //큐에서 꺼낸 노드를 리턴하는 함수

void kanpsack(); //knapsack problem을 풀기 위해 상태공간트리를 구축하는 함수
float bound(node u); //bound를 계산하는 함수


int main(void) {
	kanpsack();
	printf("\n<최대이익 : %d>\n", maxprofit);
}


//큐를 초기화하는 함수
void initialize(queue_of_node** Q) {
	(*Q) = (queue_of_node*)malloc(sizeof(queue_of_node));
	(*Q)->front = NULL;
	(*Q)->rear = NULL;
	(*Q)->count = 0;
}


//큐가 비어있는지 확인하는 함수
int empty(queue_of_node* Q) {
	if (Q->count == 0)
		return 1;
	else
		return 0;
}


//노드를 큐에 넣는 함수
void enqueue(queue_of_node* Q, node* v) {

	node* k = (node*)malloc(sizeof(node)); //노드생성

	//큐에 넣을 노드의 정보 복사
	k->level = v->level;
	k->profit = v->profit;
	k->weight = v->weight;
	k->next = v->next;

	if (Q->count == 0) { //큐가 비어있는 경우
		Q->front = Q->rear = k;
	}
	else { //큐에 다른 노드가 있는경우
		Q->rear->next = k;
		Q->rear = k;
	}
	Q->count++;
}


//큐에서 꺼낸 노드를 리턴하는 함수
node* dequeue(queue_of_node* Q) {

	node* n = Q->front;

	if (Q->front->next == NULL) {
		Q->front = NULL;
		Q->rear = NULL;
	}
	else {
		Q->front = Q->front->next;
	}

	Q->count--;
	return n;
}


//knapsack problem을 풀기 위해 상태공간트리를 구축하는 함수
void kanpsack() {
	queue_of_node* Q;
	node u, v;

	initialize(&Q); //큐 초기화
	v.level = 0; v.profit = 0; v.weight = 0; v.next = NULL; u.next = NULL; //v가 root node가 되게 초기화
	maxprofit = 0;
	enqueue(Q, &v); //노드를 큐에 넣음
	printf("노드 큐에 추가(level=%d,profit=%d,weight=%d,bound=%.2f),큐의 노드개수:%d\n", v.level, v.profit, v.weight, bound(v), Q->count);

	while (!empty(Q)) {
		node* v = dequeue(Q);
		printf("\n노드 큐에서 제거(level=%d,profit=%d,weight=%d),큐의 노드개수:%d\n", v->level, v->profit, v->weight, Q->count);

		u.level = (v->level) + 1;
		u.profit = (v->profit) + p[u.level];
		u.weight = (v->weight) + w[u.level];


		if ((u.weight <= W) && (u.profit > maxprofit)) {
			maxprofit = u.profit;
			printf("maxprofit : %d\n", maxprofit);

		}

		if (bound(u) > maxprofit) { //다음 아이템을 포함하는 노드
			enqueue(Q, &u);
			printf("노드 큐에 추가(level=%d,profit=%d,weight=%d,bound=%.2f),큐의 노드개수:%d\n", u.level, u.profit, u.weight, bound(u), Q->count);

		}

		u.weight = v->weight;
		u.profit = v->profit;

		if (bound(u) > maxprofit) { //다음 아이템을 포함하지 않는 노드
			enqueue(Q, &u);
			printf("노드 큐에 추가(level=%d,profit=%d,weight=%d,bound=%.2f),큐의 노드개수:%d\n", u.level, u.profit, u.weight, bound(u), Q->count);
		}
	}
}


//bound를 계산하는 함수
float bound(node u) {
	int j, k;
	int totweight;
	float result;

	if (u.weight >= W)
		return 0;

	else {
		result = (float)u.profit;
		j = u.level + 1;
		totweight = u.weight;

		while (j <= n && totweight + w[j] <= W) {
			totweight = totweight + w[j];
			result = result + p[j];
			j++;
		}

		k = j;

		if (k <= n) { //k째 item의 일부분을 취함
			result = result + (W - totweight) * (p[k] / w[k]);
		}

		return result;
	}
}