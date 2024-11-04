CREATE VIEW '职工个人信息' AS 
SELECT * FROM '职工'
WHERE '姓名' = 'USER';

GRANT SELECT
ON TABLE '职工个人信息'
TO PUBLIC;
