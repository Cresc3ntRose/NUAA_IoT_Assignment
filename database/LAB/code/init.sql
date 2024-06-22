-- Drop the course table if it exists to ensure a clean setup
DROP TABLE IF EXISTS course;

-- Drop the teacher table if it exists to avoid errors
DROP TABLE IF EXISTS teacher;

-- Create the teacher table with necessary fields
CREATE TABLE teacher (
    id SERIAL PRIMARY KEY,
    name VARCHAR(140) NOT NULL,
    picture_url VARCHAR(255) UNIQUE NOT NULL,
    profile VARCHAR(2000) NOT NULL
);

-- Create the course table with a reference to the teacher table's ID as a foreign key
CREATE TABLE course (
    id SERIAL PRIMARY KEY,
    teacher_id INT NOT NULL,
    name VARCHAR(140) NOT NULL,
    time TIMESTAMP DEFAULT NOW(),
    description VARCHAR(2000),
    format VARCHAR(30),
    structure VARCHAR(200),
    duration VARCHAR(30),
    price INT,
    language VARCHAR(30),
    level VARCHAR(30),

    FOREIGN KEY (teacher_id) REFERENCES teacher(id)
);

-- Insert data into the teacher table
INSERT INTO teacher (name, picture_url, profile)
VALUES (
       'Yuxuan Huang',
       'https://github.com/Cresc3ntRose',
       'This is an easter egg'
), (
       'Fei Gao',
       'https://person.zju.edu.cn/fgaoaa',
       'a teacher in FAST_Lab, ZJU'
), (
       'Boyu Zhou',
       'https://boyuzhou.net/pub/',
       'a teacher in STAR_Lab, SYSU'
), (
       'Shaojie Shen',
       'https://seng.hkust.edu.hk/about/people/faculty/shaojie-shen',
       'a teacher in DJI_Joint_Lab, HKUST'
), (
       'Guoliang Li',
       'https://dbgroup.cs.tsinghua.edu.cn/ligl',
       'a teacher in DB_Group, THU'
), (
       'Yanyan Jiang',
       'https://jyywiki.cn/',
       'a teacher in SPAR, NJU'
), (
       'Kun Zhu',
       'http://inet-nuaa.cn/kunzhu/',
       'a teacher in INET, NUAA'
), (
       'Yong Liu',
       'https://person.zju.edu.cn/yongliu',
       'a teacher in APRIL, ZJU'
), (
       'Mingming Chen',
       'https://mmcheng.net/cmm/',
       'a teacher in CCS, NKU'
), (
       'Bingwen Huang',
       'https://gsai.ruc.edu.cn/addons/teacher/index/info.html?user_id=31&ruccode=DjwFPFJhW20AZwIyBmMAMw%3D%3D&ln=cn',
       'a teacher in GSAI, RUC'
), (
       'Chi Guo',
       'http://jszy.whu.edu.cn/guochi/',
       'a teacher in LuoJia_Lab, WHU'
), (
       'Shengfeng He',
       'http://www.shengfenghe.com/',
       'a teacher in VUG_Lab, SMU'
);

-- Assuming Yuxuan Huang's ID is 1, insert data into the course table
INSERT INTO course (teacher_id, name, time)
VALUES
(1, 'First course', '2024-06-17 05:40:00'),
(1, 'Second course', '2024-06-18 05:45:00'),
(1, 'Third course', '2024-06-18 05:45:00'),
(1, 'Fourth course', '2024-06-18 05:45:00'),
(1, 'Fifth course', '2024-06-18 05:45:00'),
(1, 'Sixth course', '2024-06-18 05:45:00'),
(1, 'Seventh course', '2024-06-18 05:45:00'),
(1, 'Eighth course', '2024-06-18 05:45:00'),
(1, 'Ninth course', '2024-06-18 05:45:00'),
(1, 'Tenth course', '2024-06-18 05:45:00'),
(1, 'Eleventh course', '2024-06-18 05:45:00'),
(1, 'Twelfth course', '2024-06-18 05:45:00');
