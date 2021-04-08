from algopy import graph
import Dijkstra




G1 = graph.Graph( 6, True, ["","","","","",""])

G1.labels[0] = "Paris"
G1.labels[1] = "Rennes"
G1.labels[2] = "Starsbourg"
G1.labels[3] = "Lyon"
G1.labels[4] = "Marseille"
G1.labels[5] = "Ville Folle"

#CREATE LINK TO PARIS :
graph.Graph.addedge(G1,0,1, 354)
graph.Graph.addedge(G1,0,2, 491)
graph.Graph.addedge(G1,0,3, 463)

#Link to RENNES
graph.Graph.addedge(G1,1,0,354)
graph.Graph.addedge(G1,1,4,1045)

#Link to Starsbourg
graph.Graph.addedge(G1,2,0,491)
graph.Graph.addedge(G1,2,4,801)

#link to Lyon
graph.Graph.addedge(G1,3,0,463)
graph.Graph.addedge(G1,3,4,313)

#link to Marseille 
graph.Graph.addedge(G1,4,5,205)
graph.Graph.addedge(G1,4,1,1045)
graph.Graph.addedge(G1,4,3,313)
graph.Graph.addedge(G1,4,2,801)

#Link to Ville Folle
graph.Graph.addedge(G1,5,1,350)

start = 5
goal = 4

#_______________ALGO DIJKSTRA_______________
l = Dijkstra.Dijkstra(G1, start )
print (l[0])
print(l[1])

l2 = Dijkstra.get_dist_and_path(l[1], l[0], start, goal)

print("La distance de ", end = '')

print(G1.labels[start], end = '')
print(" a " , end = '')
print(G1.labels[goal], end  = '')
print( " est : " , l2[0])
print("Le chemin a prendre est : ", l2[1])
#___________________________________________
