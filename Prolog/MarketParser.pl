% --------------------------------------------------TESTING INSTRUCTIONS----------------------------------------------------------------------------------
% 	this program runs on SWI-Prolog.
% 	run the function readcsv with the input of the csv file and a varible for the solution,best average return market.
% 	for example, in order get the best average return market with the example csv file,30days(note the csv file should be at the same directory as this file),
% 	the following input is: readcsv('30days.csv',Solution).
%	With that input, the output was: Solution = 'FINRA/NASDAQ TRF'
% --------------------------------------------------------------------------------------------------------------------------------------------------------
% get the maximum element on the list.
maxList([A],A).
maxList([A|List],Max):- 
	maxList(List,MaxN),
	(A>=MaxN, Max=A ; A<MaxN, Max=MaxN).

% add element to the end of the list.
addLast(X,[],[X]).
addLast(X,[A|L],[A|L1]):-
 addLast(X,L,L1).
 
flattenList([Head|[Tail]],[Head|Tail]).
  
% add to val to the n-th element on the list.
ins(Val, [H|T], 1, [Head|T]):-
	Head is H + Val.
ins(Val,[H|List],Pos,[H|Res]):- 
	Pos > 1, !, 
    Pos1 is Pos - 1, ins(Val,List,Pos1,Res). 

% get the index of the element on the list.
indexOf([Element|_], Element, 1):- !.
indexOf([_|Tail], Element, Index):-
  indexOf(Tail, Element, Index1),
  !,
  Index is Index1+1.

% get the n-th element on the list.
getElement([X|_],1,X):- !.
getElement([_|L],K,X) :- 
	K1 is K - 1,
	getElement(L,K1,X).
	
% return market and total index	
getMarketTotalIndex(Rows,MarketIndex,TotalIndex,Pos) :-
	getElement(Rows,Pos,FirstRow),
	FirstRow =..Temp,
	flattenList(Temp,FRow),
	indexOf(FRow,'Exchange',MarketIndex),
	indexOf(FRow,'Total %',TotalIndex).

% parse xml lines, line by line. MarketList is the list contain all the markets name.
% ValueList is a list containing the total value for each market.
% CountList is a list containing the number of occurrence for each market.
parseLines(_,Len,Len,_,_,MarketList,MarketList,ValueList,ValueList,CountList,CountList).
parseLines(Rows,Len,Pos,MarketIndex,TotalIndex,MarketList,MarketListFinal,CurrValueList,ValueList,CurrCountL,CountList) :-
	getElement(Rows,Pos,CurrRow),
	CurrRow =..Temp,
	flattenList(Temp,CRow),
	NextPos is Pos + 1,
	getElement(CRow,MarketIndex,CurrMarket),
	getElement(CRow,TotalIndex,TotalTemp),
	atom_concat(CTotal,'%',TotalTemp),
	atom_number(CTotal,CurrTotal),
	(member(CurrMarket,MarketList)
	->	indexOf(MarketList,CurrMarket,CurrMarketIndex),
		NewMarketList = MarketList,
		ins(CurrTotal,CurrValueList,CurrMarketIndex,NewValueList),
		ins(1,CurrCountL,CurrMarketIndex,NewCountL)
	;	addLast(CurrMarket,MarketList,NewMarketList),
		addLast(CurrTotal,CurrValueList,NewValueList),
		addLast(1,CurrCountL,NewCountL)
	),
	parseLines(Rows,Len,NextPos,MarketIndex,TotalIndex,NewMarketList,MarketListFinal,NewValueList,ValueList,NewCountL,CountList).

% calculate the average total value for each element on the list given the number of his occurrence.
calculateAverage([],[],AverageList,AverageList).	
calculateAverage([H|T],[CountHead|CountTail],CurrAverageList,AverageList):-
	CurrAverage is H / CountHead,
	addLast(CurrAverage,CurrAverageList,NewAverageList),
	calculateAverage(T,CountTail,NewAverageList,AverageList).

read_file(File, Lines) :-
   open(File, read, _, [alias(in)]),
   read_string(in, "\n", "\r\t ", Next, Line),
   atomic_list_concat(CleanLine,',',Line),
   read_lines(row(CleanLine),Next,Lines).


read_lines(_,-1 ,[]).
read_lines(row(L),_ ,[row(L)|Lines]) :-
   read_string(in, "\n", "\r", Next,NextLine),
   atomic_list_concat(CleanNextLine,',',NextLine),
   !, read_lines(row(CleanNextLine),Next, Lines).
   
% this is the MAIN METHOD.it runs on SWI-Prolog.
readcsv(File,LargestMarket) :-  
   read_file(File, Rows),
   length(Rows,L),
   Len is L + 1,
   getMarketTotalIndex(Rows,MarketIndex,TotalIndex,1),
   parseLines(Rows,Len,2,MarketIndex,TotalIndex,[],MarketList,[],ValueList,[],CountList),
   calculateAverage(ValueList,CountList,[],AverageList),
   maxList(AverageList,MaxValue),
   indexOf(AverageList,MaxValue,MaxIndex),
   getElement(MarketList,MaxIndex,LargestMarket).
   
