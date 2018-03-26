% --------------------------------------------------TESTING INSTRUCTIONS----------------------------------------------------------------------------------
% 	Note that this program utilizes FD Domain, so it should be tested on gnu prolog.
% 	board parts defind by 7 number as below:
% 	water is 0
% 	submerine is 1
% 	destroyer and cruiser top edge is 2
% 	destroyer and cruiser bottom edge is 3
% 	destroyer and cruiser left edge is 4
% 	destroyer and cruiser right edge is 5
% 	cruiser middle part is 6
% 	run the function battleship, with the input of the 36 squares as a list of length 36, with _ where there are blanks pieces on the puzzle in the following configuration:
% 	for example, in order to solve the hashi problem that looks like this :
%								when the row constrain is [2,3,1,1,2,1] and the column constrain is [2,3,1,1,1,2]
%								|_	_	_	_	_	_|
%								|_	_	4	_	0	_|
%								|_	_	_	_	_	_|
%								|_	_	_	_	_	_|
%								|_	_	_	_	_	_|
%								|_	_	_	_	_	_|
%	Give the following input: battleship([_,_,_,_,_,_,_,_,4,_,0,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_],[2,3,1,1,2,1],[2,3,1,1,1,2],Solution).
%	With that input, the output was: Solution = [2,0,0,0,0,1,3,0,4,5,0,0,0,0,0,0,0,1,0,2,0,0,0,0,0,6,0,0,1,0,0,3,0,0,0,0]
%	which meaning the solution is:
%								|2	0	0	0	0	1|
%								|3	0	4	5	0	0|
%								|0	0	0	0	0	1|
%								|0	2	0	0	0	0|
%								|0	6	0	0	1	0|
%								|0	3	0	0	0	0|
% --------------------------------------------------------------------------------------------------------------------------------------------------------
% valid board column and row limits.
validLimits([],[]).
validLimits([[Head|Tail]|RsLists],[LimitHead|LimitTail]) :- 
	Count is 6 - LimitHead,
	fd_exactly(Count, [Head|Tail],0),
	validLimits(RsLists, LimitTail).


getElement([X|_],1,X).
getElement([_|L],K,X) :- 
	getElement(L,K1,X), 
	K is K1 + 1.
	
validateWater([], _).
validateWater([Head|Tail],[HeadSolution|TailSolution]) :-
	(Head > 0 , Head < 37
	->	getElement([HeadSolution|TailSolution],Head,Element) , Element #= 0,
		validateWater(Tail,[HeadSolution|TailSolution])
	; 	validateWater(Tail,[HeadSolution|TailSolution])
	).

removeMembers([],FilterList,FilterList).
removeMembers([Head|Tail],List,FilterList) :-
	delete(List,Head,NextList),
	removeMembers(Tail,NextList,FilterList).
	
concat([], List, List).
concat([Head|Tail], List, [Head|Rest]) :-
    concat(Tail, List, Rest).
	
getNeighbors([],List,List).
getNeighbors([Head|Tail],CurrList,List) :-
	DownRaw is Head - 6,
	UpRaw is Head + 6,
	LeftCol is Head + 1,
	RightCol is Head - 1,
	DiagonalUpR is Head - 7,
	DiagonalUpL is Head - 5,
	DiagonalDownR is Head + 7,
	DiagonalDownL is Head + 5,
	(0 is Head mod 6
	->	TempList = [DownRaw,UpRaw,RightCol,DiagonalUpR,DiagonalDownL]
	;
	1 is Head mod 6
	->	TempList = [DownRaw,UpRaw,LeftCol,DiagonalUpL,DiagonalDownR]
	;
		TempList = [DownRaw,UpRaw,LeftCol,RightCol,DiagonalUpR,DiagonalUpL,
			DiagonalDownR,DiagonalDownL]
	),
	concat(TempList,CurrList,NextList),
	getNeighbors(Tail,NextList,List).

% valid that no other ship is adjustment to the one found.	
validAdjacentWater([], _).
validAdjacentWater([Head|Tail],[HeadSolution|TailSolution]) :-
	getNeighbors([Head|Tail],[],List),
	removeMembers([Head|Tail],List,FilterList),
	validateWater(FilterList,[HeadSolution|TailSolution]).

% check that the optionals ships are in the board limit, 1-36.
checkListLimit([]).
checkListLimit([Head|Tail]) :-
	Head > 0 , Head < 37,
	checkListLimit(Tail).

% get the board values when the position is [Head|Tail] and return it with List. 	
getValue([], _,[]).
getValue([Head|Tail],[HeadSolution|TailSolution],List) :-
	getElement([HeadSolution|TailSolution],Head,Element),
	getValue(Tail,[HeadSolution|TailSolution],NewList),
	List = [Element|NewList].

% look for Destroyer in the positions is [[Head|Tail]|RsLists].	
findDestroyer([],_,_) :-
	fail.
findDestroyer([[Head|Tail]|RsLists],[HeadSolution|TailSolution],[Fst,Sec|TailParts]) :-
	(	checkListLimit([Head|Tail]),
		getValue([Head|Tail],[HeadSolution|TailSolution],List),
		getElement(List,1,FstEdge),getElement(List,2,SecEdge),
		FstEdge #= Fst,
		SecEdge #= Sec,
		validAdjacentWater([Head|Tail],[HeadSolution|TailSolution])
	;	findDestroyer(RsLists,[HeadSolution|TailSolution],TailParts)
	).

% check if there exist a destoyer that one of her parts at N.	
validDestroyer(N,[HeadSolution|TailSolution]) :-
	Down is N+6,
	DownDestroyer = [Down,N],
	Up is N-6,
	UpDestroyer = [N,Up],
	Left is N-1,
	LeftDestroyer = [Left,N],
	Right is N+1,
	RightDestroyer = [N,Right],
	PuzzleParts = [3,2,3,2,4,5,4,5],
	findDestroyer([UpDestroyer,DownDestroyer,RightDestroyer,LeftDestroyer],[HeadSolution|TailSolution],PuzzleParts).

% look for Cruiser in the positions is [[Head|Tail]|RsLists].	
findCruiser([],_,_) :-
	fail.
findCruiser([[Head|Tail]|RsLists],[HeadSolution|TailSolution],[Fst,Sec,Trd|TailParts]) :-
	(	checkListLimit([Head|Tail]),
		getValue([Head|Tail],[HeadSolution|TailSolution],List),
		getElement(List,1,FstEdge),getElement(List,2,Middle),
		getElement(List,3,SecEdge),
		Fst #= FstEdge,
		Sec #= Middle,
		Trd #= SecEdge,
		validAdjacentWater([Head|Tail],[HeadSolution|TailSolution])
	;	findCruiser(RsLists,[HeadSolution|TailSolution],TailParts)
	).

% check if there exist a Cruiser that one of her parts at N.	
validCruiser(N,[HeadSolution|TailSolution]) :-
	Down is N+6,
	DownTwo is N + 12,
	Up is N-6,
	UpTwo is N-12,
	Left is N-1,
	LeftTwo is N-2,
	Right is N+1,
	RightTwo is N+2,
	VerticalCruiser = [Up,N,Down],
	VerticalUpCruiser = [UpTwo,Up,N],
	VerticalDownCruiser = [N,Down,DownTwo],
	HorizontalCruiser = [Left,N,Right],
	HorizontalLeftCruiser = [LeftTwo,Left,N],
	HorizontalRightCruiser = [N,Right,RightTwo],
	PuzzleParts = [2,6,3,2,6,3,2,6,3,4,6,5,4,6,5,4,6,5],
	findCruiser([VerticalCruiser,VerticalUpCruiser,VerticalDownCruiser,HorizontalCruiser,
					HorizontalRightCruiser,HorizontalLeftCruiser],[HeadSolution|TailSolution],PuzzleParts).
	

% validate that all the ships are found on the board and that they satisfy all the conditions.
validShips(_,37,_,0,0,0):- !.
validShips([Head|Tail],N,[HeadSolution|TailSolution],SubmarinesCount,DestroyersCount,CruiserCount):-
	((Head #= 0 , 
	SubmarinesCountNext is SubmarinesCount,
	DestroyersCountNext is DestroyersCount,
	CruiserCountNext is CruiserCount
	)
	;
	(
	Head #= 1 , 
	SubmarinesCountNext is SubmarinesCount - 1,
	DestroyersCountNext is DestroyersCount,
	CruiserCountNext is CruiserCount,
	Water = [N],
	validAdjacentWater(Water,[HeadSolution|TailSolution])
	)
	;
	(Head #> 1,
	Head #< 6,
	SubmarinesCountNext is SubmarinesCount,
	DestroyersCountNext is DestroyersCount - 1,
	CruiserCountNext is CruiserCount,
	validDestroyer(N,[HeadSolution|TailSolution])
	)
	;
	(Head #> 1,
	Head #< 7,
	SubmarinesCountNext is SubmarinesCount,
	DestroyersCountNext is DestroyersCount,
	CruiserCountNext is CruiserCount - 1,
	validCruiser(N,[HeadSolution|TailSolution])
	)
	),
	Next is N + 1,
	validShips(Tail,Next,[HeadSolution|TailSolution],SubmarinesCountNext,DestroyersCountNext,CruiserCountNext).

% it runs on GNU Prolog
% board parts defind by 7 number as below:
% water is 0
% submerine is 1
% destroyer and cruiser top edge is 2
% destroyer and cruiser bottom edge is 3
% destroyer and cruiser left edge is 4
% destroyer and cruiser right edge is 5
% cruiser middle part is 6	
battleship(Puzzle,Raw_lim,Col_lim,Solution) :-
	Solution = Puzzle,
	Puzzle = [	S11, S12, S13, S14, S15 , S16,
			S21, S22, S23, S24, S25 , S26,
			S31, S32, S33, S34, S35 , S36,
			S41, S42, S43, S44, S45 , S46,
			S51, S52, S53, S54, S55 , S56,
			S61, S62, S63, S64, S65 , S66],
	fd_domain(Solution, 0, 6),
	Row1 = [S11, S12, S13, S14, S15 , S16], 
	Row2 = [S21, S22, S23, S24, S25 , S26], 
	Row3 = [S31, S32, S33, S34, S35 , S36], 
	Row4 = [S41, S42, S43, S44, S45 , S46],
	Row5 = [S51, S52, S53, S54, S55 , S56],
	Row6 = [S61, S62, S63, S64, S65 , S66],
	Col1 = [S11, S21, S31, S41, S51, S61], 
	Col2 = [S12, S22, S32, S42, S52, S62], 
	Col3 = [S13, S23, S33, S43, S53, S63], 
	Col4 = [S14, S24, S34, S44, S54, S64],
	Col5 = [S15, S25, S35, S45, S55, S65],
	Col6 = [S16, S26, S36, S46, S56, S66],
	append(Raw_lim, Col_lim,Limitlist),
	validLimits([	Row1, Row2, Row3, Row4, Row5, Row6,
		Col1, Col2, Col3, Col4, Col5, Col6], Limitlist),
	validShips(Solution,1,Solution,3,4,3).
	
	
	
		
	
	
		


