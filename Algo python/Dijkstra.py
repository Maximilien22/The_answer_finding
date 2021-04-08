from algopy import graph


def get_dist_and_path(pred, dist, start, goal) :
    path = [goal]
    e = pred[goal]
    while e != start :
        path.append(e)
        e = pred[e]
    path.append(start)
    for i in range(len(path)//2) :
        tmp = path[i]
        path[i] = path[len(path) - 1 - i]
        path[len(path) - 1 - i] = tmp

    return (dist[goal],path)


def del_elem(list, elem) :
    for i in range(len(list)) :
        if list[i] == elem :
            return i

def cost(G, s1 , s2) :
    if s1 == s2 :
        return 0
    for adj in G.adjlists[s1] :
        if adj[0] == s2 :
            return adj[1]
    return -1

def min_of_list(M , dist) :
    res = -1
    for i in range(len(dist)) :
        if i in M :
            if res == -1 or dist[i] < dist[res] :
                if dist[i] != -1 :
                    res = i
    return res
            

def Dijkstra(G, start) :
    M = []
    dist = []
    pred = []
    for i in range(G.order) :
        dist.append(cost(G, start, i))
        pred.append(start)
        M.append(i)

    del M[start]

    while M != [] :
        e = min_of_list(M , dist)
        if e == -1 :
            return (dist,pred)
        del M[del_elem(M,e)]

        for adj in G.adjlists[e] :
            if dist[adj[0]] == -1 :
                dist[adj[0]] = dist[e] + cost(G,e,adj[0])
                pred[adj[0]] = e
            if  dist[e] + cost(G,e,adj[0]) < dist[adj[0]]:
                dist[adj[0]] = dist[e] + cost(G,e,adj[0])
                pred[adj[0]] = e

    return (dist,pred)
