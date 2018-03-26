% --------------------------------------------------TESTING INSTRUCTIONS----------------------------------------------------------------------------------
% Note that this program utilizes FD Domain, so it should be tested on gnu prolog.
% run the function hashi, with the input of the 49 squares as a list of length 49, with zeros where there aren't islands in the following configuration:
% 								----------->
%								----------->
%								----------->
%								----------->
%								----------->
%								----------->
%								----------->
% for example, in order to solve the hashi problem that looks like this :
%								|3	0	2	0	0	0	1|
%								|2	0	0	0	0	0	0|
%								|0	0	0	0	0	0	0|
%								|0	0	0	0	0	0	0|
%								|0	0	0	0	0	0	0|
%								|0	0	0	0	0	0	0|
%								|0	0	0	0	0	0	0|
%	Give the following input: hashi([3,0,2,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],Bridges).
%	With that input, the output was: Bridges = [[3,7,1],[1,3,1],[1,8,2]] 
%	How to understand the solution:
%	Each bridge is a list of 3 element, in the example above we get 3 bridges: [3,7,1],[1,3,1],[1,8,2].
%	The first element is the index of the first island of the bridge, the second is the index of the second island of the bridge, and the third
%	is the amount of bridges between them in the solution (0 means we don't have a bridge, 1 means one bridge, and 2 means 2 bridges).
% 	An index of an island is the location of it in the corresponding list representing the island. Here are the indexes of the islands:
%								|1	2	3	4	5	6	7 |
%								|8	9	10	11	12	13	14|
%								|15	......................|
%								|.........................|
%								|.........................|
%								|.........................|
%								|.........................|
%	Therefore in the example above, we conclude that there is one bridge between islands of index 3,7, one bridge between islands of index 1,3, and
%	two bridges between islands of index 1,8.
% --------------------------------------------------------------------------------------------------------------------------------------------------------

% test the result, to make sure it works.
% test(+Islands,+Solution)
test(Vertices, g(Vs,Es)):-!,
	getIslandIndexes(Vertices,IslandIndexes),
	areDegreesOK(g(Vs,Es)),
	getNonZeroEdges(Es,NZEdges),
	getCleanEdgesLowToHigh(NZEdges,ChkEdges),
	isConnected(IslandIndexes, ChkEdges),
	\+hasCuttingBridges(NZEdges).


% returns whether edges are cutting or not
% hasCuttingBridges(+Edges).
hasCuttingBridges(Edges):-!,
	horizontalBridges(Edges,Horizontals),
	verticalBridges(Edges,Verticals),
	cutting(Horizontals,Verticals).


% checks if there are any cuts between the horizontal and vertical edges
% cutting(+Horizontals,+Verticals).
cutting(Horizontals,Verticals):-!,
	getCleanEdgesLowToHigh(Horizontals,HorizontalsLTH),
	getCleanEdgesLowToHigh(Verticals,VerticalsLTH),
	cuttingOrdered(HorizontalsLTH,VerticalsLTH).


% assuming the edges are ordered and as a pair, it checks if there are any cuts between the bridges
% cuttingOrdered(+OrderedCleanHorizontals,+OrderedCleanVerticals)
cuttingOrdered([[H1,H2]|_],[[V1,V2]|_]):-!,
	V1m is (V1-1) mod 7,
	H1m is (H1-1) mod 7,
	H2m is (H2-1) mod 7,
	H1d is (H1-1) div 7,
	V1d is (V1-1) div 7,
	V2d is (V2-1) div 7,
	H1m @< V1m,
	H2m @> V1m,
	V1d @< H1d,
	V2d @> H1d.
cuttingOrdered([[H1,H2]|Hs],[_|Vs]):-!,
	cuttingOrdered([[H1,H2]|Hs],Vs).
cuttingOrdered([_|Hs],[[V1,V2]|Vs]):-!,
	cuttingOrdered(Hs,[[V1,V2]|Vs]).


% return the edges in ans, in the clean state without the number in it, and when the left coordinate is always the smaller one.
% getCleanEdgesLowToHigh(+Es, -Ans)
getCleanEdgesLowToHigh([],[]).
getCleanEdgesLowToHigh([[V1,V2,_]|Es], Ans):-!,
	( V2 @> V1
	->	getCleanEdgesLowToHigh(Es, AnsRec), append([[V1,V2]],AnsRec, Ans)
	;	getCleanEdgesLowToHigh(Es, AnsRec), append([[V2,V1]],AnsRec, Ans)
	).

% receives bridges, and returns the horizontal ones.
% horizontalBridges(+Edges,-Horizontals).
horizontalBridges([],[]).
horizontalBridges([[V1,V2,Number]|Es], Ans):-!,
	( V1 mod 7 =:= V2 mod 7
	->	horizontalBridges(Es, Ans)
	;	horizontalBridges(Es, AnsRec), append([[V1,V2,Number]],AnsRec, Ans)
	).

% receives bridges, and returns the vertical ones.
% verticalBridges(+Edges,-Verticals).
verticalBridges([],[]).
verticalBridges([[V1,V2,Number]|Es], Ans):-!,
	( V1 mod 7 =:= V2 mod 7
	->	verticalBridges(Es, AnsRec), append([[V1,V2,Number]],AnsRec, Ans)
	;	verticalBridges(Es, Ans)
	).


% returns then number of edges (taking in account that edges have the number) that start on Ind.
% numOfEdgesFromIndex(+Index,+Edges,-Ans)
numOfEdgesFromIndex(_,[],0).
numOfEdgesFromIndex(Ind,[[Ind,_,Number]|Es],Ans):-!,
	numOfEdgesFromIndex(Ind,Es,AnsRec),
	Ans #= AnsRec + Number.
numOfEdgesFromIndex(Ind,[[_,Ind,Number]|Es],Ans):-!,
	numOfEdgesFromIndex(Ind,Es,AnsRec),
	Ans #= AnsRec + Number.
numOfEdgesFromIndex(Ind,[[V1,V2,_]|Es],Ans):-!,
	V1 #\= Ind,
	V2 #\= Ind,
	numOfEdgesFromIndex(Ind,Es,Ans).

% receives a graph, and says if the degree of each vertex in it is the number of edges from it.
% We leave the edges the same in recursion, and check for each vertex in each level of recursion.
% areDegreesOK(+g(Vs,Es)).
areDegreesOK(g([],_)).
areDegreesOK(g([Deg-Ind-_|Vs],Es)):-!,
	numOfEdgesFromIndex(Ind,Es,Ans),
	Ans #= Deg,
	areDegreesOK(g(Vs,Es)).

% returns whether alll Indexes are reachable from Ind using Edges.
% areAllReachable(+Ind,+Indexes,+Edges).
areAllReachable(_,[],_).
areAllReachable(Ind,[Ind1|IndRem],Edges):-!,
	reachable(Ind,Ind1,Edges),
	areAllReachable(Ind,IndRem,Edges).

% returns in all the edges that have a Number not equal to 0. (the real
% bridges in the solution).
% getNonZeroEdges(Edges,Ans)
getNonZeroEdges([],[]).
getNonZeroEdges([[V1,V2,Number]|Es],Ans):-!,
	((Number #= 0,
	getNonZeroEdges(Es,Ans)
	)
	;
	(Number#>0,
	getNonZeroEdges(Es,RecAns),
	append([[V1,V2,Number]],RecAns,Ans)
	)
	).
% returns whether the graph is connected
% isConnected(+Indexes, +Edges).
isConnected([],[]).
isConnected([Ind|Indexes],Edges):-!,
	areAllReachable(Ind, [Ind|Indexes], Edges).

	
	
	
	
	
	
	
	
% returns another index of an island connected to given one, if exists.
% getOtherIndex(+Index1,+Edges, -OtherIndex,-ChosenEdge).
getOtherIndex(Index1,[[Index1,OtherIndex]|_], OtherIndex,[Index1,OtherIndex]).
getOtherIndex(Index1,[[OtherIndex,Index1]|_], OtherIndex,[OtherIndex,Index1]).
getOtherIndex(Index1,[[Ind1,Ind2]|Es], OtherIndex,ChosenEdge):-!,
	Ind1 \= Index1,
	Ind2 \= Index1,
	getOtherIndex(Index1,Es,OtherIndex,ChosenEdge).

% returns if Index2 is reachable from Index1 through the edges of form [V1,V2]
% rechable(+Index1,+Index2,+Edges).
reachable(Index1,Index1, _).
reachable(Index1,Index2, Edges):-!,
	getOtherIndex(Index1, Edges,OtherInd,ChosenEdge),
	delete(Edges,ChosenEdge,NewEdges),
	(reachable(OtherInd, Index2, NewEdges)
	-> A is 0
	; reachable(Index1, Index2, NewEdges)
	).

	
% receives lists of islands in form Deg-Index-Island, and returns a list of the Indexes.
% getIslandIndexes(+Islands,-Indexes).
getIslandIndexes([],[]).
getIslandIndexes([_-Ind-_|Vs],[Ind|AnsRem]):-!,
	getIslandIndexes(Vs,AnsRem).


% get the relevant vertices of islands from the board with each vertex being a 3-long list of [degree, index in board list, number as island].
% getBoardIslands(+Board,+Index,+IslandNumber,-Vertices).
getBoardIslands([],_,_,[]).
getBoardIslands([0|Rest],Index,IslandNumber,Vertices):-!,
	RecursionIndex is Index+1,
	getBoardIslands(Rest,RecursionIndex,IslandNumber,Vertices).
getBoardIslands([X|Rest],Index,IslandNumber,[X-Index-IslandNumber|Vertices]):-!,
	X\=0,
	RecursionIndex is Index+1, RecursionIslandNumber is IslandNumber+1 ,
	getBoardIslands(Rest,RecursionIndex,RecursionIslandNumber,Vertices).
getBoardIslands(Board,Vertices):-!,
	getBoardIslands(Board,1,1,Vertices).

% returns a graph representing the problem.(vertices are those returned in getBoardIslands, and edges are edges between to closest islands, which may be connected)
% getNeighborsGraph(+VerticesRemaining,+AllVertices,-g(Vs,Es)).
getNeighborsGraph([],_,g([],[])).
getNeighborsGraph([Deg-Ind-Isl|Rem],Vertices,g(Vs,Es)):-!,
	getNeighborsGraph(Rem, Vertices, g(VsRec,EsRec)),
	getNewEdges(Deg-Ind-Isl, Vertices, NewEdges),
	append(EsRec,NewEdges,Es),
	append([Deg-Ind-Isl],VsRec,Vs).

% get a possible solution for the Hashi problem, with the right amount of edges for each island. Each move we fill and throw away an edge with a number between 0,2. (the amount of bridges)
% generateSolution(+NeighborsGraph,-Solution).
generateSolution(g(V,[]),g(V,[])).
generateSolution(g(Vertices,[[V1,V2]|Es]),g(Vertices,[[V1,V2,Number]|EdgesRem])):-!,
	fd_domain([Number],0,2),
	generateSolution(g(Vertices,Es),g(Vertices,EdgesRem)).


% get the index on the board Isl'th Island.
% getIndexOfBoard(+Vertices,+Isl,-Ind).
getIndexOfBoard([_-Ind-Isl|_],Isl,Ind).
getIndexOfBoard([_-_-_|Rest],Isl,Ind):-!,
	getIndexOfBoard(Rest,Isl,Ind).

% setting the edge E to be list of V1 and V2.
% getEdge(+V1,+V2,-Edge).
getEdge(Ind,IndexOfRight,[[Ind,IndexOfRight]]).

% the empty list maker (used for unification)
emptyList([]).

% return the list of vertex to the right of the given one if it does exist, else returns [].
% edgeRight(+Island,+Vertices,-RightIsland).
edgeRight(_-_-_, [], []).
edgeRight(Deg-Ind-Isl, [Deg-Ind-Isl|Vs], Bottom):-!,
	edgeRight(Deg-Ind-Isl,Vs, Bottom).
edgeRight(Deg-Ind-Isl, [_-GivenInd-Isl2|Vs], Bottom):-!,
	GivenIndd is  (GivenInd-1) div 7,
	Indd is (Ind-1) div 7,
	(GivenIndd =:= Indd, Isl is Isl2-1
	->	getEdge(Ind,GivenInd,Bottom)
	;	edgeRight(Deg-Ind-Isl,Vs, Bottom)
	).

% returns the maximal Island number in the given vertices
% maxIsland(+Vertices,-Isl).
maxIsland([_-_-Isl],Isl).
maxIsland([_-_-Isl|Xs],Isl):-!, maxIsland(Xs,Y), Isl @>= Y.
maxIsland([_-_-Isl|Xs],N):-!, maxIsland(Xs,N), N @> Isl.

% return whether the given Numberof Isl is maximal among vertices.
% isMaxIsland(+Isl,+Vertices).
% Example: isMaxIsland(12,[1-1-1,1-1-7,1-1-12,1-1-3,1-1-2]).- true
isMaxIsland(Isl,Vertices):-!,
	maxIsland(Vertices,Max),
	Max =:= Isl.

% return the list of vertex to the bottom of the given one if it does
% exist, else returns [].
% edgeBottom(+Island,+Vertices,-BottomIsland).
edgeBottom(_-_-_, [], []).
edgeBottom(Deg-Ind-Isl, [Deg-Ind-Isl|Vs], Bottom):-!,
	edgeBottom(Deg-Ind-Isl,Vs, Bottom).
edgeBottom(Deg-Ind-Isl, [_-GivenInd-_|Vs], Bottom):-!,
	GivenIndm is  GivenInd mod 7,
	Indm is Ind mod 7,
	(GivenIndm =:= Indm, GivenInd @> Ind
	->	getEdge(Ind,GivenInd,Bottom)
	;	edgeBottom(Deg-Ind-Isl,Vs, Bottom)
	).

% returns the new edges of a vertex, by appending each time the right
% edge and then the bottom edge.
% getNewEdges(+Island,+Vertices, -Edges).
getNewEdges(Deg-Ind-Isl,Vertices, Edges):-!,
	edgeBottom(Deg-Ind-Isl, Vertices, Bottom),
	append(Bottom, [], Res1),
	edgeRight(Deg-Ind-Isl,Vertices,Right),
	append(Right,Res1, Edges).

% solves the Hashi problem
% Example: hashi([0,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],Ans).
% hashi(+Board , -Edges)
hashi(Board , Es):-!,
	getBoardIslands(Board,Vertices),
	getNeighborsGraph(Vertices,Vertices,NeighborsGraph),
	generateSolution(NeighborsGraph,g(Vs,Es)),
	test(Vertices,g(Vs,Es)).








