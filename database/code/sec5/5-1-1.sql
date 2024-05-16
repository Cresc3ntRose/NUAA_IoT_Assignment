/* 职工表 */
CREATE TABLE EMP (
    Empno NUMBER(4) PRIMARY KEY,
    Ename VARCHAR(10),
    Birthday DATE,
    CONSTRAINT C1 CHECK (
        FLOOR(DATEDIFF(CURDATE(), Birthday) / 365) <= 65
    ),
    Job VARCHAR(9),
    Sal NUMBER(7, 2),
    Deptno NUMBER(2),
    CONSTRAINT FK_DEPTNO FOREIGN KEY (Deptno) REFERENCES DEPT (Deptno)
);

/* 部门表 */
CREATE TABLE DEPT (
    Deptno NUMBER(2) PRIMARY KEY,
    Deptname VARCHAR(10),
    Manager VARCHAR(10),
    PhoneNumber CHAR(12)
);