import networkx as nx 
import matplotlib.pyplot as  plt 
import random
import itertools


#create a random relationship network with positive or negitive edges
def randomRelationNetwork(n):
	G = nx.Graph()
	G.add_nodes_from([i for i in range(n)])
	signs = ['+','-']
	for i in G.nodes():
		for j in G.nodes():
			if i!=j:
				G.add_edge(i,j,sign = random.choice(signs))
	return G;	

#stabilizes the relationship network
def stabilizeNetwork(G):
	unstableSigns = []
	unstable = True
	n = len(G.nodes())
	trilist = [list(x) for x in itertools.combinations(G.nodes(),3)]
	edgeLabels = nx.get_edge_attributes(G,'sign');
	print edgeLabels
	while(unstable):
		unstable = False

		#find the unstable triangle and make it stable
		for tri in trilist:
			isStable = 1
			if(edgeLabels[(tri[0],tri[1])] == "-"):
				isStable*=-1
			if(edgeLabels[(tri[1],tri[2])] == "-"):
				isStable*=-1
			if(edgeLabels[(tri[0],tri[2])] == "-"):
				isStable*=-1

			if(isStable == -1 ):
				unstable = True
				#if unstable flip the sign of the first edge in the triangle
				if(edgeLabels[(tri[0],tri[1])] == "-"):
					edgeLabels[(tri[0],tri[1])] = "+"
				else:
					edgeLabels[(tri[0],tri[1])] = "-"



	nx.set_edge_attributes(G,'sign',edgeLabels)


def calEpsilon(edgeLabels,n):
	nods = range(n);
	trilist = [list(x) for x in itertools.combinations(nods,3)]
	unstable = 0 
	for tri in trilist:
			isStable = 1
			if(edgeLabels[(tri[0],tri[1])] == "-"):
				isStable*=-1
			if(edgeLabels[(tri[1],tri[2])] == "-"):
				isStable*=-1
			if(edgeLabels[(tri[0],tri[2])] == "-"):
				isStable*=-1

			if(isStable == -1):
				unstable+=1
	return float(unstable)/len(trilist)


def calNodesLabels(edgeLabels,n,nodesLabels):
	nods = range(n);
	trilist = [list(x) for x in itertools.combinations(nods,3)]
	unstable = 0 
	for tri in trilist:
			isStable = 1
			if(edgeLabels[(tri[0],tri[1])] == "-"):
				isStable*=-1
			if(edgeLabels[(tri[1],tri[2])] == "-"):
				isStable*=-1
			if(edgeLabels[(tri[0],tri[2])] == "-"):
				isStable*=-1

			if(isStable == -1):
				nodesLabels[tri[0]]+=1
				nodesLabels[tri[1]]+=1
				nodesLabels[tri[2]]+=1

def calDeltaAB(A,B,edgeLabels):
	delta = 0
	for nodeA in A:
		for nodeB in B:
			if(nodeA>nodeB):
				i = nodeB
				j= nodeA
			else:
				i = nodeA
				j = nodeB
			if(edgeLabels[(i,j)] != "-"):
				delta+=1
	return float(delta)/(len(A)*len(B))


def calDelta(temp,edgeLabels):
	sign = "+"
	edgeList = [list(x) for x in itertools.combinations(temp,2)]
	delta = 0
	for edge in edgeList:
		if(edgeLabels[(edge[0],edge[1])] != sign):
			delta+=1
			print edge[0] , 
			print edge[1]
	return float(delta)/len(edgeList)

#number of nodes in the Graph
n = 10

#create a random network
G = randomRelationNetwork(n)

#stabilzes the random network created
stabilizeNetwork(G)

pos = nx.circular_layout(G)
nx.draw(G,pos,with_labels = True,node_size = 5000)
edgeLabels = nx.get_edge_attributes(G,'sign')
nx.draw_networkx_edge_labels(G,pos,edge_labels = edgeLabels,font_size = 20 , font_color = 'red')
plt.show(block = False)
minError = 0.001

#adding Error in the graph
epsilon = 0


edgeLabels = nx.get_edge_attributes(G,'sign');

pivotNode = 0 

GroupA=[]
GroupB=[]


print "taking pivot as " + str(pivotNode)
GroupA+=[pivotNode]

for node in range(n):
	if(node == pivotNode):
		continue
	if(pivotNode>node):
		i=node
		j=pivotNode
	else:
		i = pivotNode
		j = node
	if(edgeLabels[(i,j)] == "+"):
		GroupA+=[node]
	else:
		GroupB+=[node]

print GroupA
print GroupB

print "Epsilon = " + str(epsilon)

deltaA = calDeltaAB(GroupA,GroupB,edgeLabels)

print deltaA
plt.show()
