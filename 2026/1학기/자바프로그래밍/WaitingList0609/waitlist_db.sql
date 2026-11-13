-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- 생성 시간: 26-05-16 16:30
-- 서버 버전: 10.4.32-MariaDB
-- PHP 버전: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";

CREATE DATABASE IF NOT EXISTS waitlist_db
DEFAULT CHARACTER SET utf8mb4
COLLATE utf8mb4_general_ci;

USE waitlist_db;

DROP TABLE IF EXISTS `store_setting`;
DROP TABLE IF EXISTS `waiting`;
DROP TABLE IF EXISTS `member`;


START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- 데이터베이스: `waitlist_db`
--

-- --------------------------------------------------------

--
-- 테이블 구조 `store_setting`
--

-- 회원 정보 테이블

CREATE TABLE `member` (
  `member_id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL,
  `phone` varchar(20) NOT NULL,
  `login_id` varchar(50) NOT NULL,
  `password` varchar(255) NOT NULL,
  `is_admin` tinyint(1) NOT NULL DEFAULT 0,
  `created_at` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

CREATE TABLE `store_setting` (
  `id` int(11) NOT NULL,
  `waiting_status` enum('OPEN','PAUSED','CLOSED') NOT NULL DEFAULT 'OPEN',
  `max_waiting_count` int(11) NOT NULL CHECK (`max_waiting_count` > 0),
  `no_show_limit_minutes` int(11) NOT NULL CHECK (`no_show_limit_minutes` > 0),
  `updated_at` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- 테이블의 덤프 데이터 `store_setting`
--

INSERT INTO `store_setting` (`id`, `waiting_status`, `max_waiting_count`, `no_show_limit_minutes`, `updated_at`) VALUES
(1, 'OPEN', 30, 3, '2026-05-16 23:16:19');

-- --------------------------------------------------------

--
-- 테이블 구조 `waiting`
--

CREATE TABLE `waiting` (
  `waiting_num` int(11) NOT NULL,
  `member_id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL,
  `phone` varchar(20) NOT NULL,
  `total_count` int(11) NOT NULL CHECK (`total_count` > 0),
  `status` enum('WAIT','CALL','IN','CANCEL','NO_SHOW') NOT NULL DEFAULT 'WAIT',
  `created_at` datetime NOT NULL DEFAULT current_timestamp(),
  `called_at` datetime DEFAULT NULL,
  `entered_at` datetime DEFAULT NULL,
  `cancelled_at` datetime DEFAULT NULL,
  `no_show_at` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- 덤프된 테이블의 인덱스
--

--
-- 테이블의 인덱스 `store_setting`
--
ALTER TABLE `member`
  ADD PRIMARY KEY (`member_id`),
  ADD UNIQUE KEY `uk_member_phone` (`phone`),
  ADD UNIQUE KEY `uk_member_login_id` (`login_id`);

ALTER TABLE `store_setting`
  ADD PRIMARY KEY (`id`);

--
-- 테이블의 인덱스 `waiting`
--
ALTER TABLE `waiting`
  ADD PRIMARY KEY (`waiting_num`),
  ADD KEY `fk_waiting_member` (`member_id`);

--
-- 덤프된 테이블의 AUTO_INCREMENT
--

--
-- 테이블의 AUTO_INCREMENT `store_setting`
--
ALTER TABLE `member`
  MODIFY `member_id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `store_setting`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- 테이블의 AUTO_INCREMENT `waiting`
--
ALTER TABLE `waiting`
  MODIFY `waiting_num` int(11) NOT NULL AUTO_INCREMENT;

--
-- member_id로 member 테이블과 연결되는 대기 테이블
--
ALTER TABLE `waiting`
  ADD CONSTRAINT `fk_waiting_member`
  FOREIGN KEY (`member_id`) REFERENCES `member` (`member_id`);

--
-- 시연용 관리자/고객 계정 데이터
--
INSERT INTO `member` (`member_id`, `name`, `phone`, `login_id`, `password`, `is_admin`) VALUES
(1, '관리자', '010-0000-0000', 'admin', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918', 1),
(2, '김민준', '010-1234-5678', 'minjun', '4f9e1fc1420e57e1b4b24d454487eba64b8c531a1fd71af370807c6a03ba9574', 0),
(3, '이서연', '010-2345-6789', 'seoyeon', '7cbf35d1ead39f50bfdfb76b3741897ab2f4a6c82207aa54ed9d658307fa64d4', 0),
(4, '박지호', '010-3456-7890', 'jiho', '63c3bf4294075d3233caa67e1376c302b6852fd787ddf90bf2c200d33858fb5d', 0),
(5, '최수아', '010-4567-8901', 'sua', 'faec19c1278b308a886fae4a2f00dd411c38fda363a1d5a2f95fb3b720257336', 0),
(6, '정태양', '010-5678-9012', 'taeyang', '94c7949c4bed346ac230180d095c513313f8a8191daec9c2b4402fd446d3573b', 0),
(7, '한나라', '010-6789-0123', 'nara', 'f30fc0e061eba665e0a964fdcd418a723c5c940bb576cbcd41d3659b852dc8cf', 0),
(8, '오민석', '010-7890-1234', 'minseok', '931f1276de382665792f9aff236ce674f6d20513ecf0cd738f154d7646008132', 0),
(9, '윤채원', '010-8901-2345', 'chaewon', '9c856db0a6374b5e8371ec96c99a67a8813b9e0af8ac367d29dacdaca620190c', 0),
(10, '장현우', '010-9012-3456', 'hyunwoo', '1d7b63b3b0f4098deb844db3ab52e048515e4c48cf3ec3127dae62bc4e0004c1', 0),
(11, '임소율', '010-0123-4567', 'soyul', '56c82b3e852d7ec938d66e219c7f7ceb627d6df8e0716cca0bf899662627c016', 0),
(12, '강도윤', '010-1357-2468', 'doyoon', '4af454b4e790cac992c0a3590aff7f49337a210bd19875be23feedc91c537e49', 0),
(13, '신예은', '010-2468-1357', 'yeeun', '30882080607b99e0dafd7125f200781d19c6847313c48c9d77f6694090adbfe0', 0);

--
-- 시연용 대기 데이터(member_id로 회원과 연결)
--
INSERT INTO `waiting` (`waiting_num`, `member_id`, `name`, `phone`, `total_count`, `status`) VALUES
(1, 2, '김민준', '010-1234-5678', 2, 'WAIT'),
(2, 3, '이서연', '010-2345-6789', 4, 'WAIT'),
(3, 4, '박지호', '010-3456-7890', 3, 'WAIT'),
(4, 5, '최수아', '010-4567-8901', 5, 'WAIT'),
(5, 6, '정태양', '010-5678-9012', 2, 'WAIT'),
(6, 7, '한나라', '010-6789-0123', 6, 'WAIT'),
(7, 8, '오민석', '010-7890-1234', 3, 'WAIT'),
(8, 9, '윤채원', '010-8901-2345', 4, 'WAIT'),
(9, 10, '장현우', '010-9012-3456', 2, 'WAIT'),
(10, 11, '임소율', '010-0123-4567', 5, 'WAIT'),
(11, 12, '강도윤', '010-1357-2468', 3, 'WAIT'),
(12, 13, '신예은', '010-2468-1357', 4, 'WAIT');
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
