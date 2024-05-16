SELECT J.JNO
FROM J
WHERE NOT EXISTS (
    SELECT PNO
    FROM SPJ SPJX
    WHERE SPJX.SNO = 'S1'
        AND NOT EXISTS (
            SELECT 1
            FROM SPJ
            WHERE SPJ.JNO = J.JNO
                AND SPJ.PNO = SPJX.PNO
        )
);