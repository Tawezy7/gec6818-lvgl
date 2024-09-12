#include "graph.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// 初始化图结构体
Graph *graphInit()
{
	Graph *graph = malloc(sizeof(Graph));

	memset(graph->V, 0, sizeof(Vtype) * MAXN);
	memset(graph->adj, 0, sizeof(AdjType) * MAXN * MAXN);

	graph->Vnum = graph->arcnum = 0;

	return graph;
}

// 找顶点所对应的下标
int graphFindVertexIndex(Graph *graph, Vtype v)
{
	if (graph)
	{
		for (int i = 0; i < graph->Vnum; i++)
		{
			if (graph->V[i] == v)
				return i;
		}
	}

	return -1;
}

// 数据输入 --- 顶点集和关系集合
void graphVRInput(Graph *graph)
{
	// 输入顶点集合字符串
	scanf("%s", graph->V);
	graph->Vnum = strlen(graph->V);

	// 输入关系集合 无向图
	// 输入格式为A B ， 输入#号结束
	Vtype v1, v2;
	AdjType w;
	int index1, index2;
	while (1)
	{
		getchar();
		scanf("%c %c %d", &v1, &v2, &w);
		if (v1 == '#' || v2 == '#')
		{
			printf("Input finish\n");
			break;
		}

		// 需要找出顶点的下标 并且顶点需要在集合中
		index1 = graphFindVertexIndex(graph, v1);
		index2 = graphFindVertexIndex(graph, v2);
		if (index1 == -1 || index2 == -1)
		{
			printf("Vertex input err\n");
			continue;
		}

		// 将关系添加进入到关系集合中
		graph->adj[index1][index2] = w;
		graph->adj[index2][index1] = w;
	}
}

void graphUninit(Graph *graph)
{
	free(graph);
}

void graphPrint(Graph *graph)
{
	if (graph)
	{
		// 打印顶点集合
		int i, j;
		putchar('\t');
		for (i = 0; i < graph->Vnum; i++)
			printf("%c\t", graph->V[i]);
		printf("\n");

		// 打印关系集合
		for (i = 0; i < graph->Vnum; i++)
		{
			printf("%c\t", graph->V[i]);
			for (j = 0; j < graph->Vnum; j++)
			{
				printf("%d\t", graph->adj[i][j]);
			}
			putchar('\n');
		}
	}
}

// 求最短路径
int s[MAXN] = {0};
int dist[MAXN] = {0};
int prev[MAXN]; // 用于记录路径上的前驱节点

// 对辅助数组进行初始化 参数为图 以及 源点下标
void graphDJSTArrInit(Graph *graph, int Vindex)
{
	// 源点到源点之间的最短路径已被求出
	s[Vindex] = 1;
	prev[Vindex] = -1; // 源点没有前驱

	for (int i = 0; i < graph->Vnum; i++)
	{
		// 如果源点到i对应的顶点之间存在路径
		if (graph->adj[Vindex][i] != 0)
		{
			dist[i] = graph->adj[Vindex][i];
			prev[i] = Vindex; // 记录前驱
		}
		else
		{
			dist[i] = MAXLENGTH;
			prev[i] = -1; // -1表示未访问
		}
	}
}

// 迪杰斯特拉算法  求源点Vindex 到其余各顶点的最短路径
void graphDJST(Graph *graph, int Vindex)
{
	if (graph)
	{
		graphDJSTArrInit(graph, Vindex);

		int n = graph->Vnum;
		while (n--)
		{
			// 找出当前未被标记的路径长度中的最短的路径 作为最优路径
			int min = MAXLENGTH; // MAXLENGTH表示无穷
			int minIndex = -1;
			for (int i = 0; i < graph->Vnum; i++)
			{
				if (s[i] == 0 && dist[i] < min)
				{
					min = dist[i];
					minIndex = i;
				}
			}

			s[minIndex] = 1;

			// 通过最优路径去更新其他路径
			for (int i = 0; i < graph->Vnum; i++)
			{
				if (s[i] == 0 && graph->adj[minIndex][i] != 0 && (graph->adj[minIndex][i] + dist[minIndex] < dist[i]))
				{
					dist[i] = graph->adj[minIndex][i] + dist[minIndex];
					prev[i] = minIndex; // 更新前驱节点
				}
			}
		}

		// 输出每个顶点的最短路径
		for (int i = 0; i < graph->Vnum; i++)
		{
			printf("%c->%c:%d\n", graph->V[Vindex], graph->V[i], dist[i]);
			printf("路径: ");
			int path[MAXN];
			int count = 0;
			int tmp = i;

			// 追踪路径
			while (tmp != -1)
			{
				path[count++] = tmp;
				tmp = prev[tmp];
			}

			// 反向输出路径
			for (int j = count - 1; j >= 0; j--)
			{
				printf("%c", graph->V[path[j]]);
				if (j > 0)
					printf(" -> ");
			}
			printf("\n");
		}
	}
}
