// Author: Tawezy7
// Date: 2024-09-10
#ifndef _INC_GRAPH_H_
#define _INC_GRAPH_H_

#define MAXN 256
#define MAXLENGTH 65536

// 关系类型
typedef int AdjType;
// 顶点类型
typedef char Vtype;

typedef struct graph
{
	Vtype V[MAXN];		//保存图中顶点
	AdjType adj[MAXN][MAXN];	//保存图中顶点的关系集
	
	int Vnum;		//保存顶点个数
	int arcnum;		//保存 边/弧 的数量
	//...	
}Graph;

#ifdef __cplusplus
extern "C" {
#endif
Graph *graphInit(void);
int graphFindVertexIndex(Graph * graph, Vtype v);
void graphVRInput(Graph * graph);
void graphUninit(Graph * graph);
void graphPrint(Graph * graph);
void graphDJSTArrInit(Graph * graph, int Vindex);
void graphDJST(Graph * graph, int Vindex);

#ifdef __cplusplus
}
#endif

#endif /* _INC_GRAPH_H_ */