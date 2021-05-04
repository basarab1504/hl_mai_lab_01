CREATE USER 'petrushin'@'localhost' IDENTIFIED BY 'qwerty123';

CREATE DATABASE petrushin_labs;
GRANT ALL PRIVILEGES ON petrushin_labs.* TO 'petrushin'@'localhost';
FLUSH PRIVILEGES;

use petrushin_labs;
CREATE TABLE IF NOT EXISTS Persosn (
    id INT NOT NULL AUTO_INCREMENT, 
    login VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
    first_name VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
    last_name VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
    age TINYINT UNSIGNED NULL,
    PRIMARY KEY (id), UNIQUE KEY login_hash (login), KEY fn (first_name), KEY ln (last_name));