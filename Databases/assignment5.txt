#Justin McLain z1910087 csci 466 spring 2021

use BabyName; #selects the database

show tables; #shows all tables in BabyName

describe BabyName; #describes columns in BabyName

select name 
       from BabyName 
       where year = '1999' 
       group by name 
       limit 50; #selects the column of name finds names with birthday of 1999 and limits output to 50

select 
      distinct year 
      from BabyName 
      where name = 'Justin' 
      limit 50; #this reports a year once from table babyname where the name is Justin limits output to 50

(select 
    `name`,
    `gender`,
    count(*) as `num_found`
from BabyName 
where `year` = '1984' AND `gender` = 'M'
GROUP BY `name`,`gender`
ORDER BY `num_found` desc
limit 1)
UNION ALL
(select 
    `name`,
    `gender`,
    count(*) as `num_found`
from BabyName 
where `year` = '1984' AND `gender` = 'F'
GROUP BY `name`,`gender`
ORDER BY `num_found` desc
limit 1);

select place,
       count(DISTINCT name) as numName 
       from BabyName 
       GROUP BY place
       ; #this displays number of names as numName and groups them by place

select `name`, 
        count(*) as `num_found`, 
       `year` 
        from BabyName 
        where name like 'Jus%' 
        group by `name` 
        order by name, `num_found` , `year` 
        limit 50;  #this displays a name similar to Jus and outputs by name number found and the year

select year, 
       count(*) as `names`
from BabyName
where year = '1974'
;  #this displays a specific year and the amount of names within that year

select count(*) as 'rows'
	from BabyName
; #displays the number of rows in the table 'BabyName' and changes count label to rows

(select 
    `gender`,
    count(*) as `nameCount`
from BabyName 
where `year` = '1962' and `gender` = 'M'
GROUP BY `gender`
ORDER BY `nameCount` desc
limit 1)
union all
(select 
    `gender`,
    count(*) as `nameCount`
from BabyName 
where `year` = '1962' and `gender` = 'F'
GROUP BY `gender`
ORDER BY `nameCount` desc
limit 1); #displays the gender in a column and then displays the total amount of names from year 1962