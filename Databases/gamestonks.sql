MariaDB [z1910087]> create table Exchange (
    -> name char primary key,
    -> userInfo varchar(30) not null,
    -> marketOrders int,
    -> limitOrders int
    -> );
Query OK, 0 rows affected (0.265 sec)

MariaDB [z1910087]> create table Stock ( 
    -> symbol varchar(4) not null primary key, 
    -> name char,  
    -> company varchar(20) not null,  
    -> foreign key (name) references Exchange(name) 
    -> );
Query OK, 0 rows affected (0.321 sec)

MariaDB [z1910087]> create table Customer (
    -> ExcAccount varchar(30) not null, #contains info for access of account
    -> ID int primary key not null, #ID for customer,
    -> BankAcc varchar(30) not null, #contains info to be linked with exchange
    -> firstName varchar(15) not null, #first name
    -> lastName varchar(15) not null, #last name
    -> name char,
    -> foreign key (name) references Exchange(name)
    -> );
Query OK, 0 rows affected (0.304 sec)

MariaDB [z1910087]> create table has (
    -> ID int not null, #foreign key to customer
    -> foreign key (ID) references Customer(ID),
    -> symbol varchar(4) not null, #foreign key to stock
    -> foreign key (symbol) references Stock(symbol),
    -> bought int null, #what was bought
    -> sold int null, #what was sold
    -> held int null #whats being held
    -> );
Query OK, 0 rows affected (0.553 sec)

MariaDB [z1910087]> create table data (
    -> baseID int primary key not null,
    -> name char not null,
    -> foreign key (name) references Exchange(name),
    -> transHis varchar(30) null,
    -> sold int null,
    -> bought int null,
    -> stockPrice double not null,
    -> UserBal double null
    -> );
Query OK, 0 rows affected (0.259 sec)

MariaDB [z1910087]> +-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
    -> Ctrl-C -- exit!
Aborted
z1910087@hopper:~$ mariadb -h courses -u z1910087 -p z1910087
Enter password:
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Welcome to the MariaDB monitor.  Commands end with ; or \g.
Your MariaDB connection id is 2923
Server version: 10.1.41-MariaDB-0+deb9u1 Debian 9.9

Copyright (c) 2000, 2018, Oracle, MariaDB Corporation Ab and others.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

MariaDB [z1910087]> show tables
    -> \c
MariaDB [z1910087]> show tables;
+--------------------+
| Tables_in_z1910087 |
+--------------------+
| Customer           |
| Exchange           |
| Stock              |
| data               |
| has                |
+--------------------+
5 rows in set (0.001 sec)

MariaDB [z1910087]> show table Exchange;
ERROR 1064 (42000): You have an error in your SQL syntax; check the manual that corresponds to your MariaDB server version for the right syntax to use near 'Exchange' at line 1
MariaDB [z1910087]> describe Exchange;
+--------------+-------------+------+-----+---------+-------+
| Field        | Type        | Null | Key | Default | Extra |
+--------------+-------------+------+-----+---------+-------+
| name         | char(1)     | NO   | PRI | NULL    |       |
| userInfo     | varchar(30) | NO   |     | NULL    |       |
| marketOrders | int(11)     | YES  |     | NULL    |       |
| limitOrders  | int(11)     | YES  |     | NULL    |       |
+--------------+-------------+------+-----+---------+-------+
4 rows in set (0.001 sec)

MariaDB [z1910087]> describe Customer;
+------------+-------------+------+-----+---------+-------+
| Field      | Type        | Null | Key | Default | Extra |
+------------+-------------+------+-----+---------+-------+
| ExcAccount | varchar(30) | NO   |     | NULL    |       |
| ID         | int(11)     | NO   | PRI | NULL    |       |
| BankAcc    | varchar(30) | NO   |     | NULL    |       |
| firstName  | varchar(15) | NO   |     | NULL    |       |
| lastName   | varchar(15) | NO   |     | NULL    |       |
| name       | char(1)     | NO   | MUL | NULL    |       |
+------------+-------------+------+-----+---------+-------+
6 rows in set (0.001 sec)

MariaDB [z1910087]> show create table Customer;
+----------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Table    | Create Table                                                                                                                                                                                                                                                                                                                                                                                        |
+----------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Customer | CREATE TABLE `Customer` (
  `ExcAccount` varchar(30) NOT NULL,
  `ID` int(11) NOT NULL,
  `BankAcc` varchar(30) NOT NULL,
  `firstName` varchar(15) NOT NULL,
  `lastName` varchar(15) NOT NULL,
  `name` char(1) NOT NULL,
  PRIMARY KEY (`ID`),
  KEY `name` (`name`),
  CONSTRAINT `Customer_ibfk_1` FOREIGN KEY (`name`) REFERENCES `Exchange` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 |
+----------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
1 row in set (0.001 sec)

MariaDB [z1910087]> describe Stock;
+---------+-------------+------+-----+---------+-------+
| Field   | Type        | Null | Key | Default | Extra |
+---------+-------------+------+-----+---------+-------+
| symbol  | varchar(4)  | NO   | PRI | NULL    |       |
| name    | char(1)     | YES  | MUL | NULL    |       |
| company | varchar(20) | NO   |     | NULL    |       |
+---------+-------------+------+-----+---------+-------+
3 rows in set (0.001 sec)

MariaDB [z1910087]> show create table Stock;
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Table | Create Table                                                                                                                                                                                                                                                                                     |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Stock | CREATE TABLE `Stock` (
  `symbol` varchar(4) NOT NULL,
  `name` char(1) DEFAULT NULL,
  `company` varchar(20) NOT NULL,
  PRIMARY KEY (`symbol`),
  KEY `name` (`name`),
  CONSTRAINT `Stock_ibfk_1` FOREIGN KEY (`name`) REFERENCES `Exchange` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
1 row in set (0.001 sec)

MariaDB [z1910087]> describe has;
+--------+------------+------+-----+---------+-------+
| Field  | Type       | Null | Key | Default | Extra |
+--------+------------+------+-----+---------+-------+
| ID     | int(11)    | NO   | MUL | NULL    |       |
| symbol | varchar(4) | NO   | MUL | NULL    |       |
| bought | int(11)    | YES  |     | NULL    |       |
| sold   | int(11)    | YES  |     | NULL    |       |
| held   | int(11)    | YES  |     | NULL    |       |
+--------+------------+------+-----+---------+-------+
5 rows in set (0.001 sec)

MariaDB [z1910087]> show create table has;
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Table | Create Table                                                                                                                                                                                                                                                                                                                                                                                                                 |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| has   | CREATE TABLE `has` (
  `ID` int(11) NOT NULL,
  `symbol` varchar(4) NOT NULL,
  `bought` int(11) DEFAULT NULL,
  `sold` int(11) DEFAULT NULL,
  `held` int(11) DEFAULT NULL,
  KEY `ID` (`ID`),
  KEY `symbol` (`symbol`),
  CONSTRAINT `has_ibfk_1` FOREIGN KEY (`ID`) REFERENCES `Customer` (`ID`),
  CONSTRAINT `has_ibfk_2` FOREIGN KEY (`symbol`) REFERENCES `Stock` (`symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
1 row in set (0.001 sec)

MariaDB [z1910087]> describe data;
+------------+-------------+------+-----+---------+-------+
| Field      | Type        | Null | Key | Default | Extra |
+------------+-------------+------+-----+---------+-------+
| baseID     | int(11)     | NO   | PRI | NULL    |       |
| name       | char(1)     | NO   | MUL | NULL    |       |
| transHis   | varchar(30) | YES  |     | NULL    |       |
| sold       | int(11)     | YES  |     | NULL    |       |
| bought     | int(11)     | YES  |     | NULL    |       |
| stockPrice | double      | NO   |     | NULL    |       |
| UserBal    | double      | YES  |     | NULL    |       |
+------------+-------------+------+-----+---------+-------+
7 rows in set (0.001 sec)

MariaDB [z1910087]> show create table data;
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Table | Create Table                                                                                                                                                                                                                                                                                                                                                                                                                  |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| data  | CREATE TABLE `data` (
  `baseID` int(11) NOT NULL,
  `name` char(1) NOT NULL,
  `transHis` varchar(30) DEFAULT NULL,
  `sold` int(11) DEFAULT NULL,
  `bought` int(11) DEFAULT NULL,
  `stockPrice` double NOT NULL,
  `UserBal` double DEFAULT NULL,
  PRIMARY KEY (`baseID`),
  KEY `name` (`name`),
  CONSTRAINT `data_ibfk_1` FOREIGN KEY (`name`) REFERENCES `Exchange` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
1 row in set (0.001 sec)

