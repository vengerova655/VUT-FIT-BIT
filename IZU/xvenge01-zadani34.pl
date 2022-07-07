%autor = xvenge01
neparne(N) :- 1 is mod(N,2).

uloha34([], []) :- !.
uloha34([H|T], R) :- neparne(H) ,!, VYSL is H*2, R=[VYSL|T1], uloha34(T,T1).
uloha34([H|T], R) :- R=[H|T1], uloha34(T,T1).
