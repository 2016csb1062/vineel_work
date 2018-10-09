import networkx as nx 
import matplotlib.pyplot as  plt 
import random
import itertools


#randomRelationNetwork
#argument : integer
#returns a random relationship network
def randomRelationNetwork(n):
	G = nx.Graph()
	G.add_nodes_from([i for i in range(n)])
	signs = ['+','-']
	for i in G.nodes():
		for j in G.nodes():
			if i!=j:
				G.add_edge(i,j,sign = random.choice(signs))
	return G;	

#stabilizeNetwork
#arguments : a relationShip network
#stabilizes the relationship network
#returns dict which has edge (u,v) as key and sign of the edge as value
def stabilizeNetwork(G):
	unstableSigns = []
	unstable = True
	n = len(G.nodes())
	trilist = [list(x) for x in itertools.combinations(G.nodes(),3)]
	edgeLabels = nx.get_edge_attributes(G,'sign');
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


	return edgeLabels;



#calEpsilon
#arguments : edgeLabels (dict with edges as keys and sign of edge as value)
#			  n (number of nodes in the graph)
#returns the ratio of unstable traingle to the total number of unstable triangles in the network
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


#calNodesLabels
#arguments : edgeLabels (dict with edges as keys and sign of edge as value)
#			 n (number of nodes in the graph)
#			 nodeLabels (list which contains the number of unstable triangle a node present in)
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

#calDeltaAB
#arguments : A (list of nodes in GroupA)
#			 B (list of nodes in GroupB)
#			 edgeLabels (dict with edges as keys and sign of edge as value)
#returns ratio of edges which have positive edges between A and B

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

#calDelta
#arguments : temp (list of nodes in GroupA/GroupB)
#			 edgeLabels (dict with edges as keys and sign of edge as value)
#returns ratio of edges that are negitive in A/B
def calDelta(temp,edgeLabels):
	sign = "+"
	edgeList = [list(x) for x in itertools.combinations(temp,2)]
	delta = 0
	for edge in edgeList:
		if(edge[0]>edge[1]):
			i = edge[1]
			j = edge[0]
		else:
			i = edge[0]
			j = edge[1]
		if(edgeLabels[(i,j)] != sign):
			delta+=1
	return float(delta)/len(edgeList)







#----------------------------------- Main Code ---------------------------------------

#number of nodes in the Graph
n = 200

#create a random network
G = randomRelationNetwork(n)

#stabilzes the random network created
edgeLabels = stabilizeNetwork(G)

print "Random Balanced Network of "+str(n)+" nodes is Created .... "

#display the network
"""pos = nx.circular_layout(G)
print "displaying a random balanced Relationship network......."
nx.draw(G,pos,with_labels = True,node_size = 5000)
edgeLabels = nx.get_edge_attributes(G,'sign')
nx.draw_networkx_edge_labels(G,pos,edge_labels = edgeLabels,font_size = 20 , font_color = 'red')
plt.show()
"""
maxError = 0.001

#adding Error in the graph
epsilon = 0

#Add Error in the network
print "Adding Error ....."
randomEdge = random.choice(edgeLabels.keys())
while(maxError>epsilon):
	randomEdge = random.choice(edgeLabels.keys())
	if(edgeLabels[randomEdge]=='+'):
		edgeLabels[randomEdge] = '-'
	else:
		edgeLabels[randomEdge] = '+'
	epsilon = calEpsilon(edgeLabels,n)


if(edgeLabels[randomEdge]=='+'):
	edgeLabels[randomEdge] = '-'
else:
	edgeLabels[randomEdge] = '+'
epsilon = calEpsilon(edgeLabels,n)

print "Created a Random Network with "+ str((1-epsilon)*100)+ " % balanced triangles....."
"""
print "displaying a network with "+str((1-epsilon)*100)+" % balanced triangles....."
pos = nx.circular_layout(G)
nx.draw(G,pos,with_labels = True,node_size = 400)
nx.draw_networkx_edge_labels(G,pos,edge_labels = edgeLabels,font_size = 20 , font_color = 'red')
plt.show()

"""

nodesLabels = [0 for i in range(n)]
calNodesLabels(edgeLabels,n,nodesLabels)

#get the node which has low number of unbalanced triangles
pivotNode = min(nodesLabels)


GroupA=[]
GroupB=[]

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


print "taking pivot as Node " + str(pivotNode)+" ..... "
print "Group A : "
print GroupA
print "Group B : "
print GroupB


print ""
print "Percentage of balanced triangles = " + str((1-epsilon)*100)
print ""
#  								Case1
#-----------------------------------------------------------------------#
if(len(GroupA) > 0.9*n or len(GroupB) > 0.9*n):
	print "Case 1 : "
	if(len(GroupA) > 0.9*n):
		temp = GroupA
		print "Group A is more than 90%"
	else:
		temp = GroupB
		print "Group B is more than 90%"

	delta = calDelta(temp,edgeLabels)
	print "Percentage of positive edges in the Group = " + str((1-delta)*100) 

#									Case2
#------------------------------------------------------------------------#
else:
	print "Case 2 : "
	print ""
	expDelta = (1-(epsilon)**(1.0/3.0))*100
	print ""
	print "According To Apprx Balance Theorem : "
	print ""
	print "Atleast "+str(expDelta)+"% of positive edges in A "
	print ""
	print "Atleast "+str(expDelta)+"% of positive edges in B "
	print ""
	print "Atleast "+str(expDelta)+"% of negitive edges across A and B "

	print " Result : "
	print ""
	DeltaA = calDelta(GroupA,edgeLabels)
	print "Percentage of positive edges in the GroupA : " + str((1-DeltaA)*100)
	print ""
	DeltaB = calDelta(GroupB,edgeLabels)
	print "Percentage of positive edges in the GroupB : " + str((1-DeltaB)*100)
	print ""
	DeltaAB = calDeltaAB(GroupA,GroupB,edgeLabels)
	print "Percentage of negitive edges across the Group A and B : " + str((1-DeltaAB)*100) 


plt.show()

