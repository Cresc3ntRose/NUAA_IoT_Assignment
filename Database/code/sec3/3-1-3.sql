SELECT A, B, S.C, S.D, E, F
FROM S, T
WHERE S.C = T.C 
  AND S.D = T.D;