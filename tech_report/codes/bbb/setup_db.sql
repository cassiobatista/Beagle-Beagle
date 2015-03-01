/*
 * Author: May 2015
 * Cassio Trindade Batista
 * cassio.batista.13@gmail.com
 * 
 * 1st: connect to MySQL on bash terminal 
 * root@beagle # mysql -u root -p 
 * password: beagle1234 
 */

DROP database if EXISTS ir_db; --delete database
CREATE database if NOT EXISTS ir_db; --create the previous deleted one
USE ir_db; --switch to the database created

DROP table if EXISTS tv; --delete table 
-- create table with fields for the commands used
CREATE table if NOT EXISTS tv (
marca varchar(40) NOT null,
vol_mais varchar(40), 
vol_menos varchar(40),
ch_mais varchar(40), 
ch_menos varchar(40),
on_off varchar(40),
primary key(marca) 
);

-- 35 bits Samsung command length
INSERT into tv(marca, vol_mais, vol_menos, ch_mais, ch_menos, on_off) values (
"samsung",
"1111000001110000011100000000111110", -- volume mais
"1111000001110000011010000001011110", -- volume menos
"1111000001110000001001000101101110", -- canal mais
"1111000001110000000001000111101110", -- canal menos
"1111000001110000001000000101111110"  -- on/off
);

-- 22 bits RC-6 command length
INSERT into tv(marca, vol_mais, vol_menos, ch_mais, ch_menos, on_off) 
values (
"philips",
"1100010000000000010000", -- volume mais
"1100010000000000010001", -- volume menos
"1100010000000001001100", -- canal mais
"1100010000000001001101", -- canal menos
"1100010000000000001100"  -- on/off
);

-- get_element() in C
SELECT vol_mais FROM tv WHERE marca='samsung';
SELECT ch_menos FROM tv WHERE marca='philips';
