SELECT J.JNO
FROM J
WHERE J.JNO NOT IN (
    SELECT SPJ.JNO
    FROM SPJ
    JOIN S
        ON SPJ.SNO = S.SNO
    JOIN P
        ON SPJ.PNO = P.PNO
    WHERE S.CITY = '天津'
        AND P.COLOR = '红'
)