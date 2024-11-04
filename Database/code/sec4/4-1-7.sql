CREATE VIEW '部门工资统计' AS
SELECT '部门号', 
    MAX('工资') AS '最高工资', 
    MIN('工资') AS '最低工资', 
    AVG('工资') AS '平均工资'
FROM '职工'
GROUP BY '部门号';

GRANT SELECT
ON TABLE '部门工资统计'
TO '杨兰';